#include <stdint.h>
#include <string.h>

#include "cobs.h"


//---------------------------------------------------------------
// Test data

static const uint8_t test_string_0[] = { 0, 0, 0, 0 };

static const uint8_t test_string_1[] =
{
    '1', '2', '3', '4', '5'
};

static const uint8_t test_string_1_expected_encoding[] =
{
    0x6, '1', '2', '3', '4', '5'
};

static const uint8_t test_string_2[] =
{
    '1', '2', '3', '4', '5', 0
};

static const uint8_t test_string_2_expected_encoding[] =
{
    0x6, '1', '2', '3', '4', '5', 0x1
};

static const uint8_t test_string_3[] =
{
    '1', '2', '3', '4', '5', 0, '6', '7', '8', '9',
};

static const uint8_t test_string_3_expected_encoding[] =
{
    0x6, '1', '2', '3', '4', '5', 0x5, '6', '7', '8', '9',
};

static const uint8_t test_string_4[] =
{
    '1', '2', '3', '4', '5', 0, '6', '7', '8', '9', 0
};

static const uint8_t test_string_4_expected_encoding[] =
{
    0x6, '1', '2', '3', '4', '5', 0x5, '6', '7', '8', '9', 0x1
};

static const uint8_t test_string_5[] =
{
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
    23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
    42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
    61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
    99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
    114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128,
    129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
    159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173,
    174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188,
    189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203,
    204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218,
    219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233,
    234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248,
    249, 250, 251, 252, 253,
};

static const uint8_t test_string_5_expected_encoding[] =
{
    0xfe, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
    23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
    42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
    61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
    99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
    114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128,
    129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
    144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
    159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173,
    174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188,
    189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203,
    204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218,
    219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233,
    234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248,
    249, 250, 251, 252, 253,
};

static const uint8_t test_string_6[] =
{
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
    79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
    98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
    113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
    128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
    143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157,
    158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172,
    173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
    188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202,
    203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217,
    218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232,
    233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247,
    248, 249, 250, 251, 252, 253, 254
};

static const uint8_t test_string_6_expected_encoding[] =
{
    0xff, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
    79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
    98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
    113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
    128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
    143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157,
    158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172,
    173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187,
    188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202,
    203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217,
    218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232,
    233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247,
    248, 249, 250, 251, 252, 253, 254
};

static const uint8_t test_string_7[] =
{
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
    79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96,
    97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
    127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140,
    141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154,
    155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169,
    170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184,
    185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
    200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
    215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229,
    230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244,
    245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
};

static const uint8_t test_string_7_expected_encoding[] =
{
    0xff, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
    79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96,
    97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
    127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140,
    141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154,
    155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169,
    170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184,
    185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
    200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
    215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229,
    230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244,
    245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 0x2, 255
};


//---------------------------------------------------------------
// Tests

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(CobsTests);

uint8_t             out_buffer[1000];

TEST_SETUP(CobsTests)
{
    memset(out_buffer, 'A', sizeof(out_buffer));
}

TEST_TEAR_DOWN(CobsTests)
{
}

TEST(CobsTests, encode_null_ptr)
{
    uint8_t *null_ptr = NULL;
    cobs_encode_result result = cobs_encode(out_buffer, sizeof(out_buffer),
                                            null_ptr, 0);
    TEST_ASSERT_EQUAL(0, result.out_len);
    TEST_ASSERT_EQUAL(COBS_ENCODE_NULL_POINTER, result.status);
}

TEST(CobsTests, encode_out_buf_overflow)
{
    uint8_t temp_out_buf[5];
    uint8_t temp_test_str[] = {'1', '2', '3', '4', '5'};

    cobs_encode_result result =
        cobs_encode(temp_out_buf, sizeof(temp_out_buf), temp_test_str, 4);

    TEST_ASSERT_EQUAL(COBS_ENCODE_OK, result.status);

    result =
        cobs_encode(temp_out_buf, sizeof(temp_out_buf), temp_test_str, 5);

    TEST_ASSERT_EQUAL(COBS_ENCODE_OUT_BUFFER_OVERFLOW, result.status);
}

TEST(CobsTests, encode_zero_len)
{
    cobs_encode_result result = cobs_encode(out_buffer, sizeof(out_buffer),
                                            test_string_0, 0);
    const char expected_encoding[] = {0x1};

    TEST_ASSERT_EQUAL(1, result.out_len);
    TEST_ASSERT_EQUAL(COBS_ENCODE_OK, result.status);
    TEST_ASSERT_EQUAL_MEMORY(expected_encoding, out_buffer, 1);
}

TEST(CobsTests, encode_1)
{
    cobs_encode_result result = cobs_encode(out_buffer, sizeof(out_buffer),
                                            test_string_0, 1);
    const char expected_encoding[] = {0x1, 0x1};

    TEST_ASSERT_EQUAL(2, result.out_len);
    TEST_ASSERT_EQUAL(COBS_ENCODE_OK, result.status);
    TEST_ASSERT_EQUAL_MEMORY(expected_encoding, out_buffer, 2);
}

