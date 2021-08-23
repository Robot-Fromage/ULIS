// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   PyULIS3
*__________________
*
* @file         _PyULIS3.cpp
* @author       Clement Berthaud
* @brief        Python3 module for ULIS3.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS3>
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "macros.h"
#include "Types/Block.ipp"
#include "Types/AbstractPixel.ipp"
#include "Types/PixelProxy.ipp"
#include "Types/PixelValue.ipp"

PyObject*
spam_system( PyObject *self, PyObject *args ) {
    const char *command; int sts;
    if (!PyArg_ParseTuple(args, "s", &command)) return NULL;
    sts = system(command);
    if (sts < 0) return NULL;
    return PyLong_FromLong(sts);
}


static PyMethodDef PyULIS3Methods[] = {
    { "system",  spam_system, METH_VARARGS, "Execute a shell command." },
    { NULL, NULL, 0, NULL } // Sentinel
};

static struct PyModuleDef PyULIS3Module  = {
      PyModuleDef_HEAD_INIT
    , "PyULIS3"                 // name of module
    , NULL                      // module documentation, may be NULL
    , -1                        // size of per-interpreter state of the module, or -1 if the module keeps state in global variables.
    , PyULIS3Methods
};

PyMODINIT_FUNC
PyInit_PyULIS3(void) {
    PyObject *m;

    PYULIS3_CHECK_FOR_ALL_TYPES_READY

    m = PyModule_Create( &PyULIS3Module );
    if( m == NULL )
        return  NULL;

    PYULIS3_FOR_ALL_TYPES_IMPORT

    return  m;
}
