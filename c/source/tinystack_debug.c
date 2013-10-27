#include <stdio.h>

int _dbg_putchar(int x)
{
    // byte per line
    // printf("%02x,  %c\n", x, x);

    // cobs frame per line
    printf("%02x:", x);
    return x;
}
