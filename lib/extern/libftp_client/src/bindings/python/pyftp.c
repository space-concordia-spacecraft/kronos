/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <Python.h>
#include <gs/ftp/client.h>

#if PY_MAJOR_VERSION == 3
#define IS_PY3
#endif

uint32_t ftp_timeout = 30000;

typedef struct {
    uint32_t size;
    uint32_t checksum;
}file_info_t;

static void size_and_checksum_cb(const gs_ftp_info_t * info)
{
    file_info_t *user_info = info->user_data;
    if (info->type == GS_FTP_INFO_FILE) {
        user_info->size     = info->u.file.size;
        user_info->checksum = info->u.file.crc;
    }
}

/*
  int ftp_download(uint8_t host, uint32_T chunk_size, const char * local_url, const char *remote_url)
 */
static PyObject* pyftp_download(PyObject* self, PyObject* args)
{
    uint8_t host;
    const char *local_url, *remote_url;
    int chunk_size;
    if (!PyArg_ParseTuple(args,
                          "biss",
                          &host,
                          &chunk_size,
                          &local_url,
                          &remote_url))
    {
        return NULL;
    }
    gs_ftp_settings_t settings = {
        .host = host,
        .timeout = ftp_timeout,
        .chunk_size = chunk_size
    };
    file_info_t fi;
    int rc = gs_ftp_download(&settings, local_url, remote_url, size_and_checksum_cb, &fi);
    return Py_BuildValue("iI", rc, fi.size);
}

/*
  int ftp_upload(uint8_t host, uint32_T chunk_size, const char * local_url, const char *remote_url)
*/
static PyObject* pyftp_upload(PyObject* self, PyObject* args)
{
    uint8_t host;
    const char *local_url, *remote_url;
    int chunk_size;
    if (!PyArg_ParseTuple(args,
                          "biss",
                          &host,
                          &chunk_size,
                          &local_url,
                          &remote_url))
    {
        return NULL;
    }

    gs_ftp_settings_t settings = {
        .host = host,
        .timeout = ftp_timeout,
        .chunk_size = chunk_size
    };

    file_info_t fi;
    int rc = gs_ftp_upload(&settings, local_url, remote_url, size_and_checksum_cb, &fi);
    return Py_BuildValue("iII", rc, fi.size, fi.checksum);
}

/* int ftp_remove(uint8_t host, char * remote_file) */
static PyObject* pyftp_remove(PyObject* self, PyObject* args)
{
    uint8_t host;
    char *remote_file;
    if (!PyArg_ParseTuple(args,
                          "bs",
                          &host,
                          &remote_file))
    {
        return NULL;
    }
    gs_ftp_settings_t settings = {
        .host = host,
        .timeout = ftp_timeout,
    };

    return Py_BuildValue("i", gs_ftp_remove(&settings, remote_file));
}

/* int ftp_mkdir(uint8_t host, char *remote_path, uint32_t mode) */
static PyObject* pyftp_mkdir(PyObject* self, PyObject* args)
{
    uint8_t host;
    char *remote_path;
    uint32_t mode;
    if (!PyArg_ParseTuple(args,
                          "bsI",
                          &host,
                          &remote_path,
                          &mode))
    {
        return NULL;
    }
    gs_ftp_settings_t settings = {
        .host = host,
        .timeout = ftp_timeout,
    };

    return Py_BuildValue("i", gs_ftp_mkdir(&settings, remote_path, mode));
}

/* int ftp_rmdir(uint8_t host, char *remote_path) */
static PyObject* pyftp_rmdir(PyObject* self, PyObject* args)
{
    uint8_t host;
    char *remote_path;
    if (!PyArg_ParseTuple(args,
                          "bs",
                          &host,
                          &remote_path))
    {
        return NULL;
    }
    gs_ftp_settings_t settings = {
        .host = host,
        .timeout = ftp_timeout,
    };

    return Py_BuildValue("i", gs_ftp_rmdir(&settings, remote_path));
}

/**
 * Helpers - ftp list
 */

typedef struct ftp_list_entry_item {
    gs_ftp_list_entry_t entry;
    struct ftp_list_entry_item *next;
} ftp_list_entry_item_t;

static ftp_list_entry_item_t* malloc_ftp_list_entry_item(void)
{
    ftp_list_entry_item_t *item = (ftp_list_entry_item_t *)malloc(sizeof(ftp_list_entry_item_t));
    item->next = NULL;
    return item;
}

static ftp_list_entry_item_t* free_ftp_list_entry_item(ftp_list_entry_item_t *current)
{
    ftp_list_entry_item_t *item = current->next;
    free(current);
    return item;
}

