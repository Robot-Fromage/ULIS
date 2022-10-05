// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         main.cpp
* @author       Clement Berthaud
* @brief        FFMPEG application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include <iostream>

extern "C" {
#include <libavformat/avformat.h>
}

int main( int argc, char * argv[] )
{
    const char* input = "C:/Users/conta/Documents/work/FFMPEG/in.mp4";

    av_register_all();
    AVFormatContext* pFormatCtx = NULL;

    //open video file
    if( avformat_open_input( &pFormatCtx, input, NULL, NULL ) != 0 )
        return  -1;

    //get stream info
    if( avformat_find_stream_info( pFormatCtx, NULL ) < 0 )
        return  -1;

    // dump the whole thing like ffprobe does:
    av_dump_format( pFormatCtx, 0, input, 0 );
    //return  0;

    // get the frame rate of each stream
    for( int idx = 0; idx < pFormatCtx->nb_streams; ++idx ) {
        AVStream* s( pFormatCtx->streams[idx] );
        std::cout << idx << ". " << s->avg_frame_rate.num << " / " << s->avg_frame_rate.den << " = " << av_q2d( s->avg_frame_rate ) << "\n";
        std::cout << s->codec->width << " " << s->codec->height << "\n";
    }

    // get the video bit rate
    std::cout << "bit rate " << pFormatCtx->bit_rate << "\n";

    return 0;
}

