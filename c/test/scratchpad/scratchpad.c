#include <stdio.h>
#include "tinystack/crc.h"
#include "tinystack/cobs_stream.h"

void crctest()
{
    int intbuf[] = {1, 2, 3, 4, 5};
    printf("%d\n", crc16_ccitt((void*)intbuf, 5));
}

const uint8_t pkt1[] =   {0};
const size_t  pkt1_len = 1;

const uint8_t pkt2[] =   {0x11, 0x22, 0x00, 0x33};
const size_t  pkt2_len = 4;

const uint8_t pkt3[] =   {0x11, 0x00, 0x00, 0x00};
const size_t  pkt3_len = 4;

void cobs_ostream_test()
{
    Cobs_vSendBlocking(pkt1, pkt1_len);
    Cobs_vSendBlocking(pkt2, pkt2_len);
    Cobs_vSendBlocking(pkt3, pkt3_len);
}

int main()
{
    crctest();
    cobs_ostream_test();
    return 0;
}
