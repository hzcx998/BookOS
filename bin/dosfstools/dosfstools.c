#include <wchar.h>
#include <wctype.h>

size_t mbstowcs(wchar_t *__restrict pwcs, const char *__restrict s, size_t n) {
	int count = 0;

	if (n != 0) {
		do {
			if ((*pwcs++ = (wchar_t) *s++) == 0){
				break;
			}

			count++;
		} while (--n != 0);
	}

	return count;
}

size_t wcstombs(char *__restrict s, const wchar_t *__restrict pwcs, size_t n)
{
	int count = 0;

	if (n != 0) {
		do {
			if ((*s++ = (char) *pwcs++) == 0) {
				break;
			}
			count++;
		} while (--n != 0);
	}

	return count;
}

wint_t towupper(wint_t c) {
	return (c < 0x00ff ? (wint_t)(toupper((int)c)) : c);
}

int iswlower(wint_t c) {
	return (towupper(c) != c);
}

/*
 * https://github.com/digilus/getline/blob/master/getline.c
 */
#define GETLINE_MINSIZE 16
int getline(char **lineptr, size_t *n, FILE *fp) {
    int ch;
    int i = 0;
    char free_on_err = 0;
    char *p;

    errno = 0;
    if (lineptr == NULL || n == NULL || fp == NULL) {
        errno = EINVAL;
        return -1;
    }
    if (*lineptr == NULL) {
        *n = GETLINE_MINSIZE;
        *lineptr = (char *)malloc( sizeof(char) * (*n));
        if (*lineptr == NULL) {
            errno = ENOMEM;
            return -1;
        }
        free_on_err = 1;
    }

    for (i=0; ; i++) {
        ch = fgetc(fp);
        while (i >= (*n) - 2) {
            *n *= 2;
            p = realloc(*lineptr, sizeof(char) * (*n));
            if (p == NULL) {
                if (free_on_err)
                    free(*lineptr);
                errno = ENOMEM;
                return -1;
            }
            *lineptr = p;
        }
        if (ch == EOF) {
            if (i == 0) {
                if (free_on_err)
                    free(*lineptr);
                return -1;
            }
            (*lineptr)[i] = '\0';
            *n = i;
            return i;
        }

        if (ch == '\n') {
            (*lineptr)[i] = '\n';
            (*lineptr)[i+1] = '\0';
            *n = i+1;
            return i+1;
        }
        (*lineptr)[i] = (char)ch;
    }
}