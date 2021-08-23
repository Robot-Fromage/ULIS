// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   PyULIS2
*__________________
*
* @file         _PyULIS2_AbstractPixel.ipp
* @author       Clement Berthaud
* @brief        Python3 module for ULIS2.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <ULIS2>
#define PY_SSIZE_T_CLEAN
#include <Python.h>

/////////////////////////////////////////////////////
/// Object Structure
typedef struct {
    PyObject_HEAD
    ::ul2::IPixel* _mPixel;
} _PyULIS2Object_AbstractPixel;

/////////////////////////////////////////////////////
/// Dealloc
static void
_PyULIS2Object_AbstractPixel_dealloc( _PyULIS2Object_AbstractPixel* self ) {
    Py_TYPE( self )->tp_free( ( PyObject * ) self );
}

/////////////////////////////////////////////////////
/// Alloc
static PyObject* _PyULIS2Object_AbstractPixel_new( PyTypeObject* type, PyObject* args, PyObject* kwds )
{
    _PyULIS2Object_AbstractPixel *self;
    self = (_PyULIS2Object_AbstractPixel*)type->tp_alloc( type, 0 );
    if (self != NULL) self->_mPixel = nullptr;
    return (PyObject *) self;
}

/////////////////////////////////////////////////////
/// Init
static int
_PyULIS2Object_AbstractPixel_init( _PyULIS2Object_AbstractPixel* self, PyObject* args, PyObject* kwds )
{
    return 0;
}

