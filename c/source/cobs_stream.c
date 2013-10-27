/**
    Byte-stream input/output with COBS encoding
*/

#include "tinystack/tinystack_config.h"
#include "tinystack/cobs.h"


//------------------------------------------------------------------------
// public functions

void
Cobs_vSendBlocking(const uint8_t* const data, const size_t len)
{
    uint8_t i = 0;
    uint8_t frame_start_idx = 0;
    uint8_t send_idx = 0;
    uint8_t final_zero = 1; // true/false

    dbg("\nsend cobs data. len: "); dbg_num(len); dbg("\n");
    TINYSTACK_ASSERT(data != NULL);
    TINYSTACK_ASSERT(len > 0);

    for (; i < len; i++)
    {
        if (data[i] == 0)
        {
            uint8_t j = frame_start_idx;
            final_zero = 1;
            TINYSTACK_PUTCHAR(send_idx - frame_start_idx + 1);
            for (; j < send_idx; j++)
                TINYSTACK_PUTCHAR(data[j]);
            frame_start_idx = send_idx + 1;
        }
        else
        {
            if (send_idx - frame_start_idx == 0xFD)
            {
                uint8_t j = frame_start_idx;
                final_zero = 0;
                TINYSTACK_PUTCHAR(0xFF);
                for (; j < (send_idx + 1); j++)
                    TINYSTACK_PUTCHAR(data[j]);
                frame_start_idx = send_idx + 1;
            }
        }
        send_idx++;
    }

    if (send_idx != frame_start_idx || final_zero)
    {
        uint8_t j = frame_start_idx;
        TINYSTACK_PUTCHAR(send_idx - frame_start_idx + 1);
        for (; j < send_idx; j++)
            TINYSTACK_PUTCHAR(data[j]);
    }
}
