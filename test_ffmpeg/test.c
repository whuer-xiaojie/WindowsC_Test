#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
//编码
#include <libavcodec/avcodec.h>
//封装格式处理
#include <libavformat/avformat.h>
//像素处理
#include <libswscale/swscale.h>
#include "graphics.h"
#include "clock.h"
//x9_opengl context info define in the paint thread
 SDL_GLContext gl_context;
 GLuint program;/*the x9 shader program*/
 GLint  positionLoc;/*the x9 shader variable entrance*/
 GLint  texCoordLoc;
 GLint  samplerLoc;
 GLuint x9_textureId;/*the x9 paint texture name*/

//the x9 show region size
 int screen_Width=600 ;
 int screen_Height=400;
int main()
{

    //获取输入输出文件名
    const char *input = "test.avi";
    const char *output = "test.rgb";
    init_graphics();
    x9_create_texture();
    //1.注册所有组件
    av_register_all();

    //封装格式上下文，统领全局的结构体，保存了视频文件封装格式的相关信息
    AVFormatContext *pFormatCtx = avformat_alloc_context();

    //2.打开输入视频文件
    if (avformat_open_input(&pFormatCtx, input, NULL, NULL) != 0)
    {
        printf("%s","can't open the input file \n");
        return;
    }

    //3.获取视频文件信息
    if (avformat_find_stream_info(pFormatCtx,NULL) < 0)
    {
        printf("%s","can't get the stream info \n");
        return;
    }

    //获取视频流的索引位置
    //遍历所有类型的流（音频流、视频流、字幕流），找到视频流
    int v_stream_idx = -1;
    int i = 0;
    //number of streams
    for (; i < pFormatCtx->nb_streams; i++)
    {
        //流的类型
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            v_stream_idx = i;
            break;
        }
    }

    if (v_stream_idx == -1)
    {
        printf("%s","can't find the stream id\n");
        return;
    }

    //只有知道视频的编码方式，才能够根据编码方式去找到解码器
    //获取视频流中的编解码上下文
    AVCodecContext *pCodecCtx = pFormatCtx->streams[v_stream_idx]->codec;
    //4.根据编解码上下文中的编码id查找对应的解码
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL)
    {
        printf("%s","can't find pCodec\n");
        return;
    }

    //5.打开解码器
    if (avcodec_open2(pCodecCtx,pCodec,NULL)<0)
    {
        printf("%s","can't open pCodec\n");
        return;
    }

    //输出视频信息
    printf("pFormatCtx->iformat->name::%s\n",pFormatCtx->iformat->name);
    printf("pFormatCtx->duration::%d\n", (pFormatCtx->duration)/1000000);
    printf("pCodecCtx->width,pCodecCtx->height::%d,%d\n",pCodecCtx->width,pCodecCtx->height);
    printf("pCodec->name::%s\n",pCodec->name);

    //准备读取
    //AVPacket用于存储一帧一帧的压缩数据（H264）
    //缓冲区，开辟空间
    AVPacket *packet = (AVPacket*)av_malloc(sizeof(AVPacket));

    //AVFrame用于存储解码后的像素数据(YUV)
    //内存分配
    AVFrame *pFrame = av_frame_alloc();
    //YUV420
    AVFrame *pFrameYUV = av_frame_alloc();
    //只有指定了AVFrame的像素格式、画面大小才能真正分配内存
    //缓冲区分配内存
    uint8_t *out_buffer = (uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_RGB24, screen_Width, screen_Height));
    //初始化缓冲区
	avpicture_fill((AVPicture *)pFrameYUV, out_buffer, AV_PIX_FMT_RGB24, screen_Width, screen_Height);

    //用于转码（缩放）的参数，转之前的宽高，转之后的宽高，格式等
    struct SwsContext *sws_ctx = sws_getContext(pCodecCtx->width,pCodecCtx->height,pCodecCtx->pix_fmt,screen_Width, screen_Height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
    int got_picture, ret;

    FILE *fp_yuv = fopen(output, "wb+");

    int frame_count = 0;
struct SwsContext *img_convert_ctx;
    //6.一帧一帧的读取压缩数据
    while (av_read_frame(pFormatCtx, packet) >= 0)
    {
        //只要视频压缩数据（根据流的索引位置判断）
        if (packet->stream_index == v_stream_idx)
        {
            //7.解码一帧视频压缩数据，得到视频像素数据
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
            if (ret < 0)
            {
                printf("%s","error1111\n");
                return;
            }

            //为0说明解码完成，非0正在解码
            if (got_picture)
            {
                //AVFrame转为像素格式YUV420，宽高
                //2 6输入、输出数据
                //3 7输入、输出画面一行的数据的大小 AVFrame 转换是一行一行转换的
                //4 输入数据第一列要转码的位置 从0开始
                //5 输入画面的高度
				//img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
					//pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
                sws_scale(sws_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,pFrameYUV->data, pFrameYUV->linesize);

                //输出到YUV文件
                //AVFrame像素帧写入文件
                //data解码后的图像像素数据（音频采样数据）
                //Y 亮度 UV 色度（压缩了） 人对亮度更加敏感
                //U V 个数是Y的1/4
 //screen_Width =pCodecCtx->width;
//screen_Height=pCodecCtx->height;
		   int y_size = screen_Width * screen_Height;
unsigned char* buf = (unsigned char *)malloc(y_size * 3);
		   if (buf==NULL)
		   {
			fprintf(stderr, "%s():malloc for buf failed\n", __FUNCTION__);
			return 0;
		   }
		   memcpy(buf, pFrameYUV->data[0], y_size * 3);
		   x9_render_buffer_R8G8B8(buf);
__sleep((pFormatCtx->duration)/1000000);
free(buf);
		   fwrite(pFrameYUV->data[0], y_size * 3, 1, fp_yuv);
		/*
                fwrite(pFrameYUV->data[0], 1, y_size, fp_yuv);
                fwrite(pFrameYUV->data[1], 1, y_size / 4, fp_yuv);
                fwrite(pFrameYUV->data[2], 1, y_size / 4, fp_yuv);
		*/
                frame_count++;
              //  printf("11111 pCodes the %d frame \n",frame_count);
            }
        }

        //释放资源
        av_free_packet(packet);
    }

    x9_close_texture();
    close_graphics();
    fclose(fp_yuv);

    av_frame_free(&pFrame);

    avcodec_close(pCodecCtx);

    avformat_free_context(pFormatCtx);
    return 0;

}
