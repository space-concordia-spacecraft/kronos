/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */
#include <Python.h>
#include <string.h>

#include <gs/param/rparam.h>
#include <param/rparam_client.h>
#include <gs/param/pp/i2c/i2c.h>
#include <gs/param/pp/spi/spi.h>

#if PY_MAJOR_VERSION == 3
#define IS_PY3
#endif

static inline int to_int(char c)
{
    if (c >= '0' && c <= '9') return      c - '0';
    if (c >= 'A' && c <= 'F') return 10 + c - 'A';
    if (c >= 'a' && c <= 'f') return 10 + c - 'a';
    return -1;
}

/* static inline int rparam_get_string(char *out, int outlen, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_get_string(PyObject *self, PyObject *args)
{
    uint16_t addr;
    uint16_t string_size;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "HHii|ii", &addr, &string_size, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }
    char buffer[string_size + 1];
    memset(&buffer, 0, string_size + 1);
    int result = rparam_get_string(&buffer[0], string_size, addr, index_id, node, port, timeout);
    return Py_BuildValue("is", result, buffer);
}

/* static inline int rparam_set_string(char *in, int inlen, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_set_string(PyObject *self, PyObject *args)
{
    char* in;
    int inlen;
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "siHii|ii", &in, &inlen, &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    /* Note: inlen + 1 adds NULL termination. Must be checked and tested when updated to gs_rparam 
    as this also adds 1 */
    return Py_BuildValue("i", rparam_set_string(in, inlen + 1, addr, index_id, node, port, timeout));
}

