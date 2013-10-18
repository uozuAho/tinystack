#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "tinystack/cobs.h"


// ---------------------------------------
// Constants

const char* USAGE = "Usage: cobs_codec.exe infile outfile";

const size_t ENCODE_BUF_SIZE = 256;
const size_t DECODE_BUF_SIZE = 256;


// ---------------------------------------
// functions

void print_usage()
{
    printf("%s\n", USAGE);
}


int print_file_to_stdout(const char* path)
{
    int ret = 0;
    FILE* fp = fopen(path, "r");

    if (fp == NULL)
    {
        printf("Couldn't open file\n");
        ret = 1;
    }
    else
    {
        while (1)
        {
            int ch = 0;
            ch = fgetc(fp);
            if (ch == EOF)
                break;
            else
                printf("%c",ch);
        }
        fclose(fp);
    }

    return ret;
}


int cobs_encode_file(const char* infile, const char* outfile)
{
    int ret = 0;

    uint8_t encode_buf[ENCODE_BUF_SIZE];
    size_t buf_ptr = 0;

    FILE* infp = fopen(infile, "r");
    FILE* outfp = fopen(outfile, "w");

    if (infp == NULL || outfp == NULL)
        ret = 1;
    else
    {
        while(1)
        {
            int eof_reached = 0;
            int send = 0;

            int ch = fgetc(infp);
            if (ch == EOF)
            {
                eof_reached = 1;
                send = 1;
            }
            else
            {
                assert(buf_ptr < ENCODE_BUF_SIZE);

                encode_buf[buf_ptr++] = (uint8_t)ch;

                if (buf_ptr == ENCODE_BUF_SIZE)
                {
                    send = 1;
                }
            }

            if (send)
            {
                uint8_t outbuf[ENCODE_BUF_SIZE + 5];

                cobs_encode_result res = cobs_encode(outbuf,
                    ENCODE_BUF_SIZE + 5, encode_buf, buf_ptr);

                if (res.status == COBS_ENCODE_OK)
                {
                    fwrite(outbuf , sizeof(uint8_t), res.out_len, outfp);
                    buf_ptr = 0;
                }
                else
                {
                    ret = 1;
                    break;
                }
            }

            if (eof_reached)
                break;
        }

        fclose(infp);
        fclose(outfp);
    }

    return ret;
}


int main(int argc, char const *argv[])
{
    if (argc != 3)
        print_usage();
    else
    {
        int result = cobs_encode_file(argv[1], argv[2]);
    }

    return 0;
}
