/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */

#include <Python.h>

#include <gs/csp/drivers/i2c/i2c.h>

static PyObject *Error = NULL;

static PyObject* PyErr_Error(const char * message, int error) {
    PyErr_Format(Error, "%s, result/error: %d", message, error); // should set error as member
    return NULL;
}

static PyObject* pygscsp_csp_i2c_init(PyObject *self, PyObject *args) {
    uint8_t device;
    uint8_t csp_addr;
    if (!PyArg_ParseTuple(args, "BB", &device, &csp_addr)) {
        Py_RETURN_NONE;
    }

    int res = gs_csp_i2c_init(device, csp_addr);
    if (res != GS_OK) {
        return PyErr_Error("gs_csp_i2c_init()", res);
    }

    Py_RETURN_NONE;
}

static PyMethodDef methods[] = {

    {"i2c_init", pygscsp_csp_i2c_init, METH_VARARGS, ""},

    /* sentinel */
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "libgscsp_py3",
    NULL,
    -1,
    methods,
    NULL,
    NULL,
    NULL,
    NULL
};

PyMODINIT_FUNC PyInit_libgscsp_py3(void) {

    PyObject* m = PyModule_Create(&moduledef);

    /* Exceptions */
    Error = PyErr_NewException((char*)"gs.csp.Error", NULL, NULL);
    Py_INCREF(Error);

    /* Add exception object to your module */
    PyModule_AddObject(m, "Error", Error);

    return m;
}
