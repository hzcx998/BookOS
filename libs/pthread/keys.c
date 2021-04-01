#if 0
#include <pthread.h>
#include <pthread_internal.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>

extern int __pthread_init_flags; 

/* Table of the key information.  */
struct pthread_key_struct __pthread_keys[PTHREAD_KEYS_MAX];

int pthread_key_create (pthread_key_t *key, void (*destr) (void *))
{
    if (!__pthread_init_flags) {
        _set_errno(EPERM);
        perror("pthread_key_create: must called in one thread!\n");
        return -1;
    }
    /* Find a slot in __pthread_keys which is unused.  */
    size_t cnt;
    for (cnt = 0; cnt < PTHREAD_KEYS_MAX; ++cnt) {
        uintptr_t seq = __pthread_keys[cnt].seq;
        if (KEY_UNUSED (seq) && KEY_USABLE (seq)
	    /* We found an unused slot.  Try to allocate it.  */
        && ! atomic_compare_and_exchange_bool_acq (&__pthread_keys[cnt].seq,
        seq + 1, seq)) {
            /* Remember the destructor.  */
            __pthread_keys[cnt].destr = destr;

            /* Return the key to the caller.  */
            *key = cnt;
            
            /* The call succeeded.  */
            return 0;
	    }
    }
    return EAGAIN;
}

int pthread_key_delete (pthread_key_t key)
{
    if (!__pthread_init_flags) {
        _set_errno(EPERM);
        perror("pthread_key_delete: must called in one thread!\n");
        return -1;
    }
    
    int result = EINVAL;
    if (key < PTHREAD_KEYS_MAX) {
        unsigned int seq = __pthread_keys[key].seq;
        if (__builtin_expect (! KEY_UNUSED (seq), 1)
        && ! atomic_compare_and_exchange_bool_acq (&__pthread_keys[key].seq,
                                seq + 1, seq))
        /* We deleted a valid key.  */
        result = 0;
    }
    return result;
}


void *pthread_getspecific (pthread_key_t key)
{
    if (!__pthread_init_flags) {
        _set_errno(EPERM);
        perror("pthread_getspecific: must called in one thread!\n");
        return NULL;
    }
    
    struct pthread_key_data *data;
    /* Special case access to the first 2nd-level block.  This is the
        usual case.  */
    if (key < PTHREAD_KEY_2NDLEVEL_SIZE)
        data = &THREAD_SELF->specific_1stblock[key];
    else {
        /* Verify the key is sane.  */
        if (key >= PTHREAD_KEYS_MAX)
            /* Not valid.  */
            return NULL;

        unsigned int idx1st = key / PTHREAD_KEY_2NDLEVEL_SIZE;
        unsigned int idx2nd = key % PTHREAD_KEY_2NDLEVEL_SIZE;

        /* If the sequence number doesn't match or the key cannot be defined
        for this thread since the second level array is not allocated
        return NULL, too.  */
        struct pthread_key_data *level2 = THREAD_GETMEM_NC (THREAD_SELF,
                                specific, idx1st);
        if (level2 == NULL)
            /* Not allocated, therefore no data.  */
            return NULL;

        /* There is data.  */
        data = &level2[idx2nd];
    }

    void *result = data->data;
    if (result != NULL) {
        uintptr_t seq = data->seq;
        if (seq != __pthread_keys[key].seq)
           result = data->data = NULL;
    }
    return result;
}