/* static inline int rparam_get_string(char *out, int outlen, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_get_data(PyObject *self, PyObject *args)
{
    uint16_t addr;
    uint16_t data_size;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;

    if (!PyArg_ParseTuple(args, "HHii|ii", &addr, &data_size, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }
    char buffer[data_size];
    memset(&buffer, 0, data_size);

    //Reading as raw string:
    int result = rparam_get_string(&buffer[0], data_size, addr, index_id, node, port, timeout);

    //Convert to string of HEX bytes
    const uint16_t bblen = (2 * data_size);
    char bb[bblen+1];
    memset(bb,0, sizeof(bb));

    char *bb_ptr = &bb[0];
    for (int i = 0; i < data_size; i++) {
        sprintf(bb_ptr, "%02"PRIX8, ((uint8_t *) buffer)[i]);
        bb_ptr+=2;
    }

    return Py_BuildValue("is", result, bb);
}

/* static inline int rparam_set_string(char *in, int inlen, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_set_data(PyObject *self, PyObject *args)
{
    char* in;
    int inlen;
    int maxlen;
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    gs_error_t error = GS_OK;

    if (!PyArg_ParseTuple(args, "siiHii|ii", &in, &inlen, &maxlen, &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    //inlen is number of bytes in DATA param in table, in reality the string will be twice as long, since it contains HEX-string...
    // e.g., DEADBEEF00000000 is 16 characters but only corresponds to 8 bytes
    char outbuffer[maxlen];
    memset(outbuffer, 0, sizeof(outbuffer));

    if ((inlen % 2) == 0) {
        // validate data first - not to end up with invalid/strange data
        for (int i = 0; i < inlen; ++i) {
            if (to_int(in[i]) < 0) {
                error = GS_ERROR_DATA;
                break;
            }
        }
        if (error == GS_OK) {
            uint8_t * out = (uint8_t *) outbuffer;
            for (int i = 0; i < inlen; i += 2, ++out) {
                *out = (16 * to_int(in[i])) + to_int(in[i+1]);
            }
            error = GS_OK;
        }
    } else {
        error = GS_ERROR_DATA;
    }

    if (error != GS_OK) {
        return Py_BuildValue("i", error);
    } else {
        return Py_BuildValue("i", rparam_set_string(outbuffer, maxlen, addr, index_id, node, port, timeout));
    }
}

/* static inline int rparam_get_uint8(uint8_t * out, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_get_uint8(PyObject *self, PyObject *args)
{
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "Hii|ii", &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    uint8_t value;
    int result = rparam_get_uint8(&value, addr, index_id, node, port, timeout);
    return Py_BuildValue("iB", result, value);
}

/* static inline int rparam_set_uint8(uint8_t * in, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_set_uint8(PyObject *self, PyObject *args)
{
    uint8_t value;
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "BHii|ii", &value, &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    int result = rparam_set_uint8(&value, addr, index_id, node, port, timeout);
    return Py_BuildValue("i", result);
}

/* static inline int rparam_get_uint16(uint16_t * out, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_get_uint16(PyObject *self, PyObject *args)
{
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "Hii|ii", &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    uint16_t value;
    int result = rparam_get_uint16(&value, addr, index_id, node, port, timeout);
    return Py_BuildValue("iH", result, value);
}

/* static inline int rparam_set_uint16(uint16_t * in, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_set_uint16(PyObject *self, PyObject *args)
{
    uint16_t value;
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "HHii|ii", &value, &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    int result = rparam_set_uint16(&value, addr, index_id, node, port, timeout);
    return Py_BuildValue("i", result);
}

/* static inline int rparam_get_uint32(uint32_t * out, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_get_uint32(PyObject *self, PyObject *args)
{
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "Hii|ii", &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    uint32_t value;
    int result = rparam_get_uint32(&value, addr, index_id, node, port, timeout);
    return Py_BuildValue("iI", result, value);
}

/* static inline int rparam_set_uint32(uint32_t * in, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_set_uint32(PyObject *self, PyObject *args)
{
    uint32_t value;
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "IHii|ii", &value, &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    int result = rparam_set_uint32(&value, addr, index_id, node, port, timeout);
    return Py_BuildValue("i", result);
}

/* static inline int rparam_get_uint64(uint64_t * out, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_get_uint64(PyObject *self, PyObject *args)
{
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "Hii|ii", &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    uint64_t value;
    int result = rparam_get_uint64(&value, addr, index_id, node, port, timeout);
    return Py_BuildValue("iK", result, value);
}

/* static inline int rparam_set_uint64(uint64_t * in, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_set_uint64(PyObject *self, PyObject *args)
{
    uint64_t value;
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "KHii|ii", &value, &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    int result = rparam_set_uint64(&value, addr, index_id, node, port, timeout);
    return Py_BuildValue("i", result);
}

/* static inline int rparam_get_int8(int8_t * out, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_get_int8(PyObject *self, PyObject *args)
{
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "Hii|ii", &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    int8_t value;
    int result = rparam_get_int8(&value, addr, index_id, node, port, timeout);
    return Py_BuildValue("hb", result, value);
}

/* static inline int rparam_set_int8(int8_t * in, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_set_int8(PyObject *self, PyObject *args)
{
    int8_t value;
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "BHii|ii", &value, &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    int result = rparam_set_int8(&value, addr, index_id, node, port, timeout);
    return Py_BuildValue("i", result);
}

/* static inline int rparam_get_int16(int16_t * out, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_get_int16(PyObject *self, PyObject *args)
{
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "Hii|ii", &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    int16_t value;
    int result = rparam_get_int16(&value, addr, index_id, node, port, timeout);
    return Py_BuildValue("ih", result, value);
}

/* static inline int rparam_set_int16(int16_t * in, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_set_int16(PyObject *self, PyObject *args)
{
    int16_t value;
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "hHii|ii", &value, &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    int result = rparam_set_int16(&value, addr, index_id, node, port, timeout);
    return Py_BuildValue("i", result);
}

/* static inline int rparam_get_int32(int32_t * out, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_get_int32(PyObject *self, PyObject *args)
{
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "Hii|ii", &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    int32_t value;
    int result = rparam_get_int32(&value, addr, index_id, node, port, timeout);
    return Py_BuildValue("ii", result, value);
}

/* static inline int rparam_set_int32(int32_t * in, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_set_int32(PyObject *self, PyObject *args)
{
    int32_t value;
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "lHii|ii", &value, &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    int result = rparam_set_int32(&value, addr, index_id, node, port, timeout);
    return Py_BuildValue("i", result);
}

/* static inline int rparam_get_int64(int64_t * out, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_get_int64(PyObject *self, PyObject *args)
{
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "Hii|ii", &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    int64_t value;
    int result = rparam_get_int64(&value, addr, index_id, node, port, timeout);
    return Py_BuildValue("iL", result, value);
}

/* static inline int rparam_set_int64(int64_t * in, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_set_int64(PyObject *self, PyObject *args)
{
    int64_t value;
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "LHii|ii", &value, &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    int result = rparam_set_int64(&value, addr, index_id, node, port, timeout);
    return Py_BuildValue("i", result);
}

/* static inline int rparam_get_float(float * out, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_get_float(PyObject *self, PyObject *args)
{
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "Hii|ii", &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    float value;
    int result = rparam_get_float(&value, addr, index_id, node, port, timeout);
    return Py_BuildValue("if", result, value);
}

/* static inline int rparam_set_float(float * in, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_set_float(PyObject *self, PyObject *args)
{
    float value;
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "fHii|ii", &value, &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    int result = rparam_set_float(&value, addr, index_id, node, port, timeout);
    return Py_BuildValue("i", result);
}

/* static inline int rparam_get_double(double * out, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_get_double(PyObject *self, PyObject *args)
{
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "Hii|ii", &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    double value;
    int result = rparam_get_double(&value, addr, index_id, node, port, timeout);
    return Py_BuildValue("id", result, value);
}

/* static inline int rparam_set_double(double * in, uint16_t addr, int index_id, int node, int port, int timeout) */
static PyObject* pyrparam_set_double(PyObject *self, PyObject *args)
{
    double value;
    uint16_t addr;
    int index_id;
    int node;
    int port = 7;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "dHii|ii", &value, &addr, &index_id, &node, &port, &timeout)) {
        Py_RETURN_NONE;
    }

    int result = rparam_set_double(&value, addr, index_id, node, port, timeout);
    return Py_BuildValue("i", result);
}