TEST(CobsTests, encode_2)
{
    cobs_encode_result result = cobs_encode(out_buffer, sizeof(out_buffer),
                                            test_string_0, 2);
    const char expected_encoding[] = {0x1, 0x1, 0x1};

    TEST_ASSERT_EQUAL(3, result.out_len);
    TEST_ASSERT_EQUAL(COBS_ENCODE_OK, result.status);
    TEST_ASSERT_EQUAL_MEMORY(expected_encoding, out_buffer, 3);
}

TEST(CobsTests, encode_3)
{
    cobs_encode_result result = cobs_encode(out_buffer, sizeof(out_buffer),
                                            test_string_0, 3);
    const char expected_encoding[] = {0x1, 0x1, 0x1, 0x1};

    TEST_ASSERT_EQUAL(4, result.out_len);
    TEST_ASSERT_EQUAL(COBS_ENCODE_OK, result.status);
    TEST_ASSERT_EQUAL_MEMORY(expected_encoding, out_buffer, 4);
}

TEST(CobsTests, encode_4)
{
    cobs_encode_result result = cobs_encode(out_buffer, sizeof(out_buffer),
                                            test_string_0, 4);
    const char expected_encoding[] = {0x1, 0x1, 0x1, 0x1, 0x1};

    TEST_ASSERT_EQUAL(5, result.out_len);
    TEST_ASSERT_EQUAL(COBS_ENCODE_OK, result.status);
    TEST_ASSERT_EQUAL_MEMORY(expected_encoding, out_buffer, 5);
}

TEST(CobsTests, encode_test_string_1)
{
    cobs_encode_result result = cobs_encode(out_buffer, sizeof(out_buffer),
                                            test_string_1, sizeof(test_string_1));

    TEST_ASSERT_EQUAL(sizeof(test_string_1) + 1, result.out_len);
    TEST_ASSERT_EQUAL(COBS_ENCODE_OK, result.status);
    TEST_ASSERT_EQUAL_MEMORY(test_string_1_expected_encoding, out_buffer,
        sizeof(test_string_1) + 1);
}

TEST(CobsTests, encode_test_string_2)
{
    cobs_encode_result result = cobs_encode(out_buffer, sizeof(out_buffer),
                                            test_string_2, sizeof(test_string_2));

    TEST_ASSERT_EQUAL(sizeof(test_string_2) + 1, result.out_len);
    TEST_ASSERT_EQUAL(COBS_ENCODE_OK, result.status);
    TEST_ASSERT_EQUAL_MEMORY(test_string_2_expected_encoding, out_buffer,
        sizeof(test_string_2) + 1);
}

TEST(CobsTests, encode_test_string_3)
{
    cobs_encode_result result = cobs_encode(out_buffer, sizeof(out_buffer),
                                            test_string_3, sizeof(test_string_3));

    TEST_ASSERT_EQUAL(sizeof(test_string_3) + 1, result.out_len);
    TEST_ASSERT_EQUAL(COBS_ENCODE_OK, result.status);
    TEST_ASSERT_EQUAL_MEMORY(test_string_3_expected_encoding, out_buffer,
        sizeof(test_string_3) + 1);
}

TEST(CobsTests, encode_test_string_4)
{
    cobs_encode_result result = cobs_encode(out_buffer, sizeof(out_buffer),
                                            test_string_4, sizeof(test_string_4));

    TEST_ASSERT_EQUAL(sizeof(test_string_4) + 1, result.out_len);
    TEST_ASSERT_EQUAL(COBS_ENCODE_OK, result.status);
    TEST_ASSERT_EQUAL_MEMORY(test_string_4_expected_encoding, out_buffer,
        sizeof(test_string_4) + 1);
}

TEST(CobsTests, encode_test_string_5)
{
    cobs_encode_result result = cobs_encode(out_buffer, sizeof(out_buffer),
                                            test_string_5, sizeof(test_string_5));

    TEST_ASSERT_EQUAL(sizeof(test_string_5) + 1, result.out_len);
    TEST_ASSERT_EQUAL(COBS_ENCODE_OK, result.status);
    TEST_ASSERT_EQUAL_MEMORY(test_string_5_expected_encoding, out_buffer,
        sizeof(test_string_5) + 1);
}

TEST(CobsTests, encode_test_string_6)
{
    cobs_encode_result result = cobs_encode(out_buffer, sizeof(out_buffer),
                                            test_string_6, sizeof(test_string_6));

    TEST_ASSERT_EQUAL(sizeof(test_string_6) + 1, result.out_len);
    TEST_ASSERT_EQUAL(COBS_ENCODE_OK, result.status);
    TEST_ASSERT_EQUAL_MEMORY(test_string_6_expected_encoding, out_buffer,
        sizeof(test_string_6) + 1);
}

TEST(CobsTests, encode_test_string_7)
{
    cobs_encode_result result = cobs_encode(out_buffer, sizeof(out_buffer),
                                            test_string_7, sizeof(test_string_7));

    TEST_ASSERT_EQUAL(sizeof(test_string_7) + 2, result.out_len);
    TEST_ASSERT_EQUAL(COBS_ENCODE_OK, result.status);
    TEST_ASSERT_EQUAL_MEMORY(test_string_7_expected_encoding, out_buffer,
        sizeof(test_string_7) + 2);
}
