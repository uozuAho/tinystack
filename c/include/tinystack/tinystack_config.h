#ifndef TINYSTACK_CONFIG_H
#define TINYSTACK_CONFIG_H

// assert and stdio used for PC debugging & output
#include <assert.h>
#include <stdio.h>


//------------------------------------------------------------------------
// user config

/// Function used for assertions. Assumes cstdlib-style assert
#define TINYSTACK_ASSERT(x)             assert(x)

/// Function used for putchar. Assumes cstdlib-style putchar
/// ie. takes one int parameter, returns an int
#define TINYSTACK_PUTCHAR(x)            putchar(x)

/// Turn tinystack debugging output on/off
#define TINYSTACK_DEBUG                 0


// end user config
//------------------------------------------------------------------------


#ifndef TINYSTACK_DEBUG
    #warning "TINYSTACK_DEBUG not defined, disabling"
    #define TINYSTACK_DEBUG                 0
#endif

#if TINYSTACK_DEBUG
    #include <stdio.h>

    int _dbg_putchar(int x);

    #undef TINYSTACK_PUTCHAR
    #define TINYSTACK_PUTCHAR(x)    _dbg_putchar(x)

    #define dbg(x)                  printf(x)
    #define dbg_num(x)              printf("%d", x)
#else
    #define dbg(x)
    #define dbg_num(x)
#endif

#endif // TINYSTACK_CONFIG_H