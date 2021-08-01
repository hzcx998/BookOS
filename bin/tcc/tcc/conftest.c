#include <stdio.h>

/* Define architecture */
#if defined(__i386__) || defined _M_IX86
# define TRIPLET_ARCH "i386"
#elif defined(__x86_64__) || defined _M_AMD64
# define TRIPLET_ARCH "x86_64"
#endif

/* Define OS */
#define TRIPLET_OS "xbook"

/* Define calling convention and ABI */
#define TRIPLET_ABI "gnu"

#ifdef __GNU__
# define TRIPLET TRIPLET_ARCH "-" TRIPLET_ABI
#else
# define TRIPLET TRIPLET_ARCH "-" TRIPLET_OS "-" TRIPLET_ABI
#endif

int main(int argc, char *argv[])
{
    switch(argc == 2 ? argv[1][0] : 0) {
        case 'b':
        {
            volatile unsigned foo = 0x01234567;
            puts(*(unsigned char*)&foo == 0x67 ? "no" : "yes");
            break;
        }
#ifdef __GNUC__
        case 'm':
            printf("%d\n", __GNUC_MINOR__);
            break;
        case 'v':
            printf("%d\n", __GNUC__);
            break;
#elif defined __TINYC__
        case 'v':
            puts("0");
            break;
        case 'm':
            printf("%d\n", __TINYC__);
            break;
#else
        case 'm':
        case 'v':
            puts("0");
            break;
#endif
        case 't':
            puts(TRIPLET);
            break;

        default:
            break;
    }
    return 0;
}
