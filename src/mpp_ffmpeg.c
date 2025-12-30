#include "mpp_ffmpeg.h"
#include <stdio.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <MagickWand/MagickWand.h>

int ffmpeg_get_cover(const char *filepath){
    int i, ret = 0;
    const char *img_cover = "/tmp/thumb.jpg";

    if (!filepath){
        printf("Wrong filepath.\n");
        return 1;
    }

    AVFormatContext *pFormat = avformat_alloc_context();

    int ret_check = avformat_open_input(&pFormat, filepath, NULL, NULL);
    if (ret_check < 0) {
        printf("Error opening file.\n");
        avformat_free_context(pFormat);
        return 1;
    }

    // extract image from mp3 file
    for (i = 0; i < pFormat->nb_streams; i++)
        if (pFormat->streams[i]->disposition & AV_DISPOSITION_ATTACHED_PIC) {
            AVPacket pkt = pFormat->streams[i]->attached_pic;
            FILE *album_art = fopen(img_cover, "wb");
            ret = fwrite(pkt.data, pkt.size, 1, album_art);
            fclose(album_art);
            break;
        }
    if (!ret) return 1;

    avformat_free_context(pFormat);

    // convert image to 100x100 thumb
    MagickWand *m_wand = NULL;
    MagickWandGenesis();
    m_wand = NewMagickWand();

    if (MagickReadImage(m_wand, img_cover) == MagickFalse) return 1;

    MagickResizeImage(m_wand, 96, 96, LanczosFilter);
    MagickSetImageCompressionQuality(m_wand, 100);
    MagickWriteImage(m_wand, img_cover);

    if (m_wand)m_wand = DestroyMagickWand(m_wand);
    MagickWandTerminus();

    return 0;
}