static int ftp_list_callback(uint16_t entries, const gs_ftp_list_entry_t *entry, void *data)
{
    ftp_list_entry_item_t *current = (ftp_list_entry_item_t*)data;

    while (current->next != NULL)
    {
        current = current->next;
    }

    current->entry.entry = entry->entry;
    current->entry.size = entry->size;
    current->entry.type = entry->type;
    strcpy(current->entry.path, entry->path);

    current->next = malloc_ftp_list_entry_item();

    return 0;
}

/* int ftp_list(uint8_t host, char * remote_path,
 *              ftp_list_callback_t callback, void *data) */
static PyObject* pyftp_list(PyObject* self, PyObject* args)
{
    uint8_t host;
    char *remote_path;
    if (!PyArg_ParseTuple(args,
                          "bs",
                          &host,
                          &remote_path))
    {
        return NULL;
    }

    ftp_list_entry_item_t *current = malloc_ftp_list_entry_item();

    gs_ftp_settings_t settings = {
        .host = host,
        .timeout = ftp_timeout,
    };

    int rc = gs_ftp_list(&settings, remote_path, ftp_list_callback, current);

    PyObject *pyList_Entries = (PyObject*) Py_BuildValue("[]");
    while (current != NULL)
    {
        if (current->next != NULL)
        {
            PyList_Append(pyList_Entries, PyList_AsTuple(Py_BuildValue("[s,I,B]",
                                                                       current->entry.path,
                                                                       current->entry.size,
                                                                       current->entry.type)));
        }
        current = free_ftp_list_entry_item(current);
    }

    PyObject *pyList = (PyObject*) Py_BuildValue("[]");
    PyList_Append(pyList, Py_BuildValue("i", rc));
    PyList_Append(pyList, pyList_Entries);
    return PyList_AsTuple(pyList);
}


// int ftp_zip(uint8_t host, const char * src, const char * dest, uint8_t action)
static PyObject* pyftp_zip(PyObject* self, PyObject* args)
{
    uint8_t host;
    const char *src, *dest;
    if (!PyArg_ParseTuple(args,
                          "bss",
                          &host,
                          &src,
                          &dest))
    {
        return NULL;
    }

    gs_ftp_settings_t settings = {
        .host = host,
        .timeout = ftp_timeout,
    };
    int rc = gs_ftp_zip(&settings, src, dest, GS_FTP_ZIP, NULL, NULL);

    return Py_BuildValue("i", rc);
}

// int ftp_zip(uint8_t host, const char * src, const char * dest, uint8_t action)
static PyObject* pyftp_unzip(PyObject* self, PyObject* args)
{
    uint8_t host;
    const char *src, *dest;
    if (!PyArg_ParseTuple(args,
                          "bss",
                          &host,
                          &src,
                          &dest))
    {
        return NULL;
    }

    gs_ftp_settings_t settings = {
        .host = host,
        .timeout = ftp_timeout,
    };
    int rc = gs_ftp_zip(&settings, src, dest, GS_FTP_UNZIP, NULL, NULL);

    return Py_BuildValue("i", rc);
}

static PyMethodDef methods[] = {
    {"ftp_download", pyftp_download, METH_VARARGS, ""},
    {"ftp_upload", pyftp_upload, METH_VARARGS, ""},
    {"ftp_remove", pyftp_remove, METH_VARARGS, ""},
    {"ftp_mkdir", pyftp_mkdir, METH_VARARGS, ""},
    {"ftp_rmdir", pyftp_rmdir, METH_VARARGS, ""},

    /**
     * Helpers - ftp list
     */

    {"ftp_list", pyftp_list, METH_VARARGS, ""},

    {"ftp_download", pyftp_download, METH_VARARGS, ""},
    {"ftp_upload", pyftp_upload, METH_VARARGS, ""},
    {"ftp_zip", pyftp_zip, METH_VARARGS, ""},
    {"ftp_unzip", pyftp_unzip, METH_VARARGS, ""},

    /* sentinel */
    {NULL, NULL, 0, NULL}
};

#ifdef IS_PY3
static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "libgsftp_client_py3",
    NULL, /* module documentation, may be NULL */
    -1,   /* size of per-interpreter state of the module,
             or -1 if the module keeps state in global variables. */
    methods,
    NULL,
    NULL,
    NULL,
    NULL
};
#endif

#ifdef IS_PY3
PyMODINIT_FUNC PyInit_libgsftp_client_py3(void) {
#else
    PyMODINIT_FUNC initlibgsftp_client_py2(void) {
#endif

#ifdef IS_PY3
        PyObject* m = PyModule_Create(&moduledef);
#else
        Py_InitModule("libgsftp_client_py2", methods);
#endif

#ifdef IS_PY3
        return m;
#endif
    }
