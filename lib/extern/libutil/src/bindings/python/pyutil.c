/* Copyright (c) 2013-2017 GomSpace A/S. All rights reserved. */

#include <Python.h>
#include <gs/util/clock.h>
#include <gs/util/error.h>

#if PY_MAJOR_VERSION == 3
#define IS_PY3
#endif

/**
 * Helpers
 */

static PyObject* pyutil_get_clock_time(PyObject *self, PyObject *args) {
	gs_timestamp_t ts;
	gs_clock_get_time(&ts);
	return Py_BuildValue("II", ts.tv_sec, ts.tv_nsec);
}


static PyObject* pyutil_error_string(PyObject *self, PyObject *args)
{
    int error;
    if (!PyArg_ParseTuple(args, "i", &error))
    {
        Py_RETURN_NONE;
    }
    return Py_BuildValue("s", gs_error_string(error));
}

static PyMethodDef methods[] = {

    /* helpers */
	{"get_clock_time", pyutil_get_clock_time, METH_NOARGS, ""},
    {"error_string", pyutil_error_string, METH_VARARGS, ""},

    /* sentinel */
    {NULL, NULL, 0, NULL}
};

#ifdef IS_PY3
static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "libgsutil_py3",
    NULL, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
	methods,
	NULL,
	NULL,
	NULL,
	NULL
};
#endif

#ifdef IS_PY3
PyMODINIT_FUNC PyInit_libgsutil_py3(void) {
#else
PyMODINIT_FUNC initlibgsutil_py2(void) {
#endif

#ifdef IS_PY3
    PyObject* m  = PyModule_Create(&moduledef);
#else
    Py_InitModule("libgsutil_py2", methods);
#endif

#ifdef IS_PY3
        return m;
#endif
}