/*gs_error_t gs_rparam_save_to_store(uint8_t node, uint32_t timeout_ms, uint8_t table_id,
  const char * store, const char * slot)*/
static PyObject* pyrparam_save_to_store(PyObject *self, PyObject *args)
{
    uint8_t table_id;
    char* store;
    int node;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "Bsi|i", &table_id, &store, &node, &timeout)) {
        Py_RETURN_NONE;
    }

    int result = gs_rparam_save_to_store(node, timeout, table_id, store, NULL);
    return Py_BuildValue("i", result);
}
/*gs_error_t gs_rparam_load_from_store(uint8_t node, uint32_t timeout_ms, uint8_t table_id,
  const char * store, const char * slot)*/
static PyObject* pyrparam_load_from_store(PyObject *self, PyObject *args)
{
    uint8_t table_id;
    char* store;
    int node;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "Bsi|i", &table_id, &store, &node, &timeout)) {
        Py_RETURN_NONE;
    }

    int result = gs_rparam_load_from_store(node, timeout, table_id, store, NULL);
    return Py_BuildValue("i", result);
}

/*gs_error_t gs_rparam_save(uint8_t node, uint32_t timeout_ms, uint8_t from, uint8_t to) */
static PyObject* pyrparam_save(PyObject *self, PyObject *args)
{
    uint8_t table_id;
    uint8_t file_id;
    int node;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "BBi|i", &table_id, &file_id, &node, &timeout)) {
        Py_RETURN_NONE;
    }

    int result = gs_rparam_save(node, timeout, table_id, file_id);
    return Py_BuildValue("i", result);
}

/*gs_error_t gs_rparam_load(uint8_t node, uint32_t timeout_ms, uint8_t from, uint8_t to) */
static PyObject* pyrparam_load(PyObject *self, PyObject *args)
{
    uint8_t table_id;
    uint8_t file_id;
    int node;
    int timeout = 1000;
    if (!PyArg_ParseTuple(args, "BBi|i", &file_id, &table_id, &node, &timeout)) {
        Py_RETURN_NONE;
    }

    int result = gs_rparam_load(node, timeout, file_id, table_id);
    return Py_BuildValue("i", result);
}

