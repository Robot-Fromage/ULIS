// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   PyULIS2
*__________________
*
* @file         _PyULIS2_PixelValue.ipp
* @author       Clement Berthaud
* @brief        Python3 module for ULIS2.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <ULIS2>
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "Types/PyULIS2_AbstractPixel.ipp"

/////////////////////////////////////////////////////
/// Object Structure
typedef struct {
    _PyULIS2Object_AbstractPixel super;
} _PyULIS2Object_PixelValue;

/////////////////////////////////////////////////////
/// Dealloc
static void
_PyULIS2Object_PixelValue_dealloc( _PyULIS2Object_PixelValue* self ) {
    delete self->super._mPixel;
    Py_TYPE( self )->tp_free( ( PyObject * ) self );
}

/////////////////////////////////////////////////////
/// Alloc
static PyObject* _PyULIS2Object_PixelValue_new( PyTypeObject* type, PyObject* args, PyObject* kwds )
{
    _PyULIS2Object_PixelValue *self;
    self = (_PyULIS2Object_PixelValue*)type->tp_alloc( type, 0 );
    if (self != NULL) self->super._mPixel = nullptr;
    return (PyObject *) self;
}

/////////////////////////////////////////////////////
/// Init
static int
_PyULIS2Object_PixelValue_init( _PyULIS2Object_PixelValue* self, PyObject* args, PyObject* kwds )
{
    static char *kwlist[] = { "format", NULL };
    ::ul2::tFormat fmt = 0;

    if( !PyArg_ParseTupleAndKeywords( args, kwds, "k", kwlist, &fmt ) ) {
        PyErr_SetString( PyExc_TypeError, "Bad input argmuents in init" );
        return -1;
    }

    self->super._mPixel = new ::ul2::FPixelValue( fmt );
    return 0;
}

/////////////////////////////////////////////////////
// Methods
/////////////////////////////////////////////////////
/// Meta Methods
static PyMethodDef _PyULIS2Object_PixelValue_methods[] = {
    { NULL } // Sentinel
};

/////////////////////////////////////////////////////
/// Meta Types
static PyTypeObject FPixelValue = {
PyVarObject_HEAD_INIT(NULL, 0)
    "PyULIS2.FPixelValue", /* tp_name */
    sizeof( _PyULIS2Object_PixelValue ), /* tp_basicsize */
    0, /* tp_itemsize */
    (destructor)_PyULIS2Object_PixelValue_dealloc, /* tp_dealloc */
    0, /* tp_print */
    0, /* tp_getattr */
    0, /* tp_setattr */
    0, /* tp_as_async */
    0, /* tp_repr */
    0, /* tp_as_number */
    0, /* tp_as_sequence */
    0, /* tp_as_mapping */
    0, /* tp_hash */
    0, /* tp_call */
    0, /* tp_str */
    0, /* tp_getattro */
    0, /* tp_setattro */
    0, /* tp_as_buffer */
    Py_TPFLAGS_BASETYPE | Py_TPFLAGS_DEFAULT, /* tp_flags */
    "ULIS2 Pixel Value Object.", /* tp_doc */
    0, /* tp_traverse */
    0, /* tp_clear */
    0, /* tp_richcompare */
    0, /* tp_weaklistoffset */
    0, /* tp_iter */
    0, /* tp_iternext */
    _PyULIS2Object_PixelValue_methods, /* tp_methods */
    0, /* tp_members */
    0, /* tp_getset */
    &IPixel, /* tp_base */
    0, /* tp_dict */
    0, /* tp_descr_get */
    0, /* tp_descr_set */
    0, /* tp_dictoffset */
    (initproc)_PyULIS2Object_PixelValue_init, /* tp_init */
    0, /* tp_alloc */
    _PyULIS2Object_PixelValue_new, /* tp_new */
    0, /* tp_free */
    0, /* tp_is_gc */
    0, /* tp_bases */
    0, /* tp_mro */
    0, /* tp_cache */
    0, /* tp_subclasses */
    0, /* tp_weaklist */
    0, /* tp_del */
    0, /* tp_version_tag */
    0, /* tp_finalize */
};

