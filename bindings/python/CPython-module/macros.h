// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   PyULIS3
*__________________
*
* @file         macros.h
* @author       Clement Berthaud
* @brief        Python3 module for ULIS3.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <ULIS3>
#define PY_SSIZE_T_CLEAN
#include <Python.h>

// Macro for all types for template instanciation
#define PYULIS3_FOR_ALL_OBJECTS_DO( X, ... )            \
    X( _PyULIS3Object_Block,            __VA_ARGS__ )   \
    X( _PyULIS3Object_AbstractPixel,    __VA_ARGS__ )   \
    X( _PyULIS3Object_PixelProxy,       __VA_ARGS__ )   \
    X( _PyULIS3Object_PixelValue,       __VA_ARGS__ )

#define PYULIS3_FOR_ALL_TYPES_DO( X, ... )              \
    X( FBlock, __VA_ARGS__ )                            \
    X( IPixel, __VA_ARGS__ )                            \
    X( FPixelProxy, __VA_ARGS__ )                       \
    X( FPixelValue, __VA_ARGS__ )

#define PYULIS3_CHECK_TYPE_READY( _TYPE, ... )  if( PyType_Ready( & _TYPE ) < 0 ) return NULL;
#define PYULIS3_CHECK_FOR_ALL_TYPES_READY       PYULIS3_FOR_ALL_TYPES_DO( PYULIS3_CHECK_TYPE_READY )

#define PYULIS3_IMPORT_TYPE( _TYPE, ... )                                                       \
    Py_INCREF( & _TYPE );                                                                       \
    if( PyModule_AddObject( m, #_TYPE , (PyObject*)& _TYPE ) < 0 ) {                            \
        Py_DECREF( & _TYPE );                                                                   \
        Py_DECREF( m );                                                                         \
        return  NULL;                                                                           \
    }
#define PYULIS3_FOR_ALL_TYPES_IMPORT            PYULIS3_FOR_ALL_TYPES_DO( PYULIS3_IMPORT_TYPE )
