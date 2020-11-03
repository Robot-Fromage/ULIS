// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   PyULIS3
*__________________
*
* @file         _PyULIS3_PixelProxy.ipp
* @author       Clement Berthaud
* @brief        Python3 module for ULIS3.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <ULIS3>
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "Types/AbstractPixel.ipp"

/////////////////////////////////////////////////////
/// Object Structure
typedef struct {
    _PyULIS3Object_AbstractPixel super;
} _PyULIS3Object_PixelProxy;

/////////////////////////////////////////////////////
/// Dealloc
static void
_PyULIS3Object_PixelProxy_dealloc( _PyULIS3Object_PixelProxy* self ) {
    delete self->super._mPixel;
    Py_TYPE( self )->tp_free( ( PyObject * ) self );
}

/////////////////////////////////////////////////////
/// Alloc
static PyObject* _PyULIS3Object_PixelProxy_new( PyTypeObject* type, PyObject* args, PyObject* kwds )
{
    _PyULIS3Object_PixelProxy *self;
    self = (_PyULIS3Object_PixelProxy*)type->tp_alloc( type, 0 );
    if (self != NULL) self->super._mPixel = nullptr;
    return (PyObject *) self;
}

/////////////////////////////////////////////////////
/// Init
static int
_PyULIS3Object_PixelProxy_init( _PyULIS3Object_PixelProxy* self, PyObject* args, PyObject* kwds )
{
    static char *kwlist[] = { "format", NULL };
    ::ul3::tFormat fmt = 0;

    if( !PyArg_ParseTupleAndKeywords( args, kwds, "k", kwlist, &fmt ) ) {
        PyErr_SetString( PyExc_TypeError, "Bad input argmuents in init" );
        return -1;
    }

    self->super._mPixel = new ::ul3::FPixelProxy( fmt );
    return 0;
}

/////////////////////////////////////////////////////
// Methods
/////////////////////////////////////////////////////
/// Meta Methods
static PyMethodDef _PyULIS3Object_PixelProxy_methods[] = {
    { NULL } // Sentinel
};

/////////////////////////////////////////////////////
/// Meta Types
static PyTypeObject FPixelProxy = {
PyVarObject_HEAD_INIT(NULL, 0)
    "PyULIS3.FPixelProxy", /* tp_name */
    sizeof( _PyULIS3Object_PixelProxy ), /* tp_basicsize */
    0, /* tp_itemsize */
    (destructor)_PyULIS3Object_PixelProxy_dealloc, /* tp_dealloc */
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
    "ULIS3 Pixel Value Object.", /* tp_doc */
    0, /* tp_traverse */
    0, /* tp_clear */
    0, /* tp_richcompare */
    0, /* tp_weaklistoffset */
    0, /* tp_iter */
    0, /* tp_iternext */
    _PyULIS3Object_PixelProxy_methods, /* tp_methods */
    0, /* tp_members */
    0, /* tp_getset */
    &IPixel, /* tp_base */
    0, /* tp_dict */
    0, /* tp_descr_get */
    0, /* tp_descr_set */
    0, /* tp_dictoffset */
    (initproc)_PyULIS3Object_PixelProxy_init, /* tp_init */
    0, /* tp_alloc */
    _PyULIS3Object_PixelProxy_new, /* tp_new */
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

