
#ifndef _XLIBC_MALLOC_H
#define _XLIBC_MALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

void free(void *ptr);
void *calloc(int num, size_t size);
void *malloc(size_t size);
void *realloc(void *oldp, size_t size);
void *memalign (size_t boundary, size_t size);

extern void (*__free_hook) (void * __ptr, const void *);
extern void * (*__malloc_hook) (size_t __size, const void *);
extern void * (*__realloc_hook) (void * __ptr, size_t __size, const void *);
extern void * (*__memalign_hook) (size_t __alignment, size_t __size, const void *);

#ifdef __cplusplus
}
#endif

#endif  /* _XLIBC_MALLOC_H */