/*int rparam_download_table_spec_from_remote_and_save_to_file2(const char* fname, uint8_t node, uint8_t port, param_index_t* index, uint16_t* checksum, uint32_t timeout);*/
static PyObject* pyrparam_download_table_spec_from_remote_and_save_to_file2(PyObject *self, PyObject *args)
{
    char* fname;
    int fname_len;
    int mem_id;
    int node;
    int port;
    uint16_t checksum;
    int timeout;
    if (!PyArg_ParseTuple(args, "s#iiiHi", &fname, &fname_len, &node, &port, &mem_id, &checksum, &timeout)) {
        Py_RETURN_NONE;
    }

    GS_PARAM_TINST_VAR(tinst);
    int result = gs_rparam_download_table_spec(tinst, fname, node, mem_id, timeout, &checksum);
    gs_param_table_free(tinst);  // free allocated resources by gs_rparam_download_table_spec()
    return Py_BuildValue("i", result);
}

/* gs_error_t gs_param_io_i2c_init(gs_param_io_t * io, uint8_t bus, uint8_t addr, bool big_endian); */

static PyObject* pyioparam_i2c_init(PyObject *self, PyObject *args)
{
    uint8_t bus;
    uint8_t addr;
    uint8_t big_endian;
    if (!PyArg_ParseTuple(args, "BBB", &bus, &addr, &big_endian)) {
        Py_RETURN_NONE;
    }
    gs_pp_t* io = malloc(sizeof(gs_pp_t));
    gs_error_t error = gs_pp_i2c_init(io, bus, addr, big_endian);
    return Py_BuildValue("iO", error, PyCapsule_New(io, "gs_pp_t", NULL));
}

static PyObject* pyioparam_io_free(PyObject *self, PyObject *args)
{
    PyObject *io_capsule;
    if (!PyArg_ParseTuple(args, "O", &io_capsule)) {
        Py_RETURN_NONE;
    }
    free(PyCapsule_GetPointer(io_capsule, "gs_pp_t"));
    Py_RETURN_NONE;
}

/*
gs_error_t gs_pp_get_int8(
        gs_pp_t * io,
        uint8_t table_id,
        uint8_t addr,
        int8_t * value,
        size_t count,
        uint32_t flags);
*/
static PyObject* pyioparam_io_get_int8(PyObject* self, PyObject* args)
{
    PyObject* io_capsule;
    uint8_t table_id;
    uint8_t addr;
    int8_t value;
    if (!PyArg_ParseTuple(args, "OBB", &io_capsule, &table_id, &addr)) {
        Py_RETURN_NONE;
    }
    gs_pp_t* tmp = PyCapsule_GetPointer(io_capsule, "gs_pp_t");
    gs_error_t error = gs_pp_get_int8(tmp, table_id, addr, &value, 1, 0);
    if (error) {
        value = 0;
    }
    return Py_BuildValue("ib", error, value);
}

/*
gs_error_t gs_pp_set_int8(
        gs_pp_t * io,
        uint8_t table_id,
        uint8_t addr,
        const int8_t * value,
        size_t count,
        uint32_t flags);
*/
static PyObject* pyioparam_io_set_int8(PyObject* self, PyObject* args)
{
    PyObject* io_capsule;
    uint8_t table_id;
    uint8_t addr;
    int8_t value;
    if (!PyArg_ParseTuple(args, "ObBB", &io_capsule, &value, &table_id, &addr)) {
        Py_RETURN_NONE;
    }
    gs_pp_t* tmp = PyCapsule_GetPointer(io_capsule, "gs_pp_t");
    gs_error_t error = gs_pp_set_int8(tmp, table_id, addr, &value, 1, 0);
    return Py_BuildValue("i", error);
}

/*
gs_error_t gs_pp_get_uint8(
        gs_pp_t * io,
        uint8_t table_id,
        uint8_t addr,
        uint8_t * value,
        size_t count,
        uint32_t flags);
*/
static PyObject* pyioparam_io_get_uint8(PyObject* self, PyObject* args)
{
    PyObject* io_capsule;
    uint8_t table_id;
    uint8_t addr;
    uint8_t value;
    if (!PyArg_ParseTuple(args, "OBB", &io_capsule, &table_id, &addr)) {
        Py_RETURN_NONE;
    }
    gs_pp_t* tmp = PyCapsule_GetPointer(io_capsule, "gs_pp_t");
    gs_error_t error = gs_pp_get_uint8(tmp, table_id, addr, &value, 1, 0);
    if (error) {
        value = 0;
    }
    return Py_BuildValue("iB", error, value);
}

