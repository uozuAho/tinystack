#ifndef TINYSTACK_CONFIG_H
#define TINYSTACK_CONFIG_H

#include <assert.h>
#include <stdio.h>

#define TINYSTACK_ASSERT(x)             assert(x)

// TODO: replace this with an ADT that can
// be initialised within pointers to putchar-like
// functions
#define TINYSTACK_PUTCHAR(x)            putchar(x)

#endif // TINYSTACK_CONFIG_H