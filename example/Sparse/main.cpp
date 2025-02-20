// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         main.cpp
* @author       Clement Berthaud
* @brief        Sparse terminal curses debug application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include <curses.h>
#include <windows.h>
using namespace ::ULIS;

#define CCOLOR( x ) short( x / 255.f * 1000 )
#define CCOLOR3( x, y, z ) CCOLOR( x ), CCOLOR( y ), CCOLOR( z )

const char* keys[] = {
      "Vendor_AMD"
    , "Vendor_Intel"
    , "OS_x64"
    , "OS_AVX"
    , "OS_AVX512"
    , "HW_MMX"
    , "HW_x64"
    , "HW_ABM"
    , "HW_RDRAND"
    , "HW_BMI1"
    , "HW_BMI2"
    , "HW_ADX"
    , "HW_PREFETCHWT1 "
    , "HW_MPX"
    , "HW_SSE"
    , "HW_SSE2"
    , "HW_SSE3"
    , "HW_SSSE3"
    , "HW_SSE41"
    , "HW_SSE42"
    , "HW_SSE4a"
    , "HW_AES"
    , "HW_SHA"
    , "HW_AVX"
    , "HW_XOP"
    , "HW_FMA3"
    , "HW_FMA4"
    , "HW_AVX2"
    , "HW_AVX512_F"
    , "HW_AVX512_PF"
    , "HW_AVX512_ER"
    , "HW_AVX512_CD"
    , "HW_AVX512_VL"
    , "HW_AVX512_BW"
    , "HW_AVX512_DQ"
    , "HW_AVX512_IFMA "
    , "HW_AVX512_VBMI "
};
constexpr auto cpuinfosize = sizeof( keys ) / sizeof( const char* );
const char* out[] = { "NO", "YES" };
const char* values[] = {
      out[ FCPUInfo::IsHardwareAMD()          ]
    , out[ FCPUInfo::IsHardwareIntel()        ]
    , out[ FCPUInfo::IsOSx64()                ]
    , out[ FCPUInfo::HasOSAVX()               ]
    , out[ FCPUInfo::HasOSAVX512()            ]
    , out[ FCPUInfo::HasHardwarex64()         ]
    , out[ FCPUInfo::HasHardwareMMX()         ]
    , out[ FCPUInfo::HasHardwareABM()         ]
    , out[ FCPUInfo::HasHardwareRDRAND()      ]
    , out[ FCPUInfo::HasHardwareBMI1()        ]
    , out[ FCPUInfo::HasHardwareBMI2()        ]
    , out[ FCPUInfo::HasHardwareADX()         ]
    , out[ FCPUInfo::HasHardwarePREFETCHWT1() ]
    , out[ FCPUInfo::HasHardwareMPX()         ]
    , out[ FCPUInfo::HasHardwareSSE()         ]
    , out[ FCPUInfo::HasHardwareSSE2()        ]
    , out[ FCPUInfo::HasHardwareSSE3()        ]
    , out[ FCPUInfo::HasHardwareSSSE3()       ]
    , out[ FCPUInfo::HasHardwareSSE41()       ]
    , out[ FCPUInfo::HasHardwareSSE42()       ]
    , out[ FCPUInfo::HasHardwareSSE4a()       ]
    , out[ FCPUInfo::HasHardwareAES()         ]
    , out[ FCPUInfo::HasHardwareSHA()         ]
    , out[ FCPUInfo::HasHardwareAVX()         ]
    , out[ FCPUInfo::HasHardwareXOP()         ]
    , out[ FCPUInfo::HasHardwareFMA3()        ]
    , out[ FCPUInfo::HasHardwareFMA4()        ]
    , out[ FCPUInfo::HasHardwareAVX2()        ]
    , out[ FCPUInfo::HasHardwareAVX512_F()    ]
    , out[ FCPUInfo::HasHardwareAVX512_PF()   ]
    , out[ FCPUInfo::HasHardwareAVX512_ER()   ]
    , out[ FCPUInfo::HasHardwareAVX512_CD()   ]
    , out[ FCPUInfo::HasHardwareAVX512_VL()   ]
    , out[ FCPUInfo::HasHardwareAVX512_BW()   ]
    , out[ FCPUInfo::HasHardwareAVX512_DQ()   ]
    , out[ FCPUInfo::HasHardwareAVX512_IFMA() ]
    , out[ FCPUInfo::HasHardwareAVX512_VBMI() ]
};