/*
gs_error_t gs_pp_set_uint8(
        gs_pp_t * io,
        uint8_t table_id,
        uint8_t addr,
        const uint8_t * value,
        size_t count,
        uint32_t flags);
*/
static PyObject* pyioparam_io_set_uint8(PyObject* self, PyObject* args)
{
    PyObject* io_capsule;
    uint8_t table_id;
    uint8_t addr;
    uint8_t value;
    if (!PyArg_ParseTuple(args, "OBBB", &io_capsule, &value, &table_id, &addr)) {
        Py_RETURN_NONE;
    }
    gs_pp_t* tmp = PyCapsule_GetPointer(io_capsule, "gs_pp_t");
    gs_error_t error = gs_pp_set_uint8(tmp, table_id, addr, &value, 1, 0);
    return Py_BuildValue("i", error);
}

/*
gs_error_t gs_pp_get_int16(
        gs_pp_t * io,
        uint8_t table_id,
        uint8_t addr,
        int16_t * value,
        size_t count,
        uint32_t flags);
*/
static PyObject* pyioparam_io_get_int16(PyObject* self, PyObject* args)
{
    PyObject* io_capsule;
    uint8_t table_id;
    uint8_t addr;
    int16_t value;
    if (!PyArg_ParseTuple(args, "OBB", &io_capsule, &table_id, &addr)) {
        Py_RETURN_NONE;
    }
    gs_pp_t* tmp = PyCapsule_GetPointer(io_capsule, "gs_pp_t");
    gs_error_t error = gs_pp_get_int16(tmp, table_id, addr, &value, 1, 0);
    if (error) {
        value = 0;
    }
    return Py_BuildValue("ih", error, value);
}

/*
gs_error_t gs_pp_set_int16(
        gs_pp_t * io,
        uint8_t table_id,
        uint8_t addr,
        const int16_t * value,
        size_t count,
        uint32_t flags);
*/
static PyObject* pyioparam_io_set_int16(PyObject* self, PyObject* args)
{
    PyObject* io_capsule;
    uint8_t table_id;
    uint8_t addr;
    int16_t value;
    if (!PyArg_ParseTuple(args, "OhBB", &io_capsule, &value, &table_id, &addr)) {
        Py_RETURN_NONE;
    }
    gs_pp_t* tmp = PyCapsule_GetPointer(io_capsule, "gs_pp_t");
    gs_error_t error = gs_pp_set_int16(tmp, table_id, addr, &value, 1, 0);
    return Py_BuildValue("i", error);
}

/*
gs_error_t gs_pp_get_uint16(
    gs_pp_t * io,
    uint8_t table_id,
    uint8_t addr,
    uint16_t * value,
    size_t count,
    uint32_t flags);
*/
static PyObject* pyioparam_io_get_uint16(PyObject* self, PyObject* args)
{
    PyObject* io_capsule;
    uint8_t table_id;
    uint8_t addr;
    uint16_t value;
    if (!PyArg_ParseTuple(args, "OBB", &io_capsule, &table_id, &addr)) {
        Py_RETURN_NONE;
    }
    gs_pp_t* tmp = PyCapsule_GetPointer(io_capsule, "gs_pp_t");
    gs_error_t error = gs_pp_get_uint16(tmp, table_id, addr, &value, 1, 0);
    if (error) {
        value = 0;
    }
    return Py_BuildValue("iH", error, value);
}

/*
gs_error_t gs_pp_set_uint16(
        gs_pp_t * io,
        uint8_t table_id,
        uint8_t addr,
        const uint16_t * value,
        size_t count,
        uint32_t flags);
*/
static PyObject* pyioparam_io_set_uint16(PyObject* self, PyObject* args)
{
    PyObject* io_capsule;
    uint8_t table_id;
    uint8_t addr;
    uint16_t value;
    if (!PyArg_ParseTuple(args, "OHBB", &io_capsule, &value, &table_id, &addr)) {
        Py_RETURN_NONE;
    }
    gs_pp_t* tmp = PyCapsule_GetPointer(io_capsule, "gs_pp_t");
    gs_error_t error = gs_pp_set_uint16(tmp, table_id, addr, &value, 1, 0);
    return Py_BuildValue("i", error);
}

