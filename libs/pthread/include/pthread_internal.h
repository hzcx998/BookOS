#ifndef _SYS_PTHREAD_INTERNAL_H
#define _SYS_PTHREAD_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <arch/atomic.h>
#include <sys/time.h>
#include <sys/list.h>
#include <signal.h>
#include <stdbool.h>

#define PTHREAD_KEYS_MAX    1024

/* Thread-local data handling.  */
struct pthread_key_struct
{
  /* Sequence numbers.  Even numbers indicated vacant entries.  Note
     that zero is even.  We use uintptr_t to not require padding on
     32- and 64-bit machines.  On 64-bit machines it helps to avoid
     wrapping, too.  */
  uintptr_t seq;

  /* Destructor for the data.  */
  void (*destr) (void *);
};

/* We keep thread specific data in a special data structure, a two-level
array.  The top-level array contains pointers to dynamically allocated
arrays of a certain number of data pointers.  So we can implement a
sparse array.  Each dynamic second-level array has
        PTHREAD_KEY_2NDLEVEL_SIZE
entries.  This value shouldn't be too large.  */
#define PTHREAD_KEY_2NDLEVEL_SIZE       32

/* We need to address PTHREAD_KEYS_MAX key with PTHREAD_KEY_2NDLEVEL_SIZE
keys in each subarray.  */
#define PTHREAD_KEY_1STLEVEL_SIZE \
    ((PTHREAD_KEYS_MAX + PTHREAD_KEY_2NDLEVEL_SIZE - 1) \
    / PTHREAD_KEY_2NDLEVEL_SIZE)


/* Controlling the iterations of destructors for thread-specific data.  */
#define _POSIX_THREAD_DESTRUCTOR_ITERATIONS     4
/* Number of iterations this implementation does.  */
#define PTHREAD_DESTRUCTOR_ITERATIONS   _POSIX_THREAD_DESTRUCTOR_ITERATIONS

struct pthread {
    list_t list;
        
    /* Thread ID - which is also a 'is this thread descriptor (and
        therefore stack) used' flag.  */
    pid_t tid;

    /* Process ID - thread group ID in kernel speak.  */
    pid_t pid;

    
    /* We allocate one block of references here.  This should be enough
        to avoid allocating any memory dynamically for most applications.  */
    struct pthread_key_data
    {
        /* Sequence number.  We use uintptr_t to not require padding on
            32- and 64-bit machines.  On 64-bit machines it helps to avoid
            wrapping, too.  */
        uintptr_t seq;


        /* Data pointer.  */
        void *data;
    } specific_1stblock[PTHREAD_KEY_2NDLEVEL_SIZE];

    /* Two-level array for the thread-specific data.  */
    struct pthread_key_data *specific[PTHREAD_KEY_1STLEVEL_SIZE];
    
    /* Flag which is set when specific data is set.  */
    bool specific_used;
};

struct pthread *pthread_struct_ptr();

/* Check whether an entry is unused.  */
#define KEY_UNUSED(p) (((p) & 1) == 0)
/* Check whether a key is usable.  We cannot reuse an allocated key if
   the sequence counter would overflow after the next destroy call.
   This would mean that we potentially free memory for a key with the
   same sequence.  This is *very* unlikely to happen, A program would
   have to create and destroy a key 2^31 times (on 32-bit platforms,
   on 64-bit platforms that would be 2^63).  If it should happen we
   simply don't use this specific key anymore.  */
#define KEY_USABLE(p) (((uintptr_t) (p)) < ((uintptr_t) ((p) + 2)))

/* Return the thread descriptor for the current thread.  */
# define THREAD_SELF \
 ((struct pthread *) pthread_struct_ptr())

/* Access to data in the thread descriptor is easy.  */
#define THREAD_GETMEM(descr, member) \
  descr->member
#define THREAD_GETMEM_NC(descr, member, idx) \
  descr->member[idx]
#define THREAD_SETMEM(descr, member, value) \
  descr->member = (value)
#define THREAD_SETMEM_NC(descr, member, idx, value) \
  descr->member[idx] = (value)

void __pthread_deallocate_tsd(void);


#ifdef __cplusplus
}
#endif

#endif /* _SYS_PTHREAD_INTERNAL_H */