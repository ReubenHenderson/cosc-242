#ifndef MYLIB_H_
#define MYLIB_H_

#include <stddef.h>
#include <stdio.h> /* for fprintf */
#include <stdlib.h> /* for size_t, malloc, realloc, exit */
#include <assert.h>
#include <ctype.h>

extern void *emalloc(size_t);
extern void *erealloc(void *, size_t);
extern int getword(char *, int, FILE *);

#endif