/*
gs_error_t gs_pp_get_int32(
        gs_pp_t * io,
        uint8_t table_id,
        uint8_t addr,
        int32_t * value,
        size_t count,
        uint32_t flags);
*/
static PyObject* pyioparam_io_get_int32(PyObject* self, PyObject* args)
{
    PyObject* io_capsule;
    uint8_t table_id;
    uint8_t addr;
    int32_t value;
    if (!PyArg_ParseTuple(args, "OBB", &io_capsule, &table_id, &addr)) {
        Py_RETURN_NONE;
    }
    gs_pp_t* tmp = PyCapsule_GetPointer(io_capsule, "gs_pp_t");
    gs_error_t error = gs_pp_get_int32(tmp, table_id, addr, &value, 1, 0);
    if (error) {
        value = 0;
    }
    return Py_BuildValue("ii", error, value);
}

/*
gs_error_t gs_pp_set_int32(
        gs_pp_t * io,
        uint8_t table_id,
        uint8_t addr,
        const int32_t * value,
        size_t count,
        uint32_t flags);
*/
static PyObject* pyioparam_io_set_int32(PyObject* self, PyObject* args)
{
    PyObject* io_capsule;
    uint8_t table_id;
    uint8_t addr;
    int32_t value;
    if (!PyArg_ParseTuple(args, "OiBB", &io_capsule, &value, &table_id, &addr)) {
        Py_RETURN_NONE;
    }
    gs_pp_t* tmp = PyCapsule_GetPointer(io_capsule, "gs_pp_t");
    gs_error_t error = gs_pp_set_int32(tmp, table_id, addr, &value, 1, 0);
    return Py_BuildValue("i", error);
}

/*
gs_error_t gs_pp_get_uint32(
        gs_pp_t * io,
        uint8_t table_id,
        uint8_t addr, 
        uint32_t * value,
        size_t count,
        uint32_t flags);
*/
static PyObject* pyioparam_io_get_uint32(PyObject* self, PyObject* args)
{
    PyObject* io_capsule;
    uint8_t table_id;
    uint8_t addr;
    uint32_t value;
    if (!PyArg_ParseTuple(args, "OBB", &io_capsule, &table_id, &addr)) {
        Py_RETURN_NONE;
    }
    gs_pp_t* tmp = PyCapsule_GetPointer(io_capsule, "gs_pp_t");
    gs_error_t error = gs_pp_get_uint32(tmp, table_id, addr, &value, 1, 0);
    if (error) {
        value = 0;
    }
    return Py_BuildValue("iI", error, value);
}
/*
gs_error_t gs_pp_set_uint32(
    gs_pp_t * io,
    uint8_t table_id,
    uint8_t addr,
    const uint32_t * value,
    size_t count,
    uint32_t flags);
*/

static PyObject* pyioparam_io_set_uint32(PyObject* self, PyObject* args)
{
    PyObject* io_capsule;
    uint8_t table_id;
    uint8_t addr;
    uint32_t value;
    if (!PyArg_ParseTuple(args, "OIBB", &io_capsule, &value, &table_id, &addr)) {
        Py_RETURN_NONE;
    }
    gs_pp_t* tmp = PyCapsule_GetPointer(io_capsule, "gs_pp_t");
    gs_error_t error = gs_pp_set_uint32(tmp, table_id, addr, &value, 1, 0);
    return Py_BuildValue("i", error);
}
/*
gs_error_t gs_pp_get_float(
        gs_pp_t * io,
        uint8_t table_id,
        uint8_t addr,
        float * value,
        size_t count,
        uint32_t flags);
*/
static PyObject* pyioparam_io_get_float(PyObject* self, PyObject* args)
{
    PyObject* io_capsule;
    uint8_t table_id;
    uint8_t addr;
    float value;
    if (!PyArg_ParseTuple(args, "OBB", &io_capsule, &table_id, &addr)) {
        Py_RETURN_NONE;
    }
    gs_pp_t* tmp = PyCapsule_GetPointer(io_capsule, "gs_pp_t");
    gs_error_t error = gs_pp_get_float(tmp, table_id, addr, &value, 1, 0);
    if (error) {
        value = 0;
    }
    return Py_BuildValue("if", error, value);
}

