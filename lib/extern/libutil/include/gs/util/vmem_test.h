#ifndef LIB_LIBAPP_SRC_VMEM_TEST_H_
#define LIB_LIBAPP_SRC_VMEM_TEST_H_
/* Copyright (c) 2013-2019 GomSpace A/S. All rights reserved. */
/**
   @file 

   Interface for vmem tests

   Functions to test functionality and performance of memory mapped through vmem
*/
#include <gs/util/error.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
   Test memory data bus
   @param[in] virtual_start Start address for test
   @param[in] out Pointer to output file stream, used to output test failure details
   @return_gs_error_t
*/
gs_error_t gs_vmem_test_data_bus_test(gs_address_t virtual_start, FILE * out);

/**
   Test memory address bus
   @param[in] virtual_start Start address for test
   @param[in] mem_size size of memory to be tested
   @param[in] out Pointer to output file stream, used to output test failure details
   @return_gs_error_t
*/
gs_error_t gs_vmem_test_addr_bus_test(gs_address_t virtual_start, size_t mem_size, FILE * out);

/**
   Test all memory cells
   @param[in] virtual_start Start address for test
   @param[in] mem_size size of memory to be tested
   @param[in] out Pointer to output file stream, used to output test failure details
   @return_gs_error_t
*/
gs_error_t gs_vmem_test_cell_test(gs_address_t virtual_start, size_t mem_size, FILE * out);

/**
   Test memory speed
   @param[in] virtual_start Start address for test
   @param[in] mem_size size of memory to be tested
   @param[in] out Pointer to output file stream, used to output test failure details
   @return_gs_error_t
*/
gs_error_t gs_vmem_test_speed_test(gs_address_t virtual_start, size_t mem_size, FILE * out);

/**
   Test memory lock. Will only PASS on memory that supports locking
   @param[in] virtual_start Start address for test
   @param[in] mem_size size of memory to be tested
   @param[in] vmem_partition_name vmem partiontion name to test
   @param[in] out Pointer to output file stream, used to output test failure details
   @return_gs_error_t
*/
gs_error_t gs_vmem_test_lock_test(gs_address_t virtual_start, size_t mem_size, const char * vmem_partition_name, FILE * out);

#ifdef __cplusplus
}
#endif
#endif /* LIB_LIBAPP_SRC_VMEM_TEST_H_ */