/////////////////////////////////////////////////////
// Methods
#define _PyULIS2Object_AbstractPixel_GetterMethod_Begin( iName )                                                        \
    static PyObject *                                                                                                   \
    _PyULIS2Object_AbstractPixel_ ## iName ( _PyULIS2Object_AbstractPixel* self, PyObject* Py_UNUSED( ignored ) )       \
    {                                                                                                                   \
        if (self->_mPixel == nullptr) {                                                                                 \
            PyErr_SetString( PyExc_AttributeError, "Bad Access to uninitialized _PyULIS2Object_AbstractPixel object" ); \
            return NULL;                                                                                                \
        }
#define _PyULIS2Object_AbstractPixel_GetterMethod_End }
#define _PyULIS2Object_AbstractPixel_CheckError_SelfNULL if( self->_mPixel == nullptr ) { PyErr_SetString( PyExc_AttributeError, "Bad Access to uninitialized _PyULIS2Object_AbstractPixel object" ); return NULL; }

#define _PyULIS2Object_AbstractPixel_SetterMethod_Begin( iName )                                                        \
    static PyObject *                                                                                                   \
    _PyULIS2Object_AbstractPixel_ ## iName ( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {                     \
        _PyULIS2Object_AbstractPixel_CheckError_SelfNULL;
#define _PyULIS2Object_AbstractPixel_SetterMethod_End }

_PyULIS2Object_AbstractPixel_GetterMethod_Begin( BytesPerSample     )   return  PyLong_FromLong( self->_mPixel->BytesPerSample() );    _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Depth              )   return  PyLong_FromLong( self->_mPixel->Depth() );             _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Format             )   return  PyLong_FromLong( self->_mPixel->Format() );            _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Model              )   return  PyLong_FromLong( self->_mPixel->Model() );             _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Type               )   return  PyLong_FromLong( self->_mPixel->Type() );              _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( HasAlpha           )   return  PyLong_FromLong( self->_mPixel->HasAlpha() );          _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Swapped            )   return  PyLong_FromLong( self->_mPixel->Swapped() );           _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Reversed           )   return  PyLong_FromLong( self->_mPixel->Reversed() );          _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( NumSamples         )   return  PyLong_FromLong( self->_mPixel->NumSamples() );        _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( NumColorChannels   )   return  PyLong_FromLong( self->_mPixel->NumColorChannels() );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( AlphaIndex         )   return  PyLong_FromLong( self->_mPixel->AlphaIndex() );        _PyULIS2Object_AbstractPixel_GetterMethod_End
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Getters
static PyObject* _PyULIS2Object_AbstractPixel_GetValueAtIndex8( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL;
    return  PyLong_FromLong( self->_mPixel->GetValue< ::ul2::uint8 >( i ) );
}
static PyObject* _PyULIS2Object_AbstractPixel_GetValueAtIndex16( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL;
    return  PyLong_FromLong( self->_mPixel->GetValue< ::ul2::uint16 >( i ) );
}
static PyObject* _PyULIS2Object_AbstractPixel_GetValueAtIndex32( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL;
    return  PyFloat_FromDouble( self->_mPixel->GetValue< ::ul2::uint32 >( i ) );
}
static PyObject* _PyULIS2Object_AbstractPixel_GetValueAtIndexF( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL;
    return  PyFloat_FromDouble( self->_mPixel->GetValue< ::ul2::ufloat >( i ) );
}
static PyObject* _PyULIS2Object_AbstractPixel_GetValueAtIndexD( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL;
    return  PyFloat_FromDouble( self->_mPixel->GetValue< ::ul2::udouble >( i ) );
}
//Raw
static PyObject* _PyULIS2Object_AbstractPixel_GetValueAtRawIndex8( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL;
    return  PyLong_FromLong( self->_mPixel->GetValueRaw< ::ul2::uint8 >( i ) );
}
static PyObject* _PyULIS2Object_AbstractPixel_GetValueAtRawIndex16( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL;
    return  PyLong_FromLong( self->_mPixel->GetValueRaw< ::ul2::uint16 >( i ) );
}
static PyObject* _PyULIS2Object_AbstractPixel_GetValueAtRawIndex32( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL;
    return  PyFloat_FromDouble( self->_mPixel->GetValueRaw< ::ul2::uint32 >( i ) );
}
static PyObject* _PyULIS2Object_AbstractPixel_GetValueAtRawIndexF( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL;
    return  PyFloat_FromDouble( self->_mPixel->GetValueRaw< ::ul2::ufloat >( i ) );
}
static PyObject* _PyULIS2Object_AbstractPixel_GetValueAtRawIndexD( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL;
    return  PyFloat_FromDouble( self->_mPixel->GetValueRaw< ::ul2::udouble >( i ) );
}
//Alpha
static PyObject* _PyULIS2Object_AbstractPixel_Alpha8( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL;
    return  PyLong_FromLong( self->_mPixel->Alpha< ::ul2::uint8 >() );
}
static PyObject* _PyULIS2Object_AbstractPixel_Alpha16( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL;
    return  PyLong_FromLong( self->_mPixel->Alpha< ::ul2::uint16 >() );
}
static PyObject* _PyULIS2Object_AbstractPixel_Alpha32( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL;
    return  PyFloat_FromDouble( self->_mPixel->Alpha< ::ul2::uint32 >() );
}
static PyObject* _PyULIS2Object_AbstractPixel_AlphaF( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL;
    return  PyFloat_FromDouble( self->_mPixel->Alpha< ::ul2::ufloat >() );
}
static PyObject* _PyULIS2Object_AbstractPixel_AlphaD( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL;
    return  PyFloat_FromDouble( self->_mPixel->Alpha< ::ul2::udouble >() );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------ Setters
static PyObject* _PyULIS2Object_AbstractPixel_SetValueAtIndex8( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i, v;
    if( !PyArg_ParseTuple( args, "ii", &i, &v ) ) return  NULL;
    ::ul2::uint8 val = static_cast< ::ul2::uint8 >( ::ul2::FMaths::Clamp( v, 0, 0xFF ) );
    self->_mPixel->SetValue< ::ul2::uint8 >( i, val );
    return  Py_None;
}
static PyObject* _PyULIS2Object_AbstractPixel_SetValueAtIndex16( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i, v;
    if( !PyArg_ParseTuple( args, "ii", &i, &v ) ) return  NULL;
    ::ul2::uint16 val = static_cast< ::ul2::uint16 >( ::ul2::FMaths::Clamp( v, 0, 0xFFFF ) );
    self->_mPixel->SetValue< ::ul2::uint16 >( i, val );
    return  Py_None;
}
static PyObject* _PyULIS2Object_AbstractPixel_SetValueAtIndex32( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i, v;
    if( !PyArg_ParseTuple( args, "ii", &i, &v ) ) return  NULL;
    ::ul2::uint32 val = static_cast< ::ul2::uint32 >( ::ul2::FMaths::Min( static_cast< ::ul2::uint32 >( ::ul2::FMaths::Max( v, 0 ) ), UINT32_MAX ) );
    self->_mPixel->SetValue< ::ul2::uint32 >( i, val );
    return  Py_None;
}
static PyObject* _PyULIS2Object_AbstractPixel_SetValueAtIndexF( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i;
    float v;
    if( !PyArg_ParseTuple( args, "if", &i, &v ) ) return  NULL;
    v = ::ul2::FMaths::Clamp( v, 0.f, 1.f );
    self->_mPixel->SetValue< ::ul2::ufloat >( i, v );
    return  Py_None;
}
static PyObject* _PyULIS2Object_AbstractPixel_SetValueAtIndexD( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i;
    double v;
    if( !PyArg_ParseTuple( args, "id", &i, &v ) ) return  NULL;
    v = ::ul2::FMaths::Clamp( v, 0.0, 1.0 );
    self->_mPixel->SetValue< ::ul2::udouble >( i, v );
    return  Py_None;
}
// Raw
static PyObject* _PyULIS2Object_AbstractPixel_SetValueAtRawIndex8( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i, v;
    if( !PyArg_ParseTuple( args, "ii", &i, &v ) ) return  NULL;
    ::ul2::uint8 val = static_cast< ::ul2::uint8 >( ::ul2::FMaths::Clamp( v, 0, 0xFF ) );
    self->_mPixel->SetValueRaw< ::ul2::uint8 >( i, val );
    return  Py_None;
}
static PyObject* _PyULIS2Object_AbstractPixel_SetValueAtRawIndex16( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i, v;
    if( !PyArg_ParseTuple( args, "ii", &i, &v ) ) return  NULL;
    ::ul2::uint16 val = static_cast< ::ul2::uint16 >( ::ul2::FMaths::Clamp( v, 0, 0xFFFF ) );
    self->_mPixel->SetValueRaw< ::ul2::uint16 >( i, val );
    return  Py_None;
}
static PyObject* _PyULIS2Object_AbstractPixel_SetValueAtRawIndex32( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i, v;
    if( !PyArg_ParseTuple( args, "ii", &i, &v ) ) return  NULL;
    ::ul2::uint32 val = static_cast< ::ul2::uint32 >( ::ul2::FMaths::Min( static_cast< ::ul2::uint32 >( ::ul2::FMaths::Max( v, 0 ) ), UINT32_MAX ) );
    self->_mPixel->SetValueRaw< ::ul2::uint32 >( i, val );
    return  Py_None;
}
static PyObject* _PyULIS2Object_AbstractPixel_SetValueAtRawIndexF( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i;
    float v;
    if( !PyArg_ParseTuple( args, "if", &i, &v ) ) return  NULL;
    v = ::ul2::FMaths::Clamp( v, 0.f, 1.f );
    self->_mPixel->SetValueRaw< ::ul2::ufloat >( i, v );
    return  Py_None;
}
static PyObject* _PyULIS2Object_AbstractPixel_SetValueAtRawIndexD( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i;
    double v;
    if( !PyArg_ParseTuple( args, "id", &i, &v ) ) return  NULL;
    v = ::ul2::FMaths::Clamp( v, 0.0, 1.0 );
    self->_mPixel->SetValueRaw< ::ul2::udouble >( i, v );
    return  Py_None;
}
// Alpha
static PyObject* _PyULIS2Object_AbstractPixel_SetAlpha8( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i, v;
    if( !PyArg_ParseTuple( args, "ii", &i, &v ) ) return  NULL;
    ::ul2::uint8 val = static_cast< ::ul2::uint8 >( ::ul2::FMaths::Clamp( v, 0, 0xFF ) );
    self->_mPixel->SetAlpha< ::ul2::uint8 >( val );
    return  Py_None;
}
static PyObject* _PyULIS2Object_AbstractPixel_SetAlpha16( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i, v;
    if( !PyArg_ParseTuple( args, "ii", &i, &v ) ) return  NULL;
    ::ul2::uint16 val = static_cast< ::ul2::uint16 >( ::ul2::FMaths::Clamp( v, 0, 0xFFFF ) );
    self->_mPixel->SetAlpha< ::ul2::uint16 >( val );
    return  Py_None;
}
static PyObject* _PyULIS2Object_AbstractPixel_SetAlpha32( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i, v;
    if( !PyArg_ParseTuple( args, "ii", &i, &v ) ) return  NULL;
    ::ul2::uint32 val = static_cast< ::ul2::uint32 >( ::ul2::FMaths::Min( static_cast< ::ul2::uint32 >( ::ul2::FMaths::Max( v, 0 ) ), UINT32_MAX ) );
    self->_mPixel->SetAlpha< ::ul2::uint32 >( val );
    return  Py_None;
}
static PyObject* _PyULIS2Object_AbstractPixel_SetAlphaF( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i;
    float v;
    if( !PyArg_ParseTuple( args, "if", &i, &v ) ) return  NULL;
    v = ::ul2::FMaths::Clamp( v, 0.f, 1.f );
    self->_mPixel->SetAlpha< ::ul2::ufloat >( v );
    return  Py_None;
}
static PyObject* _PyULIS2Object_AbstractPixel_SetAlphaD( _PyULIS2Object_AbstractPixel* self, PyObject* args ) {
    _PyULIS2Object_AbstractPixel_CheckError_SelfNULL
    int i;
    double v;
    if( !PyArg_ParseTuple( args, "id", &i, &v ) ) return  NULL;
    v = ::ul2::FMaths::Clamp( v, 0.0, 1.0 );
    self->_mPixel->SetAlpha< ::ul2::udouble >( v );
    return  Py_None;
}

/////////////////////////////////////////////////////
/// Named index getters
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( A8             )   return  PyLong_FromLong( self->_mPixel->A8()            );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Grey8          )   return  PyLong_FromLong( self->_mPixel->Grey8()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Red8           )   return  PyLong_FromLong( self->_mPixel->Red8()          );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Green8         )   return  PyLong_FromLong( self->_mPixel->Green8()        );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Blue8          )   return  PyLong_FromLong( self->_mPixel->Blue8()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( R8             )   return  PyLong_FromLong( self->_mPixel->R8()            );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( G8             )   return  PyLong_FromLong( self->_mPixel->G8()            );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( B8             )   return  PyLong_FromLong( self->_mPixel->B8()            );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Hue8           )   return  PyLong_FromLong( self->_mPixel->Hue8()          );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Saturation8    )   return  PyLong_FromLong( self->_mPixel->Saturation8()   );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Lightness8     )   return  PyLong_FromLong( self->_mPixel->Lightness8()    );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Value8         )   return  PyLong_FromLong( self->_mPixel->Value8()        );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Cyan8          )   return  PyLong_FromLong( self->_mPixel->Cyan8()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Magenta8       )   return  PyLong_FromLong( self->_mPixel->Magenta8()      );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Yellow8        )   return  PyLong_FromLong( self->_mPixel->Yellow8()       );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Key8           )   return  PyLong_FromLong( self->_mPixel->Key8()          );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Luma8          )   return  PyLong_FromLong( self->_mPixel->Luma8()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( U8             )   return  PyLong_FromLong( self->_mPixel->U8()            );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( V8             )   return  PyLong_FromLong( self->_mPixel->V8()            );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( K8             )   return  PyLong_FromLong( self->_mPixel->K8()            );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( X8             )   return  PyLong_FromLong( self->_mPixel->X8()            );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Y8             )   return  PyLong_FromLong( self->_mPixel->Y8()            );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Z8             )   return  PyLong_FromLong( self->_mPixel->Z8()            );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( L8             )   return  PyLong_FromLong( self->_mPixel->L8()            );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( a8             )   return  PyLong_FromLong( self->_mPixel->a8()            );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( b8             )   return  PyLong_FromLong( self->_mPixel->b8()            );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( x8             )   return  PyLong_FromLong( self->_mPixel->x8()            );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( y8             )   return  PyLong_FromLong( self->_mPixel->y8()            );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( A16            )   return  PyLong_FromLong( self->_mPixel->A16()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Grey16         )   return  PyLong_FromLong( self->_mPixel->Grey16()        );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Red16          )   return  PyLong_FromLong( self->_mPixel->Red16()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Green16        )   return  PyLong_FromLong( self->_mPixel->Green16()       );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Blue16         )   return  PyLong_FromLong( self->_mPixel->Blue16()        );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( R16            )   return  PyLong_FromLong( self->_mPixel->R16()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( G16            )   return  PyLong_FromLong( self->_mPixel->G16()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( B16            )   return  PyLong_FromLong( self->_mPixel->B16()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Hue16          )   return  PyLong_FromLong( self->_mPixel->Hue16()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Saturation16   )   return  PyLong_FromLong( self->_mPixel->Saturation16()  );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Lightness16    )   return  PyLong_FromLong( self->_mPixel->Lightness16()   );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Value16        )   return  PyLong_FromLong( self->_mPixel->Value16()       );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Cyan16         )   return  PyLong_FromLong( self->_mPixel->Cyan16()        );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Magenta16      )   return  PyLong_FromLong( self->_mPixel->Magenta16()     );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Yellow16       )   return  PyLong_FromLong( self->_mPixel->Yellow16()      );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Key16          )   return  PyLong_FromLong( self->_mPixel->Key16()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Luma16         )   return  PyLong_FromLong( self->_mPixel->Luma16()        );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( U16            )   return  PyLong_FromLong( self->_mPixel->U16()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( V16            )   return  PyLong_FromLong( self->_mPixel->V16()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( K16            )   return  PyLong_FromLong( self->_mPixel->K16()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( X16            )   return  PyLong_FromLong( self->_mPixel->X16()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Y16            )   return  PyLong_FromLong( self->_mPixel->Y16()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Z16            )   return  PyLong_FromLong( self->_mPixel->Z16()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( L16            )   return  PyLong_FromLong( self->_mPixel->L16()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( a16            )   return  PyLong_FromLong( self->_mPixel->a16()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( b16            )   return  PyLong_FromLong( self->_mPixel->b16()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( x16            )   return  PyLong_FromLong( self->_mPixel->x16()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( y16            )   return  PyLong_FromLong( self->_mPixel->y16()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( A32            )   return  PyLong_FromLong( self->_mPixel->A32()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Grey32         )   return  PyLong_FromLong( self->_mPixel->Grey32()        );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Red32          )   return  PyLong_FromLong( self->_mPixel->Red32()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Green32        )   return  PyLong_FromLong( self->_mPixel->Green32()       );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Blue32         )   return  PyLong_FromLong( self->_mPixel->Blue32()        );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( R32            )   return  PyLong_FromLong( self->_mPixel->R32()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( G32            )   return  PyLong_FromLong( self->_mPixel->G32()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( B32            )   return  PyLong_FromLong( self->_mPixel->B32()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Hue32          )   return  PyLong_FromLong( self->_mPixel->Hue32()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Saturation32   )   return  PyLong_FromLong( self->_mPixel->Saturation32()  );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Lightness32    )   return  PyLong_FromLong( self->_mPixel->Lightness32()   );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Value32        )   return  PyLong_FromLong( self->_mPixel->Value32()       );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Cyan32         )   return  PyLong_FromLong( self->_mPixel->Cyan32()        );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Magenta32      )   return  PyLong_FromLong( self->_mPixel->Magenta32()     );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Yellow32       )   return  PyLong_FromLong( self->_mPixel->Yellow32()      );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Key32          )   return  PyLong_FromLong( self->_mPixel->Key32()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Luma32         )   return  PyLong_FromLong( self->_mPixel->Luma32()        );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( U32            )   return  PyLong_FromLong( self->_mPixel->U32()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( V32            )   return  PyLong_FromLong( self->_mPixel->V32()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( K32            )   return  PyLong_FromLong( self->_mPixel->K32()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( X32            )   return  PyLong_FromLong( self->_mPixel->X32()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Y32            )   return  PyLong_FromLong( self->_mPixel->Y32()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( Z32            )   return  PyLong_FromLong( self->_mPixel->Z32()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( L32            )   return  PyLong_FromLong( self->_mPixel->L32()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( a32            )   return  PyLong_FromLong( self->_mPixel->a32()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( b32            )   return  PyLong_FromLong( self->_mPixel->b32()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( x32            )   return  PyLong_FromLong( self->_mPixel->x32()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( y32            )   return  PyLong_FromLong( self->_mPixel->y32()           );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( AF             )   return  PyFloat_FromDouble( self->_mPixel->AF()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( GreyF          )   return  PyFloat_FromDouble( self->_mPixel->GreyF()      );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( RedF           )   return  PyFloat_FromDouble( self->_mPixel->RedF()       );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( GreenF         )   return  PyFloat_FromDouble( self->_mPixel->GreenF()     );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( BlueF          )   return  PyFloat_FromDouble( self->_mPixel->BlueF()      );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( RF             )   return  PyFloat_FromDouble( self->_mPixel->RF()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( GF             )   return  PyFloat_FromDouble( self->_mPixel->GF()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( BF             )   return  PyFloat_FromDouble( self->_mPixel->BF()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( HueF           )   return  PyFloat_FromDouble( self->_mPixel->HueF()       );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( SaturationF    )   return  PyFloat_FromDouble( self->_mPixel->SaturationF());  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( LightnessF     )   return  PyFloat_FromDouble( self->_mPixel->LightnessF() );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( ValueF         )   return  PyFloat_FromDouble( self->_mPixel->ValueF()     );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( CyanF          )   return  PyFloat_FromDouble( self->_mPixel->CyanF()      );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( MagentaF       )   return  PyFloat_FromDouble( self->_mPixel->MagentaF()   );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( YellowF        )   return  PyFloat_FromDouble( self->_mPixel->YellowF()    );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( KeyF           )   return  PyFloat_FromDouble( self->_mPixel->KeyF()       );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( LumaF          )   return  PyFloat_FromDouble( self->_mPixel->LumaF()      );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( UF             )   return  PyFloat_FromDouble( self->_mPixel->UF()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( VF             )   return  PyFloat_FromDouble( self->_mPixel->VF()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( KF             )   return  PyFloat_FromDouble( self->_mPixel->KF()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( XF             )   return  PyFloat_FromDouble( self->_mPixel->XF()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( YF             )   return  PyFloat_FromDouble( self->_mPixel->YF()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( ZF             )   return  PyFloat_FromDouble( self->_mPixel->ZF()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( LF             )   return  PyFloat_FromDouble( self->_mPixel->LF()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( aF             )   return  PyFloat_FromDouble( self->_mPixel->aF()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( bF             )   return  PyFloat_FromDouble( self->_mPixel->bF()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( xF             )   return  PyFloat_FromDouble( self->_mPixel->xF()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( yF             )   return  PyFloat_FromDouble( self->_mPixel->yF()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( AD             )   return  PyFloat_FromDouble( self->_mPixel->AD()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( GreyD          )   return  PyFloat_FromDouble( self->_mPixel->GreyD()      );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( RedD           )   return  PyFloat_FromDouble( self->_mPixel->RedD()       );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( GreenD         )   return  PyFloat_FromDouble( self->_mPixel->GreenD()     );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( BlueD          )   return  PyFloat_FromDouble( self->_mPixel->BlueD()      );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( RD             )   return  PyFloat_FromDouble( self->_mPixel->RD()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( GD             )   return  PyFloat_FromDouble( self->_mPixel->GD()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( BD             )   return  PyFloat_FromDouble( self->_mPixel->BD()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( HueD           )   return  PyFloat_FromDouble( self->_mPixel->HueD()       );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( SaturationD    )   return  PyFloat_FromDouble( self->_mPixel->SaturationD());  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( LightnessD     )   return  PyFloat_FromDouble( self->_mPixel->LightnessD() );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( ValueD         )   return  PyFloat_FromDouble( self->_mPixel->ValueD()     );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( CyanD          )   return  PyFloat_FromDouble( self->_mPixel->CyanD()      );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( MagentaD       )   return  PyFloat_FromDouble( self->_mPixel->MagentaD()   );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( YellowD        )   return  PyFloat_FromDouble( self->_mPixel->YellowD()    );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( KeyD           )   return  PyFloat_FromDouble( self->_mPixel->KeyD()       );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( LumaD          )   return  PyFloat_FromDouble( self->_mPixel->LumaD()      );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( UD             )   return  PyFloat_FromDouble( self->_mPixel->UD()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( VD             )   return  PyFloat_FromDouble( self->_mPixel->VD()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( KD             )   return  PyFloat_FromDouble( self->_mPixel->KD()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( XD             )   return  PyFloat_FromDouble( self->_mPixel->XD()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( YD             )   return  PyFloat_FromDouble( self->_mPixel->YD()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( ZD             )   return  PyFloat_FromDouble( self->_mPixel->ZD()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( LD             )   return  PyFloat_FromDouble( self->_mPixel->LD()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( aD             )   return  PyFloat_FromDouble( self->_mPixel->aD()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( bD             )   return  PyFloat_FromDouble( self->_mPixel->bD()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( xD             )   return  PyFloat_FromDouble( self->_mPixel->xD()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_GetterMethod_Begin( yD             )   return  PyFloat_FromDouble( self->_mPixel->yD()         );  _PyULIS2Object_AbstractPixel_GetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetA8          ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetA8            ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetGrey8       ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetGrey8         ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetRed8        ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetRed8          ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetGreen8      ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetGreen8        ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetBlue8       ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetBlue8         ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetR8          ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetR8            ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetG8          ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetG8            ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetB8          ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetB8            ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetHue8        ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetHue8          ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetSaturation8 ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetSaturation8   ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetLightness8  ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetLightness8    ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetValue8      ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetValue8        ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetCyan8       ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetCyan8         ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetMagenta8    ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetMagenta8      ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetYellow8     ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetYellow8       ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetKey8        ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetKey8          ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetLuma8       ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetLuma8         ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetU8          ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetU8            ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetV8          ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetV8            ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetK8          ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetK8            ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetX8          ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetX8            ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetY8          ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetY8            ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetZ8          ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetZ8            ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetL8          ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetL8            ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( Seta8          ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->Seta8            ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( Setb8          ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->Setb8            ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( Setx8          ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->Setx8            ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( Sety8          ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->Sety8            ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetA16         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetA16           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetGrey16      ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetGrey16        ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetRed16       ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetRed16         ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetGreen16     ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetGreen16       ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetBlue16      ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetBlue16        ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetR16         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetR16           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetG16         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetG16           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetB16         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetB16           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetHue16       ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetHue16         ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetSaturation16) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetSaturation16  ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetLightness16 ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetLightness16   ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetValue16     ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetValue16       ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetCyan16      ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetCyan16        ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetMagenta16   ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetMagenta16     ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetYellow16    ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetYellow16      ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetKey16       ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetKey16         ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetLuma16      ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetLuma16        ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetU16         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetU16           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetV16         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetV16           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetK16         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetK16           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetX16         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetX16           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetY16         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetY16           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetZ16         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetZ16           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetL16         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetL16           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( Seta16         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->Seta16           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( Setb16         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->Setb16           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( Setx16         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->Setx16           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( Sety16         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->Sety16           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetA32         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetA32           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetGrey32      ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetGrey32        ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetRed32       ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetRed32         ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetGreen32     ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetGreen32       ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetBlue32      ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetBlue32        ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetR32         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetR32           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetG32         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetG32           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetB32         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetB32           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetHue32       ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetHue32         ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetSaturation32) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetSaturation32  ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetLightness32 ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetLightness32   ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetValue32     ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetValue32       ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetCyan32      ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetCyan32        ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetMagenta32   ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetMagenta32     ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetYellow32    ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetYellow32      ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetKey32       ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetKey32         ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetLuma32      ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetLuma32        ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetU32         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetU32           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetV32         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetV32           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetK32         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetK32           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetX32         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetX32           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetY32         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetY32           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetZ32         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetZ32           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetL32         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->SetL32           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( Seta32         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->Seta32           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( Setb32         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->Setb32           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( Setx32         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->Setx32           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( Sety32         ) int i; if( !PyArg_ParseTuple( args, "i", &i ) ) return  NULL; self->_mPixel->Sety32           ( i ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetAF          ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetAF          ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetGreyF       ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetGreyF       ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetRedF        ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetRedF        ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetGreenF      ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetGreenF      ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetBlueF       ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetBlueF       ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetRF          ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetRF          ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetGF          ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetGF          ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetBF          ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetBF          ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetHueF        ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetHueF        ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetSaturationF ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetSaturationF ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetLightnessF  ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetLightnessF  ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetValueF      ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetValueF      ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetCyanF       ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetCyanF       ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetMagentaF    ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetMagentaF    ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetYellowF     ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetYellowF     ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetKeyF        ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetKeyF        ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetLumaF       ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetLumaF       ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetUF          ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetUF          ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetVF          ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetVF          ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetKF          ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetKF          ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetXF          ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetXF          ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetYF          ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetYF          ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetZF          ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetZF          ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetLF          ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetLF          ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetaF          ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetaF          ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetbF          ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetbF          ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetxF          ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetxF          ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetyF          ) float f; if( !PyArg_ParseTuple( args, "f", &f ) ) return  NULL; self->_mPixel->SetyF          ( f ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetAD          ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetAD         ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetGreyD       ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetGreyD      ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetRedD        ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetRedD       ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetGreenD      ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetGreenD     ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetBlueD       ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetBlueD      ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetRD          ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetRD         ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetGD          ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetGD         ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetBD          ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetBD         ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetHueD        ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetHueD       ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetSaturationD ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetSaturationD( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetLightnessD  ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetLightnessD ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetValueD      ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetValueD     ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetCyanD       ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetCyanD      ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetMagentaD    ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetMagentaD   ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetYellowD     ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetYellowD    ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetKeyD        ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetKeyD       ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetLumaD       ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetLumaD      ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetUD          ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetUD         ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetVD          ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetVD         ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetKD          ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetKD         ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetXD          ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetXD         ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetYD          ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetYD         ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetZD          ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetZD         ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetLD          ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetLD         ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetaD          ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetaD         ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetbD          ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetbD         ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetxD          ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetxD         ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
_PyULIS2Object_AbstractPixel_SetterMethod_Begin( SetyD          ) double d; if( !PyArg_ParseTuple( args, "d", &d ) ) return  NULL; self->_mPixel->SetyD         ( d ); return  Py_None; _PyULIS2Object_AbstractPixel_SetterMethod_End
/////////////////////////////////////////////////////
/// Meta Methods
static PyMethodDef _PyULIS2Object_AbstractPixel_methods[] = {
    { "BytesPerSample"      , (PyCFunction)_PyULIS2Object_AbstractPixel_BytesPerSample      , METH_NOARGS, "BytesPerSample"         },
    { "Depth"               , (PyCFunction)_PyULIS2Object_AbstractPixel_Depth               , METH_NOARGS, "Depth"                  },
    { "Format"              , (PyCFunction)_PyULIS2Object_AbstractPixel_Format              , METH_NOARGS, "Format"                 },
    { "Model"               , (PyCFunction)_PyULIS2Object_AbstractPixel_Model               , METH_NOARGS, "Model"                  },
    { "Type"                , (PyCFunction)_PyULIS2Object_AbstractPixel_Type                , METH_NOARGS, "Type"                   },
    { "HasAlpha"            , (PyCFunction)_PyULIS2Object_AbstractPixel_HasAlpha            , METH_NOARGS, "HasAlpha"               },
    { "Swapped"             , (PyCFunction)_PyULIS2Object_AbstractPixel_Swapped             , METH_NOARGS, "Swapped"                },
    { "Reversed"            , (PyCFunction)_PyULIS2Object_AbstractPixel_Reversed            , METH_NOARGS, "Reversed"               },
    { "NumSamples"          , (PyCFunction)_PyULIS2Object_AbstractPixel_NumSamples          , METH_NOARGS, "NumSamples"             },
    { "NumColorChannels"    , (PyCFunction)_PyULIS2Object_AbstractPixel_NumColorChannels    , METH_NOARGS, "NumColorChannels"       },
    { "AlphaIndex"          , (PyCFunction)_PyULIS2Object_AbstractPixel_AlphaIndex          , METH_NOARGS, "AlphaIndex"             },
    { "GetValueAtIndex8"    , (PyCFunction)_PyULIS2Object_AbstractPixel_GetValueAtIndex8    , METH_VARARGS, "GetValueAtIndex8"      },
    { "GetValueAtIndex16"   , (PyCFunction)_PyULIS2Object_AbstractPixel_GetValueAtIndex16   , METH_VARARGS, "GetValueAtIndex16"     },
    { "GetValueAtIndex32"   , (PyCFunction)_PyULIS2Object_AbstractPixel_GetValueAtIndex32   , METH_VARARGS, "GetValueAtIndex32"     },
    { "GetValueAtIndexF"    , (PyCFunction)_PyULIS2Object_AbstractPixel_GetValueAtIndexF    , METH_VARARGS, "GetValueAtIndexF"      },
    { "GetValueAtIndexD"    , (PyCFunction)_PyULIS2Object_AbstractPixel_GetValueAtIndexD    , METH_VARARGS, "GetValueAtIndexD"      },
    { "GetValueAtRawIndex8" , (PyCFunction)_PyULIS2Object_AbstractPixel_GetValueAtRawIndex8 , METH_VARARGS, "GetValueAtRawIndex8"   },
    { "GetValueAtRawIndex16", (PyCFunction)_PyULIS2Object_AbstractPixel_GetValueAtRawIndex16, METH_VARARGS, "GetValueAtRawIndex16"  },
    { "GetValueAtRawIndex32", (PyCFunction)_PyULIS2Object_AbstractPixel_GetValueAtRawIndex32, METH_VARARGS, "GetValueAtRawIndex32"  },
    { "GetValueAtRawIndexF" , (PyCFunction)_PyULIS2Object_AbstractPixel_GetValueAtRawIndexF , METH_VARARGS, "GetValueAtRawIndexF"   },
    { "GetValueAtRawIndexD" , (PyCFunction)_PyULIS2Object_AbstractPixel_GetValueAtRawIndexD , METH_VARARGS, "GetValueAtRawIndexD"   },
    { "Alpha8"              , (PyCFunction)_PyULIS2Object_AbstractPixel_Alpha8              , METH_VARARGS, "Alpha8"                },
    { "Alpha16"             , (PyCFunction)_PyULIS2Object_AbstractPixel_Alpha16             , METH_VARARGS, "Alpha16"               },
    { "Alpha32"             , (PyCFunction)_PyULIS2Object_AbstractPixel_Alpha32             , METH_VARARGS, "Alpha32"               },
    { "AlphaF"              , (PyCFunction)_PyULIS2Object_AbstractPixel_AlphaF              , METH_VARARGS, "AlphaF"                },
    { "AlphaD"              , (PyCFunction)_PyULIS2Object_AbstractPixel_AlphaD              , METH_VARARGS, "AlphaD"                },
    { "SetValueAtIndex8"    , (PyCFunction)_PyULIS2Object_AbstractPixel_SetValueAtIndex8    , METH_VARARGS, "SetValueAtIndex8"      },
    { "SetValueAtIndex16"   , (PyCFunction)_PyULIS2Object_AbstractPixel_SetValueAtIndex16   , METH_VARARGS, "SetValueAtIndex16"     },
    { "SetValueAtIndex32"   , (PyCFunction)_PyULIS2Object_AbstractPixel_SetValueAtIndex32   , METH_VARARGS, "SetValueAtIndex32"     },
    { "SetValueAtIndexF"    , (PyCFunction)_PyULIS2Object_AbstractPixel_SetValueAtIndexF    , METH_VARARGS, "SetValueAtIndexF"      },
    { "SetValueAtIndexD"    , (PyCFunction)_PyULIS2Object_AbstractPixel_SetValueAtIndexD    , METH_VARARGS, "SetValueAtIndexD"      },
    { "SetValueAtRawIndex8" , (PyCFunction)_PyULIS2Object_AbstractPixel_SetValueAtRawIndex8 , METH_VARARGS, "SetValueAtRawIndex8"   },
    { "SetValueAtRawIndex16", (PyCFunction)_PyULIS2Object_AbstractPixel_SetValueAtRawIndex16, METH_VARARGS, "SetValueAtRawIndex16"  },
    { "SetValueAtRawIndex32", (PyCFunction)_PyULIS2Object_AbstractPixel_SetValueAtRawIndex32, METH_VARARGS, "SetValueAtRawIndex32"  },
    { "SetValueAtRawIndexF" , (PyCFunction)_PyULIS2Object_AbstractPixel_SetValueAtRawIndexF , METH_VARARGS, "SetValueAtRawIndexF"   },
    { "SetValueAtRawIndexD" , (PyCFunction)_PyULIS2Object_AbstractPixel_SetValueAtRawIndexD , METH_VARARGS, "SetValueAtRawIndexD"   },
    { "SetAlpha8"           , (PyCFunction)_PyULIS2Object_AbstractPixel_SetAlpha8           , METH_VARARGS, "SetAlpha8"             },
    { "SetAlpha16"          , (PyCFunction)_PyULIS2Object_AbstractPixel_SetAlpha16          , METH_VARARGS, "SetAlpha16"            },
    { "SetAlpha32"          , (PyCFunction)_PyULIS2Object_AbstractPixel_SetAlpha32          , METH_VARARGS, "SetAlpha32"            },
    { "SetAlphaF"           , (PyCFunction)_PyULIS2Object_AbstractPixel_SetAlphaF           , METH_VARARGS, "SetAlphaF"             },
    { "SetAlphaD"           , (PyCFunction)_PyULIS2Object_AbstractPixel_SetAlphaD           , METH_VARARGS, "SetAlphaD"             },
    { "A8"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_A8                  , METH_NOARGS, "A8"                     },
    { "Grey8"               , (PyCFunction)_PyULIS2Object_AbstractPixel_Grey8               , METH_NOARGS, "Grey8"                  },
    { "Red8"                , (PyCFunction)_PyULIS2Object_AbstractPixel_Red8                , METH_NOARGS, "Red8"                   },
    { "Green8"              , (PyCFunction)_PyULIS2Object_AbstractPixel_Green8              , METH_NOARGS, "Green8"                 },
    { "Blue8"               , (PyCFunction)_PyULIS2Object_AbstractPixel_Blue8               , METH_NOARGS, "Blue8"                  },
    { "R8"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_R8                  , METH_NOARGS, "R8"                     },
    { "G8"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_G8                  , METH_NOARGS, "G8"                     },
    { "B8"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_B8                  , METH_NOARGS, "B8"                     },
    { "Hue8"                , (PyCFunction)_PyULIS2Object_AbstractPixel_Hue8                , METH_NOARGS, "Hue8"                   },
    { "Saturation8"         , (PyCFunction)_PyULIS2Object_AbstractPixel_Saturation8         , METH_NOARGS, "Saturation8"            },
    { "Lightness8"          , (PyCFunction)_PyULIS2Object_AbstractPixel_Lightness8          , METH_NOARGS, "Lightness8"             },
    { "Value8"              , (PyCFunction)_PyULIS2Object_AbstractPixel_Value8              , METH_NOARGS, "Value8"                 },
    { "Cyan8"               , (PyCFunction)_PyULIS2Object_AbstractPixel_Cyan8               , METH_NOARGS, "Cyan8"                  },
    { "Magenta8"            , (PyCFunction)_PyULIS2Object_AbstractPixel_Magenta8            , METH_NOARGS, "Magenta8"               },
    { "Yellow8"             , (PyCFunction)_PyULIS2Object_AbstractPixel_Yellow8             , METH_NOARGS, "Yellow8"                },
    { "Key8"                , (PyCFunction)_PyULIS2Object_AbstractPixel_Key8                , METH_NOARGS, "Key8"                   },
    { "Luma8"               , (PyCFunction)_PyULIS2Object_AbstractPixel_Luma8               , METH_NOARGS, "Luma8"                  },
    { "U8"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_U8                  , METH_NOARGS, "U8"                     },
    { "V8"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_V8                  , METH_NOARGS, "V8"                     },
    { "K8"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_K8                  , METH_NOARGS, "K8"                     },
    { "X8"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_X8                  , METH_NOARGS, "X8"                     },
    { "Y8"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_Y8                  , METH_NOARGS, "Y8"                     },
    { "Z8"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_Z8                  , METH_NOARGS, "Z8"                     },
    { "L8"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_L8                  , METH_NOARGS, "L8"                     },
    { "a8"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_a8                  , METH_NOARGS, "a8"                     },
    { "b8"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_b8                  , METH_NOARGS, "b8"                     },
    { "x8"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_x8                  , METH_NOARGS, "x8"                     },
    { "y8"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_y8                  , METH_NOARGS, "y8"                     },
    { "A16"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_A16                 , METH_NOARGS, "A16"                    },
    { "Grey16"              , (PyCFunction)_PyULIS2Object_AbstractPixel_Grey16              , METH_NOARGS, "Grey16"                 },
    { "Red16"               , (PyCFunction)_PyULIS2Object_AbstractPixel_Red16               , METH_NOARGS, "Red16"                  },
    { "Green16"             , (PyCFunction)_PyULIS2Object_AbstractPixel_Green16             , METH_NOARGS, "Green16"                },
    { "Blue16"              , (PyCFunction)_PyULIS2Object_AbstractPixel_Blue16              , METH_NOARGS, "Blue16"                 },
    { "R16"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_R16                 , METH_NOARGS, "R16"                    },
    { "G16"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_G16                 , METH_NOARGS, "G16"                    },
    { "B16"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_B16                 , METH_NOARGS, "B16"                    },
    { "Hue16"               , (PyCFunction)_PyULIS2Object_AbstractPixel_Hue16               , METH_NOARGS, "Hue16"                  },
    { "Saturation16"        , (PyCFunction)_PyULIS2Object_AbstractPixel_Saturation16        , METH_NOARGS, "Saturation16"           },
    { "Lightness16"         , (PyCFunction)_PyULIS2Object_AbstractPixel_Lightness16         , METH_NOARGS, "Lightness16"            },
    { "Value16"             , (PyCFunction)_PyULIS2Object_AbstractPixel_Value16             , METH_NOARGS, "Value16"                },
    { "Cyan16"              , (PyCFunction)_PyULIS2Object_AbstractPixel_Cyan16              , METH_NOARGS, "Cyan16"                 },
    { "Magenta16"           , (PyCFunction)_PyULIS2Object_AbstractPixel_Magenta16           , METH_NOARGS, "Magenta16"              },
    { "Yellow16"            , (PyCFunction)_PyULIS2Object_AbstractPixel_Yellow16            , METH_NOARGS, "Yellow16"               },
    { "Key16"               , (PyCFunction)_PyULIS2Object_AbstractPixel_Key16               , METH_NOARGS, "Key16"                  },
    { "Luma16"              , (PyCFunction)_PyULIS2Object_AbstractPixel_Luma16              , METH_NOARGS, "Luma16"                 },
    { "U16"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_U16                 , METH_NOARGS, "U16"                    },
    { "V16"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_V16                 , METH_NOARGS, "V16"                    },
    { "K16"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_K16                 , METH_NOARGS, "K16"                    },
    { "X16"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_X16                 , METH_NOARGS, "X16"                    },
    { "Y16"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_Y16                 , METH_NOARGS, "Y16"                    },
    { "Z16"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_Z16                 , METH_NOARGS, "Z16"                    },
    { "L16"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_L16                 , METH_NOARGS, "L16"                    },
    { "a16"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_a16                 , METH_NOARGS, "a16"                    },
    { "b16"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_b16                 , METH_NOARGS, "b16"                    },
    { "x16"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_x16                 , METH_NOARGS, "x16"                    },
    { "y16"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_y16                 , METH_NOARGS, "y16"                    },
    { "A32"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_A32                 , METH_NOARGS, "A32"                    },
    { "Grey32"              , (PyCFunction)_PyULIS2Object_AbstractPixel_Grey32              , METH_NOARGS, "Grey32"                 },
    { "Red32"               , (PyCFunction)_PyULIS2Object_AbstractPixel_Red32               , METH_NOARGS, "Red32"                  },
    { "Green32"             , (PyCFunction)_PyULIS2Object_AbstractPixel_Green32             , METH_NOARGS, "Green32"                },
    { "Blue32"              , (PyCFunction)_PyULIS2Object_AbstractPixel_Blue32              , METH_NOARGS, "Blue32"                 },
    { "R32"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_R32                 , METH_NOARGS, "R32"                    },
    { "G32"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_G32                 , METH_NOARGS, "G32"                    },
    { "B32"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_B32                 , METH_NOARGS, "B32"                    },
    { "Hue32"               , (PyCFunction)_PyULIS2Object_AbstractPixel_Hue32               , METH_NOARGS, "Hue32"                  },
    { "Saturation32"        , (PyCFunction)_PyULIS2Object_AbstractPixel_Saturation32        , METH_NOARGS, "Saturation32"           },
    { "Lightness32"         , (PyCFunction)_PyULIS2Object_AbstractPixel_Lightness32         , METH_NOARGS, "Lightness32"            },
    { "Value32"             , (PyCFunction)_PyULIS2Object_AbstractPixel_Value32             , METH_NOARGS, "Value32"                },
    { "Cyan32"              , (PyCFunction)_PyULIS2Object_AbstractPixel_Cyan32              , METH_NOARGS, "Cyan32"                 },
    { "Magenta32"           , (PyCFunction)_PyULIS2Object_AbstractPixel_Magenta32           , METH_NOARGS, "Magenta32"              },
    { "Yellow32"            , (PyCFunction)_PyULIS2Object_AbstractPixel_Yellow32            , METH_NOARGS, "Yellow32"               },
    { "Key32"               , (PyCFunction)_PyULIS2Object_AbstractPixel_Key32               , METH_NOARGS, "Key32"                  },
    { "Luma32"              , (PyCFunction)_PyULIS2Object_AbstractPixel_Luma32              , METH_NOARGS, "Luma32"                 },
    { "U32"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_U32                 , METH_NOARGS, "U32"                    },
    { "V32"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_V32                 , METH_NOARGS, "V32"                    },
    { "K32"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_K32                 , METH_NOARGS, "K32"                    },
    { "X32"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_X32                 , METH_NOARGS, "X32"                    },
    { "Y32"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_Y32                 , METH_NOARGS, "Y32"                    },
    { "Z32"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_Z32                 , METH_NOARGS, "Z32"                    },
    { "L32"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_L32                 , METH_NOARGS, "L32"                    },
    { "a32"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_a32                 , METH_NOARGS, "a32"                    },
    { "b32"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_b32                 , METH_NOARGS, "b32"                    },
    { "x32"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_x32                 , METH_NOARGS, "x32"                    },
    { "y32"                 , (PyCFunction)_PyULIS2Object_AbstractPixel_y32                 , METH_NOARGS, "y32"                    },
    { "AF"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_AF                  , METH_NOARGS, "AF"                     },
    { "GreyF"               , (PyCFunction)_PyULIS2Object_AbstractPixel_GreyF               , METH_NOARGS, "GreyF"                  },
    { "RedF"                , (PyCFunction)_PyULIS2Object_AbstractPixel_RedF                , METH_NOARGS, "RedF"                   },
    { "GreenF"              , (PyCFunction)_PyULIS2Object_AbstractPixel_GreenF              , METH_NOARGS, "GreenF"                 },
    { "BlueF"               , (PyCFunction)_PyULIS2Object_AbstractPixel_BlueF               , METH_NOARGS, "BlueF"                  },
    { "RF"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_RF                  , METH_NOARGS, "RF"                     },
    { "GF"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_GF                  , METH_NOARGS, "GF"                     },
    { "BF"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_BF                  , METH_NOARGS, "BF"                     },
    { "HueF"                , (PyCFunction)_PyULIS2Object_AbstractPixel_HueF                , METH_NOARGS, "HueF"                   },
    { "SaturationF"         , (PyCFunction)_PyULIS2Object_AbstractPixel_SaturationF         , METH_NOARGS, "SaturationF"            },
    { "LightnessF"          , (PyCFunction)_PyULIS2Object_AbstractPixel_LightnessF          , METH_NOARGS, "LightnessF"             },
    { "ValueF"              , (PyCFunction)_PyULIS2Object_AbstractPixel_ValueF              , METH_NOARGS, "ValueF"                 },
    { "CyanF"               , (PyCFunction)_PyULIS2Object_AbstractPixel_CyanF               , METH_NOARGS, "CyanF"                  },
    { "MagentaF"            , (PyCFunction)_PyULIS2Object_AbstractPixel_MagentaF            , METH_NOARGS, "MagentaF"               },
    { "YellowF"             , (PyCFunction)_PyULIS2Object_AbstractPixel_YellowF             , METH_NOARGS, "YellowF"                },
    { "KeyF"                , (PyCFunction)_PyULIS2Object_AbstractPixel_KeyF                , METH_NOARGS, "KeyF"                   },
    { "LumaF"               , (PyCFunction)_PyULIS2Object_AbstractPixel_LumaF               , METH_NOARGS, "LumaF"                  },
    { "UF"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_UF                  , METH_NOARGS, "UF"                     },
    { "VF"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_VF                  , METH_NOARGS, "VF"                     },
    { "KF"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_KF                  , METH_NOARGS, "KF"                     },
    { "XF"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_XF                  , METH_NOARGS, "XF"                     },
    { "YF"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_YF                  , METH_NOARGS, "YF"                     },
    { "ZF"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_ZF                  , METH_NOARGS, "ZF"                     },
    { "LF"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_LF                  , METH_NOARGS, "LF"                     },
    { "aF"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_aF                  , METH_NOARGS, "aF"                     },
    { "bF"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_bF                  , METH_NOARGS, "bF"                     },
    { "xF"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_xF                  , METH_NOARGS, "xF"                     },
    { "yF"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_yF                  , METH_NOARGS, "yF"                     },
    { "AD"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_AD                  , METH_NOARGS, "AD"                     },
    { "GreyD"               , (PyCFunction)_PyULIS2Object_AbstractPixel_GreyD               , METH_NOARGS, "GreyD"                  },
    { "RedD"                , (PyCFunction)_PyULIS2Object_AbstractPixel_RedD                , METH_NOARGS, "RedD"                   },
    { "GreenD"              , (PyCFunction)_PyULIS2Object_AbstractPixel_GreenD              , METH_NOARGS, "GreenD"                 },
    { "BlueD"               , (PyCFunction)_PyULIS2Object_AbstractPixel_BlueD               , METH_NOARGS, "BlueD"                  },
    { "RD"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_RD                  , METH_NOARGS, "RD"                     },
    { "GD"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_GD                  , METH_NOARGS, "GD"                     },
    { "BD"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_BD                  , METH_NOARGS, "BD"                     },
    { "HueD"                , (PyCFunction)_PyULIS2Object_AbstractPixel_HueD                , METH_NOARGS, "HueD"                   },
    { "SaturationD"         , (PyCFunction)_PyULIS2Object_AbstractPixel_SaturationD         , METH_NOARGS, "SaturationD"            },
    { "LightnessD"          , (PyCFunction)_PyULIS2Object_AbstractPixel_LightnessD          , METH_NOARGS, "LightnessD"             },
    { "ValueD"              , (PyCFunction)_PyULIS2Object_AbstractPixel_ValueD              , METH_NOARGS, "ValueD"                 },
    { "CyanD"               , (PyCFunction)_PyULIS2Object_AbstractPixel_CyanD               , METH_NOARGS, "CyanD"                  },
    { "MagentaD"            , (PyCFunction)_PyULIS2Object_AbstractPixel_MagentaD            , METH_NOARGS, "MagentaD"               },
    { "YellowD"             , (PyCFunction)_PyULIS2Object_AbstractPixel_YellowD             , METH_NOARGS, "YellowD"                },
    { "KeyD"                , (PyCFunction)_PyULIS2Object_AbstractPixel_KeyD                , METH_NOARGS, "KeyD"                   },
    { "LumaD"               , (PyCFunction)_PyULIS2Object_AbstractPixel_LumaD               , METH_NOARGS, "LumaD"                  },
    { "UD"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_UD                  , METH_NOARGS, "UD"                     },
    { "VD"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_VD                  , METH_NOARGS, "VD"                     },
    { "KD"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_KD                  , METH_NOARGS, "KD"                     },
    { "XD"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_XD                  , METH_NOARGS, "XD"                     },
    { "YD"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_YD                  , METH_NOARGS, "YD"                     },
    { "ZD"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_ZD                  , METH_NOARGS, "ZD"                     },
    { "LD"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_LD                  , METH_NOARGS, "LD"                     },
    { "aD"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_aD                  , METH_NOARGS, "aD"                     },
    { "bD"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_bD                  , METH_NOARGS, "bD"                     },
    { "xD"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_xD                  , METH_NOARGS, "xD"                     },
    { "yD"                  , (PyCFunction)_PyULIS2Object_AbstractPixel_yD                  , METH_NOARGS, "yD"                     },
    { "SetA8"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetA8               , METH_VARARGS, "SetA8"                 },
    { "SetGrey8"            , (PyCFunction)_PyULIS2Object_AbstractPixel_SetGrey8            , METH_VARARGS, "SetGrey8"              },
    { "SetRed8"             , (PyCFunction)_PyULIS2Object_AbstractPixel_SetRed8             , METH_VARARGS, "SetRed8"               },
    { "SetGreen8"           , (PyCFunction)_PyULIS2Object_AbstractPixel_SetGreen8           , METH_VARARGS, "SetGreen8"             },
    { "SetBlue8"            , (PyCFunction)_PyULIS2Object_AbstractPixel_SetBlue8            , METH_VARARGS, "SetBlue8"              },
    { "SetR8"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetR8               , METH_VARARGS, "SetR8"                 },
    { "SetG8"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetG8               , METH_VARARGS, "SetG8"                 },
    { "SetB8"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetB8               , METH_VARARGS, "SetB8"                 },
    { "SetHue8"             , (PyCFunction)_PyULIS2Object_AbstractPixel_SetHue8             , METH_VARARGS, "SetHue8"               },
    { "SetSaturation8"      , (PyCFunction)_PyULIS2Object_AbstractPixel_SetSaturation8      , METH_VARARGS, "SetSaturation8"        },
    { "SetLightness8"       , (PyCFunction)_PyULIS2Object_AbstractPixel_SetLightness8       , METH_VARARGS, "SetLightness8"         },
    { "SetValue8"           , (PyCFunction)_PyULIS2Object_AbstractPixel_SetValue8           , METH_VARARGS, "SetValue8"             },
    { "SetCyan8"            , (PyCFunction)_PyULIS2Object_AbstractPixel_SetCyan8            , METH_VARARGS, "SetCyan8"              },
    { "SetMagenta8"         , (PyCFunction)_PyULIS2Object_AbstractPixel_SetMagenta8         , METH_VARARGS, "SetMagenta8"           },
    { "SetYellow8"          , (PyCFunction)_PyULIS2Object_AbstractPixel_SetYellow8          , METH_VARARGS, "SetYellow8"            },
    { "SetKey8"             , (PyCFunction)_PyULIS2Object_AbstractPixel_SetKey8             , METH_VARARGS, "SetKey8"               },
    { "SetLuma8"            , (PyCFunction)_PyULIS2Object_AbstractPixel_SetLuma8            , METH_VARARGS, "SetLuma8"              },
    { "SetU8"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetU8               , METH_VARARGS, "SetU8"                 },
    { "SetV8"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetV8               , METH_VARARGS, "SetV8"                 },
    { "SetK8"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetK8               , METH_VARARGS, "SetK8"                 },
    { "SetX8"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetX8               , METH_VARARGS, "SetX8"                 },
    { "SetY8"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetY8               , METH_VARARGS, "SetY8"                 },
    { "SetZ8"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetZ8               , METH_VARARGS, "SetZ8"                 },
    { "SetL8"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetL8               , METH_VARARGS, "SetL8"                 },
    { "Seta8"               , (PyCFunction)_PyULIS2Object_AbstractPixel_Seta8               , METH_VARARGS, "Seta8"                 },
    { "Setb8"               , (PyCFunction)_PyULIS2Object_AbstractPixel_Setb8               , METH_VARARGS, "Setb8"                 },
    { "Setx8"               , (PyCFunction)_PyULIS2Object_AbstractPixel_Setx8               , METH_VARARGS, "Setx8"                 },
    { "Sety8"               , (PyCFunction)_PyULIS2Object_AbstractPixel_Sety8               , METH_VARARGS, "Sety8"                 },
    { "SetA16"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetA16              , METH_VARARGS, "SetA16"                },
    { "SetGrey16"           , (PyCFunction)_PyULIS2Object_AbstractPixel_SetGrey16           , METH_VARARGS, "SetGrey16"             },
    { "SetRed16"            , (PyCFunction)_PyULIS2Object_AbstractPixel_SetRed16            , METH_VARARGS, "SetRed16"              },
    { "SetGreen16"          , (PyCFunction)_PyULIS2Object_AbstractPixel_SetGreen16          , METH_VARARGS, "SetGreen16"            },
    { "SetBlue16"           , (PyCFunction)_PyULIS2Object_AbstractPixel_SetBlue16           , METH_VARARGS, "SetBlue16"             },
    { "SetR16"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetR16              , METH_VARARGS, "SetR16"                },
    { "SetG16"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetG16              , METH_VARARGS, "SetG16"                },
    { "SetB16"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetB16              , METH_VARARGS, "SetB16"                },
    { "SetHue16"            , (PyCFunction)_PyULIS2Object_AbstractPixel_SetHue16            , METH_VARARGS, "SetHue16"              },
    { "SetSaturation16"     , (PyCFunction)_PyULIS2Object_AbstractPixel_SetSaturation16     , METH_VARARGS, "SetSaturation16"       },
    { "SetLightness16"      , (PyCFunction)_PyULIS2Object_AbstractPixel_SetLightness16      , METH_VARARGS, "SetLightness16"        },
    { "SetValue16"          , (PyCFunction)_PyULIS2Object_AbstractPixel_SetValue16          , METH_VARARGS, "SetValue16"            },
    { "SetCyan16"           , (PyCFunction)_PyULIS2Object_AbstractPixel_SetCyan16           , METH_VARARGS, "SetCyan16"             },
    { "SetMagenta16"        , (PyCFunction)_PyULIS2Object_AbstractPixel_SetMagenta16        , METH_VARARGS, "SetMagenta16"          },
    { "SetYellow16"         , (PyCFunction)_PyULIS2Object_AbstractPixel_SetYellow16         , METH_VARARGS, "SetYellow16"           },
    { "SetKey16"            , (PyCFunction)_PyULIS2Object_AbstractPixel_SetKey16            , METH_VARARGS, "SetKey16"              },
    { "SetLuma16"           , (PyCFunction)_PyULIS2Object_AbstractPixel_SetLuma16           , METH_VARARGS, "SetLuma16"             },
    { "SetU16"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetU16              , METH_VARARGS, "SetU16"                },
    { "SetV16"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetV16              , METH_VARARGS, "SetV16"                },
    { "SetK16"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetK16              , METH_VARARGS, "SetK16"                },
    { "SetX16"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetX16              , METH_VARARGS, "SetX16"                },
    { "SetY16"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetY16              , METH_VARARGS, "SetY16"                },
    { "SetZ16"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetZ16              , METH_VARARGS, "SetZ16"                },
    { "SetL16"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetL16              , METH_VARARGS, "SetL16"                },
    { "Seta16"              , (PyCFunction)_PyULIS2Object_AbstractPixel_Seta16              , METH_VARARGS, "Seta16"                },
    { "Setb16"              , (PyCFunction)_PyULIS2Object_AbstractPixel_Setb16              , METH_VARARGS, "Setb16"                },
    { "Setx16"              , (PyCFunction)_PyULIS2Object_AbstractPixel_Setx16              , METH_VARARGS, "Setx16"                },
    { "Sety16"              , (PyCFunction)_PyULIS2Object_AbstractPixel_Sety16              , METH_VARARGS, "Sety16"                },
    { "SetA32"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetA32              , METH_VARARGS, "SetA32"                },
    { "SetGrey32"           , (PyCFunction)_PyULIS2Object_AbstractPixel_SetGrey32           , METH_VARARGS, "SetGrey32"             },
    { "SetRed32"            , (PyCFunction)_PyULIS2Object_AbstractPixel_SetRed32            , METH_VARARGS, "SetRed32"              },
    { "SetGreen32"          , (PyCFunction)_PyULIS2Object_AbstractPixel_SetGreen32          , METH_VARARGS, "SetGreen32"            },
    { "SetBlue32"           , (PyCFunction)_PyULIS2Object_AbstractPixel_SetBlue32           , METH_VARARGS, "SetBlue32"             },
    { "SetR32"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetR32              , METH_VARARGS, "SetR32"                },
    { "SetG32"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetG32              , METH_VARARGS, "SetG32"                },
    { "SetB32"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetB32              , METH_VARARGS, "SetB32"                },
    { "SetHue32"            , (PyCFunction)_PyULIS2Object_AbstractPixel_SetHue32            , METH_VARARGS, "SetHue32"              },
    { "SetSaturation32"     , (PyCFunction)_PyULIS2Object_AbstractPixel_SetSaturation32     , METH_VARARGS, "SetSaturation32"       },
    { "SetLightness32"      , (PyCFunction)_PyULIS2Object_AbstractPixel_SetLightness32      , METH_VARARGS, "SetLightness32"        },
    { "SetValue32"          , (PyCFunction)_PyULIS2Object_AbstractPixel_SetValue32          , METH_VARARGS, "SetValue32"            },
    { "SetCyan32"           , (PyCFunction)_PyULIS2Object_AbstractPixel_SetCyan32           , METH_VARARGS, "SetCyan32"             },
    { "SetMagenta32"        , (PyCFunction)_PyULIS2Object_AbstractPixel_SetMagenta32        , METH_VARARGS, "SetMagenta32"          },
    { "SetYellow32"         , (PyCFunction)_PyULIS2Object_AbstractPixel_SetYellow32         , METH_VARARGS, "SetYellow32"           },
    { "SetKey32"            , (PyCFunction)_PyULIS2Object_AbstractPixel_SetKey32            , METH_VARARGS, "SetKey32"              },
    { "SetLuma32"           , (PyCFunction)_PyULIS2Object_AbstractPixel_SetLuma32           , METH_VARARGS, "SetLuma32"             },
    { "SetU32"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetU32              , METH_VARARGS, "SetU32"                },
    { "SetV32"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetV32              , METH_VARARGS, "SetV32"                },
    { "SetK32"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetK32              , METH_VARARGS, "SetK32"                },
    { "SetX32"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetX32              , METH_VARARGS, "SetX32"                },
    { "SetY32"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetY32              , METH_VARARGS, "SetY32"                },
    { "SetZ32"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetZ32              , METH_VARARGS, "SetZ32"                },
    { "SetL32"              , (PyCFunction)_PyULIS2Object_AbstractPixel_SetL32              , METH_VARARGS, "SetL32"                },
    { "Seta32"              , (PyCFunction)_PyULIS2Object_AbstractPixel_Seta32              , METH_VARARGS, "Seta32"                },
    { "Setb32"              , (PyCFunction)_PyULIS2Object_AbstractPixel_Setb32              , METH_VARARGS, "Setb32"                },
    { "Setx32"              , (PyCFunction)_PyULIS2Object_AbstractPixel_Setx32              , METH_VARARGS, "Setx32"                },
    { "Sety32"              , (PyCFunction)_PyULIS2Object_AbstractPixel_Sety32              , METH_VARARGS, "Sety32"                },
    { "SetAF"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetAF               , METH_VARARGS, "SetAF"                 },
    { "SetGreyF"            , (PyCFunction)_PyULIS2Object_AbstractPixel_SetGreyF            , METH_VARARGS, "SetGreyF"              },
    { "SetRedF"             , (PyCFunction)_PyULIS2Object_AbstractPixel_SetRedF             , METH_VARARGS, "SetRedF"               },
    { "SetGreenF"           , (PyCFunction)_PyULIS2Object_AbstractPixel_SetGreenF           , METH_VARARGS, "SetGreenF"             },
    { "SetBlueF"            , (PyCFunction)_PyULIS2Object_AbstractPixel_SetBlueF            , METH_VARARGS, "SetBlueF"              },
    { "SetRF"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetRF               , METH_VARARGS, "SetRF"                 },
    { "SetGF"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetGF               , METH_VARARGS, "SetGF"                 },
    { "SetBF"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetBF               , METH_VARARGS, "SetBF"                 },
    { "SetHueF"             , (PyCFunction)_PyULIS2Object_AbstractPixel_SetHueF             , METH_VARARGS, "SetHueF"               },
    { "SetSaturationF"      , (PyCFunction)_PyULIS2Object_AbstractPixel_SetSaturationF      , METH_VARARGS, "SetSaturationF"        },
    { "SetLightnessF"       , (PyCFunction)_PyULIS2Object_AbstractPixel_SetLightnessF       , METH_VARARGS, "SetLightnessF"         },
    { "SetValueF"           , (PyCFunction)_PyULIS2Object_AbstractPixel_SetValueF           , METH_VARARGS, "SetValueF"             },
    { "SetCyanF"            , (PyCFunction)_PyULIS2Object_AbstractPixel_SetCyanF            , METH_VARARGS, "SetCyanF"              },
    { "SetMagentaF"         , (PyCFunction)_PyULIS2Object_AbstractPixel_SetMagentaF         , METH_VARARGS, "SetMagentaF"           },
    { "SetYellowF"          , (PyCFunction)_PyULIS2Object_AbstractPixel_SetYellowF          , METH_VARARGS, "SetYellowF"            },
    { "SetKeyF"             , (PyCFunction)_PyULIS2Object_AbstractPixel_SetKeyF             , METH_VARARGS, "SetKeyF"               },
    { "SetLumaF"            , (PyCFunction)_PyULIS2Object_AbstractPixel_SetLumaF            , METH_VARARGS, "SetLumaF"              },
    { "SetUF"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetUF               , METH_VARARGS, "SetUF"                 },
    { "SetVF"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetVF               , METH_VARARGS, "SetVF"                 },
    { "SetKF"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetKF               , METH_VARARGS, "SetKF"                 },
    { "SetXF"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetXF               , METH_VARARGS, "SetXF"                 },
    { "SetYF"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetYF               , METH_VARARGS, "SetYF"                 },
    { "SetZF"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetZF               , METH_VARARGS, "SetZF"                 },
    { "SetLF"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetLF               , METH_VARARGS, "SetLF"                 },
    { "SetaF"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetaF               , METH_VARARGS, "SetaF"                 },
    { "SetbF"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetbF               , METH_VARARGS, "SetbF"                 },
    { "SetxF"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetxF               , METH_VARARGS, "SetxF"                 },
    { "SetyF"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetyF               , METH_VARARGS, "SetyF"                 },
    { "SetAD"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetAD               , METH_VARARGS, "SetAD"                 },
    { "SetGreyD"            , (PyCFunction)_PyULIS2Object_AbstractPixel_SetGreyD            , METH_VARARGS, "SetGreyD"              },
    { "SetRedD"             , (PyCFunction)_PyULIS2Object_AbstractPixel_SetRedD             , METH_VARARGS, "SetRedD"               },
    { "SetGreenD"           , (PyCFunction)_PyULIS2Object_AbstractPixel_SetGreenD           , METH_VARARGS, "SetGreenD"             },
    { "SetBlueD"            , (PyCFunction)_PyULIS2Object_AbstractPixel_SetBlueD            , METH_VARARGS, "SetBlueD"              },
    { "SetRD"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetRD               , METH_VARARGS, "SetRD"                 },
    { "SetGD"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetGD               , METH_VARARGS, "SetGD"                 },
    { "SetBD"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetBD               , METH_VARARGS, "SetBD"                 },
    { "SetHueD"             , (PyCFunction)_PyULIS2Object_AbstractPixel_SetHueD             , METH_VARARGS, "SetHueD"               },
    { "SetSaturationD"      , (PyCFunction)_PyULIS2Object_AbstractPixel_SetSaturationD      , METH_VARARGS, "SetSaturationD"        },
    { "SetLightnessD"       , (PyCFunction)_PyULIS2Object_AbstractPixel_SetLightnessD       , METH_VARARGS, "SetLightnessD"         },
    { "SetValueD"           , (PyCFunction)_PyULIS2Object_AbstractPixel_SetValueD           , METH_VARARGS, "SetValueD"             },
    { "SetCyanD"            , (PyCFunction)_PyULIS2Object_AbstractPixel_SetCyanD            , METH_VARARGS, "SetCyanD"              },
    { "SetMagentaD"         , (PyCFunction)_PyULIS2Object_AbstractPixel_SetMagentaD         , METH_VARARGS, "SetMagentaD"           },
    { "SetYellowD"          , (PyCFunction)_PyULIS2Object_AbstractPixel_SetYellowD          , METH_VARARGS, "SetYellowD"            },
    { "SetKeyD"             , (PyCFunction)_PyULIS2Object_AbstractPixel_SetKeyD             , METH_VARARGS, "SetKeyD"               },
    { "SetLumaD"            , (PyCFunction)_PyULIS2Object_AbstractPixel_SetLumaD            , METH_VARARGS, "SetLumaD"              },
    { "SetUD"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetUD               , METH_VARARGS, "SetUD"                 },
    { "SetVD"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetVD               , METH_VARARGS, "SetVD"                 },
    { "SetKD"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetKD               , METH_VARARGS, "SetKD"                 },
    { "SetXD"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetXD               , METH_VARARGS, "SetXD"                 },
    { "SetYD"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetYD               , METH_VARARGS, "SetYD"                 },
    { "SetZD"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetZD               , METH_VARARGS, "SetZD"                 },
    { "SetLD"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetLD               , METH_VARARGS, "SetLD"                 },
    { "SetaD"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetaD               , METH_VARARGS, "SetaD"                 },
    { "SetbD"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetbD               , METH_VARARGS, "SetbD"                 },
    { "SetxD"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetxD               , METH_VARARGS, "SetxD"                 },
    { "SetyD"               , (PyCFunction)_PyULIS2Object_AbstractPixel_SetyD               , METH_VARARGS, "SetyD"                 },
    { NULL } // Sentinel
};

/////////////////////////////////////////////////////
/// Meta Types
static PyTypeObject IPixel = {
PyVarObject_HEAD_INIT(NULL, 0)
    "PyULIS2.IPixel", /* tp_name */
    sizeof( _PyULIS2Object_AbstractPixel ), /* tp_basicsize */
    0, /* tp_itemsize */
    (destructor)_PyULIS2Object_AbstractPixel_dealloc, /* tp_dealloc */
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
    _PyULIS2Object_AbstractPixel_methods, /* tp_methods */
    0, /* tp_members */
    0, /* tp_getset */
    0, /* tp_base */
    0, /* tp_dict */
    0, /* tp_descr_get */
    0, /* tp_descr_set */
    0, /* tp_dictoffset */
    (initproc)_PyULIS2Object_AbstractPixel_init, /* tp_init */
    0, /* tp_alloc */
    _PyULIS2Object_AbstractPixel_new, /* tp_new */
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

