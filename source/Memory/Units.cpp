// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Units.cpp
* @author       Clement Berthaud
* @brief        This file provides the declaration for Units tools.
* @license      Please refer to LICENSE.md
*/
#include "Memory/Units.h"

ULIS_NAMESPACE_BEGIN
namespace units_literals {
/////////////////////////////////////////////////////
/// Used defined literals
// double
// User defined literals - SI - bit - double
bit_t operator "" _bit( long double i ) { return  bit_t( i ); }
kbit_t operator "" _kbit( long double i ) { return  kbit_t( i ); }
Mbit_t operator "" _Mbit( long double i ) { return  Mbit_t( i ); }
Gbit_t operator "" _Gbit( long double i ) { return  Gbit_t( i ); }
Tbit_t operator "" _Tbit( long double i ) { return  Tbit_t( i ); }
Pbit_t operator "" _Pbit( long double i ) { return  Pbit_t( i ); }
Ebit_t operator "" _Ebit( long double i ) { return  Ebit_t( i ); }
Zbit_t operator "" _Zbit( long double i ) { return  Zbit_t( i ); }
Ybit_t operator "" _Ybit( long double i ) { return  Ybit_t( i ); }
// User defined literals - SI - byte - double
B_t operator "" _B( long double i ) { return  B_t( i ); }
kB_t operator "" _kB( long double i ) { return  kB_t( i ); }
MB_t operator "" _MB( long double i ) { return  MB_t( i ); }
GB_t operator "" _GB( long double i ) { return  GB_t( i ); }
TB_t operator "" _TB( long double i ) { return  TB_t( i ); }
PB_t operator "" _PB( long double i ) { return  PB_t( i ); }
EB_t operator "" _EB( long double i ) { return  EB_t( i ); }
ZB_t operator "" _ZB( long double i ) { return  ZB_t( i ); }
YB_t operator "" _YB( long double i ) { return  YB_t( i ); }
// User defined literals - SI - octet - double
o_t operator "" _o( long double i ) { return  o_t( i ); }
ko_t operator "" _ko( long double i ) { return  ko_t( i ); }
Mo_t operator "" _Mo( long double i ) { return  Mo_t( i ); }
Go_t operator "" _Go( long double i ) { return  Go_t( i ); }
To_t operator "" _To( long double i ) { return  To_t( i ); }
Po_t operator "" _Po( long double i ) { return  Po_t( i ); }
Eo_t operator "" _Eo( long double i ) { return  Eo_t( i ); }
Zo_t operator "" _Zo( long double i ) { return  Zo_t( i ); }
Yo_t operator "" _Yo( long double i ) { return  Yo_t( i ); }
// User defined literals - IEC - bit - double
Kibit_t operator "" _Kibit( long double i ) { return  Kibit_t( i ); }
Mibit_t operator "" _Mibit( long double i ) { return  Mibit_t( i ); }
Gibit_t operator "" _Gibit( long double i ) { return  Gibit_t( i ); }
Tibit_t operator "" _Tibit( long double i ) { return  Tibit_t( i ); }
Pibit_t operator "" _Pibit( long double i ) { return  Pibit_t( i ); }
Eibit_t operator "" _Eibit( long double i ) { return  Eibit_t( i ); }
Zibit_t operator "" _Zibit( long double i ) { return  Zibit_t( i ); }
Yibit_t operator "" _Yibit( long double i ) { return  Yibit_t( i ); }
// User defined literals - IEC - byte - double
KiB_t operator "" _KiB( long double i ) { return  KiB_t( i ); }
MiB_t operator "" _MiB( long double i ) { return  MiB_t( i ); }
GiB_t operator "" _GiB( long double i ) { return  GiB_t( i ); }
TiB_t operator "" _TiB( long double i ) { return  TiB_t( i ); }
PiB_t operator "" _PiB( long double i ) { return  PiB_t( i ); }
EiB_t operator "" _EiB( long double i ) { return  EiB_t( i ); }
ZiB_t operator "" _ZiB( long double i ) { return  ZiB_t( i ); }
YiB_t operator "" _YiB( long double i ) { return  YiB_t( i ); }
// User defined literals - IEC - octet - double
Kio_t operator "" _Kio( long double i ) { return  Kio_t( i ); }
Mio_t operator "" _Mio( long double i ) { return  Mio_t( i ); }
Gio_t operator "" _Gio( long double i ) { return  Gio_t( i ); }
Tio_t operator "" _Tio( long double i ) { return  Tio_t( i ); }
Pio_t operator "" _Pio( long double i ) { return  Pio_t( i ); }
Eio_t operator "" _Eio( long double i ) { return  Eio_t( i ); }
Zio_t operator "" _Zio( long double i ) { return  Zio_t( i ); }
Yio_t operator "" _Yio( long double i ) { return  Yio_t( i ); }

// uint64_literal
// User defined literals - SI - bit - uint64_literal
bit_t operator "" _bit( uint64_literal i ) { return  bit_t( ( i ) ); }
kbit_t operator "" _kbit( uint64_literal i ) { return  kbit_t( ( i ) ); }
Mbit_t operator "" _Mbit( uint64_literal i ) { return  Mbit_t( ( i ) ); }
Gbit_t operator "" _Gbit( uint64_literal i ) { return  Gbit_t( ( i ) ); }
Tbit_t operator "" _Tbit( uint64_literal i ) { return  Tbit_t( ( i ) ); }
Pbit_t operator "" _Pbit( uint64_literal i ) { return  Pbit_t( ( i ) ); }
Ebit_t operator "" _Ebit( uint64_literal i ) { return  Ebit_t( ( i ) ); }
Zbit_t operator "" _Zbit( uint64_literal i ) { return  Zbit_t( ( i ) ); }
Ybit_t operator "" _Ybit( uint64_literal i ) { return  Ybit_t( ( i ) ); }
// User defined literals - SI - byte - uint64_literal
B_t operator "" _B( uint64_literal i ) { return  B_t( ( i ) ); }
kB_t operator "" _kB( uint64_literal i ) { return  kB_t( ( i ) ); }
MB_t operator "" _MB( uint64_literal i ) { return  MB_t( ( i ) ); }
GB_t operator "" _GB( uint64_literal i ) { return  GB_t( ( i ) ); }
TB_t operator "" _TB( uint64_literal i ) { return  TB_t( ( i ) ); }
PB_t operator "" _PB( uint64_literal i ) { return  PB_t( ( i ) ); }
EB_t operator "" _EB( uint64_literal i ) { return  EB_t( ( i ) ); }
ZB_t operator "" _ZB( uint64_literal i ) { return  ZB_t( ( i ) ); }
YB_t operator "" _YB( uint64_literal i ) { return  YB_t( ( i ) ); }
// User defined literals - SI - octet - uint64_literal
o_t operator "" _o( uint64_literal i ) { return  o_t( ( i ) ); }
ko_t operator "" _ko( uint64_literal i ) { return  ko_t( ( i ) ); }
Mo_t operator "" _Mo( uint64_literal i ) { return  Mo_t( ( i ) ); }
Go_t operator "" _Go( uint64_literal i ) { return  Go_t( ( i ) ); }
To_t operator "" _To( uint64_literal i ) { return  To_t( ( i ) ); }
Po_t operator "" _Po( uint64_literal i ) { return  Po_t( ( i ) ); }
Eo_t operator "" _Eo( uint64_literal i ) { return  Eo_t( ( i ) ); }
Zo_t operator "" _Zo( uint64_literal i ) { return  Zo_t( ( i ) ); }
Yo_t operator "" _Yo( uint64_literal i ) { return  Yo_t( ( i ) ); }
// User defined literals - IEC - bit - uint64_literal
Kibit_t operator "" _Kibit( uint64_literal i ) { return  Kibit_t( ( i ) ); }
Mibit_t operator "" _Mibit( uint64_literal i ) { return  Mibit_t( ( i ) ); }
Gibit_t operator "" _Gibit( uint64_literal i ) { return  Gibit_t( ( i ) ); }
Tibit_t operator "" _Tibit( uint64_literal i ) { return  Tibit_t( ( i ) ); }
Pibit_t operator "" _Pibit( uint64_literal i ) { return  Pibit_t( ( i ) ); }
Eibit_t operator "" _Eibit( uint64_literal i ) { return  Eibit_t( ( i ) ); }
Zibit_t operator "" _Zibit( uint64_literal i ) { return  Zibit_t( ( i ) ); }
Yibit_t operator "" _Yibit( uint64_literal i ) { return  Yibit_t( ( i ) ); }
// User defined literals - IEC - byte - uint64_literal
KiB_t operator "" _KiB( uint64_literal i ) { return  KiB_t( ( i ) ); }
MiB_t operator "" _MiB( uint64_literal i ) { return  MiB_t( ( i ) ); }
GiB_t operator "" _GiB( uint64_literal i ) { return  GiB_t( ( i ) ); }
TiB_t operator "" _TiB( uint64_literal i ) { return  TiB_t( ( i ) ); }
PiB_t operator "" _PiB( uint64_literal i ) { return  PiB_t( ( i ) ); }
EiB_t operator "" _EiB( uint64_literal i ) { return  EiB_t( ( i ) ); }
ZiB_t operator "" _ZiB( uint64_literal i ) { return  ZiB_t( ( i ) ); }
YiB_t operator "" _YiB( uint64_literal i ) { return  YiB_t( ( i ) ); }
// User defined literals - IEC - octet - uint64_literal
Kio_t operator "" _Kio( uint64_literal i ) { return  Kio_t( ( i ) ); }
Mio_t operator "" _Mio( uint64_literal i ) { return  Mio_t( ( i ) ); }
Gio_t operator "" _Gio( uint64_literal i ) { return  Gio_t( ( i ) ); }
Tio_t operator "" _Tio( uint64_literal i ) { return  Tio_t( ( i ) ); }
Pio_t operator "" _Pio( uint64_literal i ) { return  Pio_t( ( i ) ); }
Eio_t operator "" _Eio( uint64_literal i ) { return  Eio_t( ( i ) ); }
Zio_t operator "" _Zio( uint64_literal i ) { return  Zio_t( ( i ) ); }
Yio_t operator "" _Yio( uint64_literal i ) { return  Yio_t( ( i ) ); }

} // namespace units_literals

ULIS_NAMESPACE_END

