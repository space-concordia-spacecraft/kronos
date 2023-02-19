/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include "gs/util/zip/zip.h"
#include "miniz/miniz.h"

#include <gs/util/minmax.h>
#include <gs/util/log.h>
#include <gs/util/error.h>

#include <limits.h>

static void cleanup(FILE *pInfile, FILE *pOutfile, uint8_t *stream_inbuf, uint8_t *stream_outbuf)
{
    if(pInfile)
        fclose(pInfile);

    if(pOutfile)
        fclose(pOutfile);

    if(stream_inbuf)
        free(stream_inbuf);

    if(stream_outbuf)
        free(stream_outbuf);
}

int gs_zip_compress_file(const char *src, const char *dest)
{
    FILE *pInfile, *pOutfile;
    uint32_t infile_size;
    long file_loc;

    // Open input file.
    pInfile = fopen(src, "rb");
    if (!pInfile)
    {
        log_error("Zip compress: Failed opening input file!");
        return GS_ERROR_IO;
    }

    // Determine input file's size.
    fseek(pInfile, 0, SEEK_END);
    file_loc = ftell(pInfile);
    fseek(pInfile, 0, SEEK_SET);

    if((file_loc < 0) || ((unsigned long)file_loc > UINT_MAX))
    {
        log_error("Zip compress: File is too large to be processed.");
        fclose(pInfile);

        return GS_ERROR_IO;
    }

    infile_size = (uint32_t)file_loc;
    uint32_t buffer_size = infile_size;

    // Allocate input buffer memory
    uint8_t *stream_inbuf = malloc(buffer_size);
    if (stream_inbuf == NULL)
    {
        log_error("Zip compress: Failed to allocate input buffer memory");        
        fclose(pInfile);

        return GS_ERROR_IO;
    }

    // Allocate output buffer memory
    uint8_t *stream_outbuf = malloc(buffer_size);
    if (stream_outbuf == NULL)
    {
        log_error("Zip compress: Failed to allocate output buffer memory");
        cleanup(pInfile, NULL, stream_inbuf, NULL);

        return GS_ERROR_IO;
    }

    // Open output file.
    pOutfile = fopen(dest, "wb");
    if (!pOutfile)
    {
        log_error("Zip compress: Failed opening output file!");
        cleanup(pInfile, NULL, stream_inbuf, stream_outbuf);

        return GS_ERROR_IO;
    }

    // Init the z_stream
    z_stream stream;
    memset(&stream, 0, sizeof(stream));
    stream.next_in = stream_inbuf;
    stream.avail_in = 0;
    stream.next_out = stream_outbuf;
    stream.avail_out = buffer_size;

    // Compression.
    uint32_t infile_remaining = infile_size;

    if(deflateInit(&stream, Z_BEST_COMPRESSION) != Z_OK)
    {
        log_error("Zip compress: deflateInit() failed!");
        cleanup(pInfile, pOutfile, stream_inbuf, stream_outbuf);

        return GS_ERROR_DATA;
    }

    for( ; ; )
    {
        int status;
        if(!stream.avail_in)
        {
            // Input buffer is empty, so read more bytes from input file.
            uint32_t n = gs_min((uint32_t)buffer_size, infile_remaining);

            if (fread(stream_inbuf, 1, n, pInfile) != n)
            {
                log_error("Zip compress: Failed reading from input file!");
                cleanup(pInfile, pOutfile, stream_inbuf, stream_outbuf);

                return GS_ERROR_IO;
            }

            stream.next_in = stream_inbuf;
            stream.avail_in = n;

            infile_remaining -= n;
        }

        status = deflate(&stream, infile_remaining ? Z_NO_FLUSH : Z_FINISH);

        if((status == Z_STREAM_END) || (!stream.avail_out))
        {
            // Output buffer is full, or compression is done, so write buffer to output file.
            uint32_t n = buffer_size - stream.avail_out;
            if (fwrite(stream_outbuf, 1, n, pOutfile) != n)
            {
                log_error("Zip compress: Failed writing to output file!");
                cleanup(pInfile, pOutfile, stream_inbuf, stream_outbuf);

                return GS_ERROR_IO;
            }

            stream.next_out = stream_outbuf;
            stream.avail_out = buffer_size;
        }

        if(status == Z_STREAM_END)
        {
            break;
        }
        else if(status != Z_OK)
        {
            log_error("Zip compress: deflate() failed with status %i!", status);
            cleanup(pInfile, pOutfile, stream_inbuf, stream_outbuf);

            return GS_ERROR_DATA;
        }
    }

    if(deflateEnd(&stream) != Z_OK)
    {
        log_error("Zip compress: deflateEnd() failed!");
        cleanup(pInfile, pOutfile, stream_inbuf, stream_outbuf);

        return GS_ERROR_DATA;
    }

    
    cleanup(pInfile, NULL, stream_inbuf, stream_outbuf);
    if(EOF == fclose(pOutfile))
    {
        log_error("Zip compress: Failed writing to output file!");
        return GS_ERROR_IO;
    }

    log_debug("Total input bytes: %u\n", (mz_uint32)stream.total_in);
    log_debug("Total output bytes: %u\n", (mz_uint32)stream.total_out);
    log_debug("Success.\n");

    return GS_OK;
}

