#include <stdio.h>
#include "tinystack/crc.h"

void crctest()
{
    int intbuf[] = {1, 2, 3, 4, 5};
    printf("%d\n", crc16_ccitt((void*)intbuf, 5));
}

int main()
{
    crctest();
    return 0;
}
