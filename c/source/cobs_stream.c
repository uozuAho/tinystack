/**
    Byte-stream input/output with COBS encoding
*/

#include "tinystack/tinystack_config.h"
#include "tinystack/cobs.h"


//------------------------------------------------------------------------
// constants

const uint8_t COBS_MAX_FRAME_LEN = 0xff;

//------------------------------------------------------------------------
// data

WriteToStreamFunc _streamWrite = NULL;


//------------------------------------------------------------------------
// public functions

void Cobs_vSetStreamWriter(WriteToStreamFunc func)
{
    TINYSTACK_ASSERT(func != NULL);
    _streamWrite = func;
}

void Cobs_vSendBlocking(const uint8_t* const data, const size_t len)
{
    uint8_t i = 0;
    uint8_t frame_start_idx = 0;
    uint8_t send_idx = 0;
    uint8_t final_zero = 1; // true/false

    dbg("\nsend cobs data. len: "); dbg_num(len); dbg("\n");
    TINYSTACK_ASSERT(data != NULL);
    TINYSTACK_ASSERT(_streamWrite != NULL);

    for (; i < len; i++)
    {
        if (data[i] == 0)
        {
            uint8_t frame_len = send_idx - frame_start_idx + 1;
            final_zero = 1;
            _streamWrite(&frame_len, 1);
            _streamWrite(&data[frame_start_idx], frame_len - 1);
            frame_start_idx = send_idx + 1;
        }
        else
        {
            if (send_idx - frame_start_idx == 0xFD)
            {
                uint8_t frame_len = send_idx - frame_start_idx + 1;
                final_zero = 0;
                _streamWrite(&COBS_MAX_FRAME_LEN, 1);
                _streamWrite(&data[frame_start_idx], frame_len);
                frame_start_idx = send_idx + 1;
            }
        }
        send_idx++;
    }

    if (send_idx != frame_start_idx || final_zero)
    {
        uint8_t frame_len = send_idx - frame_start_idx + 1;
        _streamWrite(&frame_len, 1);
        _streamWrite(&data[frame_start_idx], frame_len - 1);
    }
}