/*
gs_error_t gs_pp_set_float(
        gs_pp_t * io,
        uint8_t table_id,
        uint8_t addr,
        const float * value,
        size_t count,
        uint32_t flags);
*/
static PyObject* pyioparam_io_set_float(PyObject* self, PyObject* args)
{
    PyObject* io_capsule;
    uint8_t table_id;
    uint8_t addr;
    float value;
    if (!PyArg_ParseTuple(args, "OfBB", &io_capsule, &value, &table_id, &addr)) {
        Py_RETURN_NONE;
    }
    gs_pp_t* tmp = PyCapsule_GetPointer(io_capsule, "gs_pp_t");
    gs_error_t error = gs_pp_set_float(tmp, table_id, addr, &value, 1, 0);
    return Py_BuildValue("i", error);
}

static PyObject* pyioparam_spi_init(PyObject* self, PyObject* args)
{
    uint8_t addr;
    uint8_t big_endian;
    if (!PyArg_ParseTuple(args, "BB", &addr, &big_endian)) {
        Py_RETURN_NONE;
    }
    gs_pp_t* io = malloc(sizeof(gs_pp_t));
    gs_error_t error = gs_pp_spi_init(io, addr, big_endian);
    return Py_BuildValue("iO", error, PyCapsule_New(io, "gs_pp_t", NULL));
}

/**
   Get lock value

   @param[in] pp Handle for connection
   @param[in] table_id Table ID
   @param[out] value Lock state (0 = unlocked, 1 = locked)
   @param[in] flags
   @return_gs_error_t
gs_error_t gs_pp_get_table_lock(gs_pp_t * pp, uint8_t table_id, bool * value, uint32_t flags);
*/

static PyObject* pyioparam_get_table_lock(PyObject* self, PyObject* args)
{
    PyObject* io_capsule;
    uint8_t table_id;
    uint32_t flags = 0;
    if (!PyArg_ParseTuple(args, "OB|I", &io_capsule, &table_id, &flags)) {
        Py_RETURN_NONE;
    }
    bool result;
    gs_pp_t* tmp = PyCapsule_GetPointer(io_capsule, "gs_pp_t");
    gs_error_t error = gs_pp_get_table_lock(tmp, table_id, &result, flags);

    uint8_t retval = result ? 1 : 0;
    return Py_BuildValue("iB", error, retval);
}
/**
   Set lock value

   @param[in] pp Handle for connection
   @param[in] table_id Table ID
   @param[in] value Lock state (0 = unlocked, 1 = locked)
   @param[in] flags
   @return_gs_error_t

 gs_error_t gs_pp_set_table_lock(gs_pp_t * pp, uint8_t table_id, const bool * value, uint32_t flags);
 */

