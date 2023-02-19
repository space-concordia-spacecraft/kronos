#ifndef LIBUTIL_ZIP_ZIP_UTILS_H
#define LIBUTIL_ZIP_ZIP_UTILS_H
/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
/**
   @file

   Compress/decompress API based on zlib compressed data format specification standards.
*/

#include <gs/util/types.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
	Compress file.

	@param[in] src file to be compressed.
	@param[out] dest compressed output file.
	@return_gs_error_t
*/
int gs_zip_compress_file(const char *src, const char *dest);

/**
	Decompress file.

	@param[in] src file to be secompressed.
	@param[out] dest decompressed output file.
	@return_gs_error_t
*/
int gs_zip_decompress_file(const char *src, const char *dest);

/**
	Compress data.

	@param[in] src pointer to the data to be compressed.
	@param[in] src_len size of the data.
	@param[out] dest pointer to the compressed data.
	@param[out] dest_len pointer to the size of the compressed data.
	@return_gs_error_t
*/
int gs_zip_compress_data(const unsigned char *src, uint32_t src_len, unsigned char *dest, uint32_t *dest_len);

/**
	Decompress data.

	@param[in] src pointer to the data to be decompressed.
	@param[in] src_len size of the data.
	@param[out] dest pointer to the decompressed data.
	@param[out] dest_len size of the destination memory area.
	@param[out] decomp_len pointer to the size of the decompressed data.
	@return_gs_error_t
*/
int gs_zip_decompress_data(const unsigned char *src, uint32_t src_len, unsigned char *dest, uint32_t dest_len, uint32_t *decomp_len);


#ifdef __cplusplus
}
#endif
#endif /* LIBUTIL_ZIP_ZIP_UTILS_H */