int
main( int argc, char *argv[] ) {
    // Disable console resizing by user
    // Console Title
    SetConsoleTitle( L"ULIS" ) ;
    HWND hwnd = GetConsoleWindow();
    SetWindowLong( hwnd, GWL_STYLE, GetWindowLong( hwnd, GWL_STYLE ) /* & ~WS_CAPTION */ & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX );

    //// Resize console programatically
    //HWND hwnd = GetConsoleWindow();
    //if( hwnd != NULL ){ SetWindowPos(hwnd ,0,0,0 ,1200, 695, SWP_SHOWWINDOW| SWP_NOMOVE); }

    initscr(); // Start curses mode

    // For maximized term:
    //resize_term(2000, 2000);
    //int _rows, _cols;
    //getmaxyx(stdscr, _rows, _cols);
    //resize_term(_rows, _cols );
    //getmaxyx(stdscr, _rows, _cols);

    raw(); // Disable line buffering and interrupt, quit, flow control keys
    keypad( stdscr, true ); // Intercept function keys, and arrows
    noecho(); // Don't type in terminal during getch, intercept charcode
    curs_set( 0 ); // 0: hidden, 1: visible, 2: very visible
    start_color(); // Init color mode
    mouseinterval(0);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    nodelay( stdscr, TRUE );
    wtimeout( stdscr, 1 );
    printf( "\033[?1003h\n" ); // Makes the terminal report mouse movement events

    int w, h;
    getmaxyx( stdscr, h, w );

    // Background
    init_color( 0, CCOLOR3( 40, 40, 40 ) ); // Background
    init_color( 1, CCOLOR3( 69, 133, 136 ) ); // Title BG
    init_color( 2, CCOLOR3( 168, 153, 132 ) ); // Left Title
    init_color( 3, CCOLOR3( 220, 214, 205 ) ); // Left bg
    init_pair( 1, 0, 0 ); // Init pair for main bg
    init_pair( 2, 0, 1 ); // Init pair for menu draw
    init_pair( 3, 0, 3 ); // Init pair for left widget bg
    init_pair( 4, 0, 2 ); // Init pair for left widget title
    init_pair( 5, 0, 3 ); // Init pair for left widget text
    bkgd( COLOR_PAIR( 1 ) ); // Set background color

    // Menu draw
    attron( COLOR_PAIR( 2 ) );
    FString libInfo = FLibInfo::LibraryInformationString();
    printw( " " );
    printw( libInfo.Data() );

    const int wwlen = w - ( strlen( libInfo.Data() ) + 1 );
    for( int i = 0; i < wwlen; ++i )
        printw( " " );
    attroff( COLOR_PAIR( 2 ) );

    WINDOW* widget_left;
    widget_left = newwin( h - 1, w / 6, 1, 0 );
    wbkgd( widget_left, COLOR_PAIR( 3 ) );
    wattron( widget_left, COLOR_PAIR( 4 ) );
    wprintw( widget_left, " " );
    wprintw( widget_left, "CPU Info" );
    const int wwlen2 = w / 6 - ( strlen( "CPU Info" ) + 1 );
    for( int i = 0; i < wwlen2; ++i )
        wprintw( widget_left, " " );
    wattroff( widget_left, COLOR_PAIR( 4 ) );

    wattron( widget_left, COLOR_PAIR( 5 ) );
    for( int i = 0; i < cpuinfosize; ++i ) {
        mvwprintw( widget_left, 1 + i, 1, keys[i] );
    }
    wattroff( widget_left, COLOR_PAIR( 5 ) );

    auto colors = COLORS;
    auto colorpairs = COLOR_PAIRS;

    // Main loop
    while( true ) {
        int ch = getch();
        if( ch == 'q' ) {
            break;
        }
        
        if( ch == KEY_RESIZE ) {
            resize_term( 30, 130 );
            getch();
        }

        //wattron( widget_left, COLOR_PAIR( 4 ) );
        //mvwprintw( widget_left, 1, 1, "Mouse at row=%d, column=%d ", MOUSE_Y_POS, MOUSE_X_POS );
        //wattroff( widget_left, COLOR_PAIR( 4 ) );

        refresh();
        wrefresh( widget_left );
    }

    endwin(); // Quit curses mode
    return  0;
}