static PyObject* pyioparam_set_table_lock(PyObject* self, PyObject* args)
{
    PyObject* io_capsule;
    uint8_t table_id;
    uint32_t flags = 0;
    uint8_t status;
    if (!PyArg_ParseTuple(args, "OBB|I", &io_capsule, &table_id, &status, &flags)) {
        Py_RETURN_NONE;
    }
    bool setval = status ? true : false;
    gs_pp_t* tmp = PyCapsule_GetPointer(io_capsule, "gs_pp_t");
    gs_error_t error = gs_pp_set_table_lock(tmp, table_id, &setval, flags);
    return Py_BuildValue("i", error);
}
static PyMethodDef methods[] = {

    /* param/rparam_client.h */
    {"rparam_get_string", pyrparam_get_string, METH_VARARGS, ""},
    {"rparam_set_string", pyrparam_set_string, METH_VARARGS, ""},

    {"rparam_get_float", pyrparam_get_float, METH_VARARGS, ""},
    {"rparam_set_float", pyrparam_set_float, METH_VARARGS, ""},
    {"rparam_get_double", pyrparam_get_double, METH_VARARGS, ""},
    {"rparam_set_double", pyrparam_set_double, METH_VARARGS, ""},

    {"rparam_get_uint8", pyrparam_get_uint8, METH_VARARGS, ""},
    {"rparam_set_uint8", pyrparam_set_uint8, METH_VARARGS, ""},
    {"rparam_get_uint16", pyrparam_get_uint16, METH_VARARGS, ""},
    {"rparam_set_uint16", pyrparam_set_uint16, METH_VARARGS, ""},
    {"rparam_get_uint32", pyrparam_get_uint32, METH_VARARGS, ""},
    {"rparam_set_uint32", pyrparam_set_uint32, METH_VARARGS, ""},
    {"rparam_get_uint64", pyrparam_get_uint64, METH_VARARGS, ""},
    {"rparam_set_uint64", pyrparam_set_uint64, METH_VARARGS, ""},

    {"rparam_get_int8", pyrparam_get_int8, METH_VARARGS, ""},
    {"rparam_set_int8", pyrparam_set_int8, METH_VARARGS, ""},
    {"rparam_get_int16", pyrparam_get_int16, METH_VARARGS, ""},
    {"rparam_set_int16", pyrparam_set_int16, METH_VARARGS, ""},
    {"rparam_get_int32", pyrparam_get_int32, METH_VARARGS, ""},
    {"rparam_set_int32", pyrparam_set_int32, METH_VARARGS, ""},
    {"rparam_get_int64", pyrparam_get_int64, METH_VARARGS, ""},
    {"rparam_set_int64", pyrparam_set_int64, METH_VARARGS, ""},

    {"rparam_get_data", pyrparam_get_data, METH_VARARGS, ""},
    {"rparam_set_data", pyrparam_set_data, METH_VARARGS, ""},

    {"rparam_save_to_store", pyrparam_save_to_store, METH_VARARGS, "" },
    {"rparam_load_from_store", pyrparam_load_from_store, METH_VARARGS, "" },
    {"rparam_save", pyrparam_save, METH_VARARGS, "" },
    {"rparam_load", pyrparam_load, METH_VARARGS, "" },

    {"rparam_download_table_spec_from_remote_and_save_to_file2", pyrparam_download_table_spec_from_remote_and_save_to_file2, METH_VARARGS, ""},


    /* param/rparam_client.h */
    {"pp_i2c_init", pyioparam_i2c_init, METH_VARARGS, ""},
    {"pp_spi_init", pyioparam_spi_init, METH_VARARGS, ""},
    {"pp_get_int8", pyioparam_io_get_int8, METH_VARARGS, ""},
    {"pp_get_uint8", pyioparam_io_get_uint8, METH_VARARGS, ""},
    {"pp_set_int8", pyioparam_io_set_int8, METH_VARARGS, ""},
    {"pp_set_uint8", pyioparam_io_set_uint8, METH_VARARGS, ""},
    {"pp_get_int16", pyioparam_io_get_int16, METH_VARARGS, ""},
    {"pp_get_uint16", pyioparam_io_get_uint16, METH_VARARGS, ""},
    {"pp_set_int16", pyioparam_io_set_int16, METH_VARARGS, ""},
    {"pp_set_uint16", pyioparam_io_set_uint16, METH_VARARGS, ""},
    {"pp_get_int32", pyioparam_io_get_int32, METH_VARARGS, ""},
    {"pp_get_uint32", pyioparam_io_get_uint32, METH_VARARGS, ""},
    {"pp_set_int32", pyioparam_io_set_int32, METH_VARARGS, ""},
    {"pp_set_uint32", pyioparam_io_set_uint32, METH_VARARGS, ""},
    {"pp_get_float", pyioparam_io_get_float, METH_VARARGS, ""},
    {"pp_set_float", pyioparam_io_set_float, METH_VARARGS, ""},
    {"pp_io_free", pyioparam_io_free, METH_VARARGS, ""},
    {"pp_get_table_lock", pyioparam_get_table_lock, METH_VARARGS, ""},
    {"pp_set_table_lock", pyioparam_set_table_lock, METH_VARARGS, ""},

    /* sentinel */
    {NULL, NULL, 0, NULL}
};

#ifdef IS_PY3
static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "libgsparam_client_py3",
    NULL, /* module documentation, may be NULL */
    -1, /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    methods,
    NULL,
    NULL,
    NULL,
    NULL
};
#endif

#ifdef IS_PY3
PyMODINIT_FUNC PyInit_libgsparam_client_py3(void) {
#else
PyMODINIT_FUNC initlibgsparam_client_py2(void)
{
#endif

#ifdef IS_PY3
    PyObject* m = PyModule_Create(&moduledef);
#else
    Py_InitModule("libgsparam_client_py2", methods);
#endif

#ifdef IS_PY3
    return m;
#endif
}
