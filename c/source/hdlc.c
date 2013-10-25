/** Only the byte-stuffing aspect of hdlc is required.
    See http://en.wikipedia.org/wiki/High-Level_Data_Link_Control,
    "Asynchronous framing".
*/

#include <stdint.h>

#define TINYSTACK_SEND_BYTE(x)        putchar(x)

//---------------------------------------------------------------
// constants

const uint8_t FRAME_DELIMITER = 0x7e;
const uint8_t ESCAPE_BYTE = 0x7d;
const uint8_t POST_ESCAPE_BYTE_MASK = 0x20;


//---------------------------------------------------------------
//

size_t Hdlc_uiSend_blocking(const uint8_t* data, size_t len)
{
    size_t bytes_sent = 0;

    while(bytes_sent != len)
    {
        if (data[bytes_sent] == FRAME_DELIMITER ||
            data[bytes_sent] == ESCAPE_BYTE)
        {
            while (TINYSTACK_SEND_BYTE(ESCAPE_BYTE) < 0) {}
            while (TINYSTACK_SEND_BYTE(data[bytes_sent] ^ POST_ESCAPE_BYTE_MASK) < 0) {}
            bytes_sent++;
        }
        else if (TINYSTACK_SEND_BYTE(data[bytes_sent]) >= 0)
            bytes_sent++;
    }
}

/// Send a byte with stuffing. Returns the byte on success,
/// -1 on error. Can block on TINYSTACK_SEND_BYTE if a stuffing
/// byte is required & TINYSTACK_SEND_BYTE fails
int Hdlc_siSendByte_blocking(uint8_t ch)
{
    int ret = -1;

    if (ch == FRAME_DELIMITER || ch == ESCAPE_BYTE)
    {
        // block until the escaped bytes are sent
        for(;;)
        {
            if (TINYSTACK_SEND_BYTE(ESCAPE_BYTE) >= 0)
                break;
        }
        for(;;)
        {
            if (TINYSTACK_SEND_BYTE(ch ^ POST_ESCAPE_BYTE_MASK) >= 0)
            {
                ret = ch;
                break;
            }
        }
    }
    else
        ret = TINYSTACK_SEND_BYTE(ch);

    return ret;
}
