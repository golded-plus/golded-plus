/*
 *  PATMAT.C - Pattern matching. Taken from sh sources
 */

/*
 * Written by Alexander S. Aganichev.
 *
 * Released to public domain.
 */

/*
 * Returns true if the pattern matches the string.
 */

int patmat (char *tag, char *mask)
{
    if (tag && mask) {
        while (*tag && *mask)
            if ((*tag == *mask) || (*mask == '?')) {
                ++ tag;
                ++ mask;
            } else if (*mask == '*') {
                while ((*mask == '*') || (*mask == '?')) {
                    if (*mask == '?') {
                        if (*tag)
                            ++ tag;
                        else
                            return 0;
                    }
                    ++ mask;
                }
                if (*mask) {
                    while (*tag && !patmat (tag, mask))
                        ++ tag;
                    if (*tag)
                        return 1;
                } else
                    return 1;
            } else
                return 0;
        while (*mask == '*')
            ++ mask;
        if (*mask)
            return 0;
        return (*tag) ? 0 : 1;
    }
    return 0;
}

#ifdef TEST

#include <stdio.h>

int main(void)
{
    printf("patmat(\"abcdefghi\", \"*ghi\"): %d\n", patmat("abcdefghi", "*ghi"));
    printf("patmat(\"abcdefghi\", \"??c??f*\"): %d\n", patmat("abcdefghi", "??c??f*"));
    printf("patmat(\"abcdefghi\", \"*dh*\"): %d\n", patmat("abcdefghi", "*dh*"));
    printf("patmat(\"abcdefghi\", \"*def\"): %d\n", patmat("abcdefghi", "*def"));
    return 0;
}

#endif
