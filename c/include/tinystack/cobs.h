/*****************************************************************************
 *
 * cobs.h
 *
 * Consistent Overhead Byte Stuffing
 *
 ****************************************************************************/

#ifndef COBS_H_
#define COBS_H_

#include <stdint.h>
#include <stdlib.h>

//------------------------------------------------------------------------
// data types

typedef enum
{
    COBS_ENCODE_OK                  = 0x00,
    COBS_ENCODE_NULL_POINTER        = 0x01,
    COBS_ENCODE_OUT_BUFFER_OVERFLOW = 0x02
} cobs_encode_status;

typedef struct
{
    size_t              out_len;
    cobs_encode_status  status;
} cobs_encode_result;

typedef enum
{
    COBS_DECODE_OK                  = 0x00,
    COBS_DECODE_NULL_POINTER        = 0x01,
    COBS_DECODE_OUT_BUFFER_OVERFLOW = 0x02,
    COBS_DECODE_ZERO_BYTE_IN_INPUT  = 0x04,
    COBS_DECODE_INPUT_TOO_SHORT     = 0x08
} cobs_decode_status;

typedef struct
{
    size_t              out_len;
    cobs_decode_status  status;
} cobs_decode_result;

typedef size_t (*WriteToStreamFunc)(const uint8_t* data, size_t len);

//------------------------------------------------------------------------
// public functions

#ifdef __cplusplus
extern "C" {
#endif


/// Copy src buffer to destination buffer with COBS encoding
cobs_encode_result cobs_encode(uint8_t*         dst_buf_ptr,
                               size_t           dst_buf_len,
                               const uint8_t*   src_ptr,
                               size_t           src_len);


/// Decode COBS-encoded src buffer into destination buffer
cobs_decode_result cobs_decode(uint8_t*         dst_buf_ptr,
                               size_t           dst_buf_len,
                               const uint8_t*   src_ptr,
                               size_t           src_len);

/// Set the function that the COBS stream functions will use
/// to output data
void Cobs_vSetStreamWriter(WriteToStreamFunc);

/// Blocking send data with COBS encoding. Blocks until all
/// data has been sent using TINYSTACK_PUTCHAR()
void Cobs_vSendBlocking(const uint8_t* const data, const size_t len);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* COBS_H_ */
