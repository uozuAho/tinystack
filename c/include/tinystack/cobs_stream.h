#ifndef COBS_STREAM_H
#define COBS_STREAM_H

#include <stdint.h>

/// Blocking send data with COBS encoding.
/// Blocks on TINYSTACK_PUTCHAR()
void
Cobs_vSendBlocking(uint8_t* data, size_t len);

#endif // COBS_STREAM_H