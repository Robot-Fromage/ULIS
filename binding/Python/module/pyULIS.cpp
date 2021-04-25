// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         pyULIS.cpp
* @author       Clement Berthaud
* @brief        Binding of ULIS for Python.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include <pybind11/pybind11.h>
#include "pyTypes/Block.h"
using namespace ::ULIS;
using namespace pybind11::literals;
namespace py = pybind11;

#define PYULIS_MODULE_NAME pyULIS ## ULIS_VERSION_MAJOR
PYBIND11_MODULE( PYULIS_MODULE_NAME, m ) {
    m.doc() = "pyULIS module, a python binding for ULIS.";
}