int gs_zip_decompress_file(const char *src, const char *dest)
{
    FILE *pInfile, *pOutfile;
    uint32_t infile_size;
    long file_loc;

    // Open input file.
    pInfile = fopen(src, "rb");
    if (!pInfile)
    {
        log_error("Zip decompress: Failed opening input file!");
        return GS_ERROR_IO;
    }

    // Determine input file's size.
    fseek(pInfile, 0, SEEK_END);
    file_loc = ftell(pInfile);
    fseek(pInfile, 0, SEEK_SET);

    if((file_loc < 0) || ((unsigned long)file_loc > UINT_MAX))
    {
        log_error("Zip decompress: File is too large to be processed.");
        fclose(pInfile);

        return GS_ERROR_IO;
    }

    infile_size = (uint32_t)file_loc;
    uint32_t buffer_size = infile_size;

    // Allocate input buffer memory
    uint8_t *stream_inbuf = malloc(buffer_size);
    if (stream_inbuf == NULL)
    {
        log_error("Zip decompress: Failed to allocate input buffer memory");        
        fclose(pInfile);

        return GS_ERROR_IO;
    }

    // Allocate output buffer memory
    uint8_t *stream_outbuf = malloc(buffer_size);
    if (stream_outbuf == NULL)
    {
        log_error("Zip decompress: Failed to allocate output buffer memory");
        cleanup(pInfile, NULL, stream_inbuf, NULL);

        return GS_ERROR_IO;
    }

    // Open output file.
    pOutfile = fopen(dest, "wb");
    if (!pOutfile)
    {
        log_error("Zip decompress: Failed opening output file!");
        cleanup(pInfile, NULL, stream_inbuf, stream_outbuf);

        return GS_ERROR_IO;
    }

    // Init the z_stream
    z_stream stream;
    memset(&stream, 0, sizeof(stream));
    stream.next_in = stream_inbuf;
    stream.avail_in = 0;
    stream.next_out = stream_outbuf;
    stream.avail_out = buffer_size;

    // Decompression.
    uint32_t infile_remaining = infile_size;

    if(inflateInit(&stream))
    {
        log_error("Zip decompress: inflateInit() failed!");
        cleanup(pInfile, pOutfile, stream_inbuf, stream_outbuf);

        return GS_ERROR_DATA;
    }

    for( ; ; )
    {
        int status;
        if(!stream.avail_in)
        {
            // Input buffer is empty, so read more bytes from input file.
            uint32_t n = gs_min((uint32_t)buffer_size, infile_remaining);

            if(fread(stream_inbuf, 1, n, pInfile) != n)
            {
                log_error("Zip decompress: Failed reading from input file!");
                cleanup(pInfile, pOutfile, stream_inbuf, stream_outbuf);

                return GS_ERROR_IO;
            }

            stream.next_in = stream_inbuf;
            stream.avail_in = n;

            infile_remaining -= n;
        }

        status = inflate(&stream, Z_SYNC_FLUSH);

        if((status == Z_STREAM_END) || (!stream.avail_out))
        {
            // Output buffer is full, or decompression is done, so write buffer to output file.
            uint32_t n = buffer_size - stream.avail_out;
            if(fwrite(stream_outbuf, 1, n, pOutfile) != n)
            {
                log_error("Zip decompress: Failed writing to output file!");
                cleanup(pInfile, pOutfile, stream_inbuf, stream_outbuf);

                return GS_ERROR_IO;
            }
            stream.next_out = stream_outbuf;
            stream.avail_out = buffer_size;
        }

        if(status == Z_STREAM_END)
        {
            break;
        }
        else if(status != Z_OK)
        {
            log_error("Zip decompress: inflate() failed with status %i!", status);
            cleanup(pInfile, pOutfile, stream_inbuf, stream_outbuf);

            return GS_ERROR_DATA;
        }
    }

    if(inflateEnd(&stream) != Z_OK)
    {
        log_error("Zip decompress: inflateEnd() failed!");
        cleanup(pInfile, pOutfile, stream_inbuf, stream_outbuf);

        return GS_ERROR_DATA;
    }

    cleanup(pInfile, NULL, stream_inbuf, stream_outbuf);
    if(EOF == fclose(pOutfile))
    {
        log_error("Zip decompress: Failed writing to output file!");
        return GS_ERROR_IO;
    }

    log_debug("Total input bytes: %u", (mz_uint32)stream.total_in);
    log_debug("Total output bytes: %u", (mz_uint32)stream.total_out);
    log_debug("Success.\n");

    return GS_OK;
}

int gs_zip_compress_data(const unsigned char *src, uint32_t src_len, unsigned char *dest, uint32_t *dest_len)
{
    mz_ulong cmp_len = src_len;
    if(compress(dest, &cmp_len, src, (mz_ulong)src_len) != MZ_OK)
    {
        return GS_ERROR_DATA;
    }

    *dest_len = cmp_len;

    return GS_OK;
}

int gs_zip_decompress_data(const unsigned char *src, uint32_t src_len, unsigned char *dest, uint32_t dest_len, uint32_t *decomp_len)
{
    mz_ulong tmp = dest_len;
    if(uncompress(dest, &tmp, src, (mz_ulong)src_len) != MZ_OK)
        return GS_ERROR_DATA;

    *decomp_len = tmp;

    return GS_OK;
}
