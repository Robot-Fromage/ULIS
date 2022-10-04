// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         VReader.h
* @author       Clement Berthaud
* @brief        AnimatedLayerStackBasicDisplay application for ULIS.
* @license      Please refer to LICENSE.md
*/

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <inttypes.h>
}

struct FVideoReaderState {
    // Public
    int width, height;
    AVRational time_base;

    // Private
    AVFormatContext* av_format_ctx;
    AVCodecContext* av_codec_ctx;
    int video_stream_index;
    AVFrame* av_frame;
    AVPacket* av_packet;
    SwsContext* sws_scaler_ctx;
};

bool video_reader_open( FVideoReaderState* state, const char* filename );
bool video_reader_read_frame( FVideoReaderState* state, uint8_t* frame_buffer, int64_t* pts );
bool video_reader_seek_frame( FVideoReaderState* state, int64_t ts );
void video_reader_close( FVideoReaderState* state );