int pthread_setspecific (pthread_key_t key, const void *value)
{
    if (!__pthread_init_flags) {
        _set_errno(EPERM);
        perror("pthread_setspecific: must called in one thread!\n");
        return -1;
    }
    
    struct pthread *self;
    unsigned int idx1st;
    unsigned int idx2nd;
    struct pthread_key_data *level2;
    unsigned int seq;

    self = THREAD_SELF;

    /* Special case access to the first 2nd-level block.  This is the
        usual case.  */
    if (key < PTHREAD_KEY_2NDLEVEL_SIZE) {
        /* Verify the key is sane.  */
        if (KEY_UNUSED ((seq = __pthread_keys[key].seq)))
            /* Not valid.  */
            return EINVAL;

        level2 = &self->specific_1stblock[key];

        /* Remember that we stored at least one set of data.  */
        if (value != NULL)
            THREAD_SETMEM (self, specific_used, true);
    } else {
        if (key >= PTHREAD_KEYS_MAX
        || KEY_UNUSED ((seq = __pthread_keys[key].seq)))
            /* Not valid.  */
            return EINVAL;

        idx1st = key / PTHREAD_KEY_2NDLEVEL_SIZE;
        idx2nd = key % PTHREAD_KEY_2NDLEVEL_SIZE;

        /* This is the second level array.  Allocate it if necessary.  */
        level2 = THREAD_GETMEM_NC (self, specific, idx1st);
        if (level2 == NULL) {
            if (value == NULL)
                /* We don't have to do anything.  The value would in any case
                be NULL.  We can save the memory allocation.  */
                return 0;

            level2
                = (struct pthread_key_data *) calloc (PTHREAD_KEY_2NDLEVEL_SIZE,
                                sizeof (*level2));
            if (level2 == NULL)
                return ENOMEM;

            THREAD_SETMEM_NC (self, specific, idx1st, level2);
        }

        /* Pointer to the right array element.  */
        level2 = &level2[idx2nd];

        /* Remember that we stored at least one set of data.  */
        THREAD_SETMEM (self, specific_used, true);
    }
    /* Store the data and the sequence number so that we can recognize
        stale data.  */
    level2->seq = seq;
    level2->data = (void *) value;
    return 0;
}


/* Deallocate POSIX thread-local-storage.  */
void __pthread_deallocate_tsd(void)
{
  struct pthread *self = THREAD_SELF;

  /* Maybe no data was ever allocated.  This happens often so we have
     a flag for this.  */
  if (THREAD_GETMEM (self, specific_used))
    {
      size_t round;
      size_t cnt;

      round = 0;
      do
	{
	  size_t idx;

	  /* So far no new nonzero data entry.  */
	  THREAD_SETMEM (self, specific_used, false);

	  for (cnt = idx = 0; cnt < PTHREAD_KEY_1STLEVEL_SIZE; ++cnt)
	    {
	      struct pthread_key_data *level2;

	      level2 = THREAD_GETMEM_NC (self, specific, cnt);

	      if (level2 != NULL)
		{
		  size_t inner;

		  for (inner = 0; inner < PTHREAD_KEY_2NDLEVEL_SIZE;
		       ++inner, ++idx)
		    {
		      void *data = level2[inner].data;

		      if (data != NULL)
			{
			  /* Always clear the data.  */
			  level2[inner].data = NULL;

			  /* Make sure the data corresponds to a valid
			     key.  This test fails if the key was
			     deallocated and also if it was
			     re-allocated.  It is the user's
			     responsibility to free the memory in this
			     case.  */
			  if (level2[inner].seq
			      == __pthread_keys[idx].seq
			      /* It is not necessary to register a destructor
				 function.  */
			      && __pthread_keys[idx].destr != NULL)
			    /* Call the user-provided destructor.  */
			    __pthread_keys[idx].destr (data);
			}
		    }
		}
	      else
		idx += PTHREAD_KEY_1STLEVEL_SIZE;
	    }

	  if (THREAD_GETMEM (self, specific_used) == 0)
	    /* No data has been modified.  */
	    goto just_free;
	}
      /* We only repeat the process a fixed number of times.  */
      while (__builtin_expect (++round < PTHREAD_DESTRUCTOR_ITERATIONS, 0));

      /* Just clear the memory of the first block for reuse.  */
      memset (&THREAD_SELF->specific_1stblock, '\0',
	      sizeof (self->specific_1stblock));

    just_free:
      /* Free the memory for the other blocks.  */
      for (cnt = 1; cnt < PTHREAD_KEY_1STLEVEL_SIZE; ++cnt)
	{
	  struct pthread_key_data *level2;

	  level2 = THREAD_GETMEM_NC (self, specific, cnt);
	  if (level2 != NULL)
	    {
	      /* The first block is allocated as part of the thread
		 descriptor.  */
	      free (level2);
	      THREAD_SETMEM_NC (self, specific, cnt, NULL);
	    }
	}

      THREAD_SETMEM (self, specific_used, false);
    }
}
#endif