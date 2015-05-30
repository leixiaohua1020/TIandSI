/**
 * TIandSIprocmd
 * (TIandSI Professional CMD version)
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 *
 * http://blog.csdn.net/leixiaohua1020
 * 
 * 本工程可以计算一个压缩视频的时间信息TI（Temporal perceptual Information，
 * 也可以称时间复杂度）和空间信息SI（Spatial perceptual Information，也可以
 * 称空间复杂度）。计算方法出自标准：ITU-R BT.1788
 *
 * Professional: 支持压缩码流（而不是像素数据比如YUV，RGB）作为输入
 *
 * This software can calculate a video bitstream's TI(Temporal perceptual Information) 
 * and SI(Spatial perceptual Information) according to ITU-R BT.1788.
 *
 * Professional: Support bitstreams (not raw data such as YUV, RGB, etc.) as Input.
 *
 */
#include <Windows.h>
#include <malloc.h>
#include "Getopt.h"
#include <vector>

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "sdl/SDL.h"
};

#define REFRESH_EVENT  (SDL_USEREVENT + 1)

typedef struct SDLParam{
	SDL_Surface *screen; 
	SDL_Overlay *bmp; 
	SDL_Rect rect;
	bool graphically_ti;
	bool graphically_si;
	bool isinterval;
	char *show_YBuffer;
	char *show_UVBuffer;
	//Show Info
	int show_w;
	int show_h;
	//Pixel Info
	int pixel_w;
	int pixel_h;
}SDLParam;

int mark_exit=0;


int show_thread(void *opaque){
	SDLParam *sdlparam=(SDLParam *)opaque;

	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {  
		printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
		return 0;
	} 

	sdlparam->screen = SDL_SetVideoMode(sdlparam->show_w, sdlparam->show_h, 0, 0);
	if(!sdlparam->screen) {  
		printf("SDL: could not set video mode - exiting\n");  
		return 0;
	}
	sdlparam->bmp = SDL_CreateYUVOverlay(sdlparam->pixel_w, sdlparam->pixel_h,SDL_YV12_OVERLAY, sdlparam->screen); 

	if(sdlparam->graphically_si){
		SDL_WM_SetCaption("Spatial perceptual Information",NULL);
	}else if(sdlparam->graphically_ti){
		SDL_WM_SetCaption("Temporal perceptual Information",NULL);
	}

	sdlparam->rect.x = 0;    
	sdlparam->rect.y = 0;    
	sdlparam->rect.w = sdlparam->show_w;    
	sdlparam->rect.h = sdlparam->show_h;    

	SDL_Event event;
	while(mark_exit==0) {
		SDL_WaitEvent(&event);
		switch(event.type){
		case REFRESH_EVENT:{
			SDL_LockYUVOverlay(sdlparam->bmp);
			sdlparam->bmp->pixels[0]=(Uint8 *)sdlparam->show_YBuffer;
			sdlparam->bmp->pixels[2]=(Uint8 *)sdlparam->show_UVBuffer;
			sdlparam->bmp->pixels[1]=(Uint8 *)sdlparam->show_UVBuffer+sdlparam->pixel_w*sdlparam->pixel_h/4;     
			sdlparam->bmp->pitches[0]=sdlparam->pixel_w;
			sdlparam->bmp->pitches[2]=sdlparam->pixel_w/2;   
			sdlparam->bmp->pitches[1]=sdlparam->pixel_w/2;
			SDL_UnlockYUVOverlay(sdlparam->bmp); 
			SDL_DisplayYUVOverlay(sdlparam->bmp, &sdlparam->rect); 
			break;
						   }
		case SDL_QUIT:{
			mark_exit=1;
			break;
					  }

		}
	}
	return 0;
}

//帮助
void tisip_usage(){
	printf("\nHelp==================================\n");
	printf("TISIpro version 1.0:\n");
	printf("This software calculate value of TI and SI, and save result as *.csv file\n\n");
	printf("Lei Xiaohua \n");
	printf("leixiaohua1020@126.com\n");
	printf("Communication University of China / Digital TV Technology \n");
	printf("http://blog.csdn.net/leixiaohua1020\n\n");
	printf("----------------------------------------\n");
	printf("Options:\n");
	printf("-i\tInput file URL\n");
	printf("-o\tOutput *.csv file URL [Default is {Input Name}.csv]\n");
	printf("-l\tLimit calculate frame number\n");
	printf("-g\tShow result Graphically [Option is 'ti' or 'si']\n");
	printf("-n\tInterval to show result to the screen [Default is 5]\n");
	printf("-h\tShow this text.\n");
	printf("----------------------------------------\n");
	printf("Examples:\n");
	printf("TISIprogCmd -i test.avi\n");
	printf("TISIprogCmd -i test.avi -o test_cal.csv\n");
	printf("TISIprogCmd -i test.avi -g ti\n");
	printf("=========================================\n");
}

//计算TI、SI
int tisip_TISI(char* ydata,char* prev_ydata,int width,int height,SDLParam sdlparam,float &ti,float &si)
{
	//----------------
	unsigned char *NewYBuffer=NULL;
	unsigned char *NewUVBuffer=NULL;
	int nHeight=0, nWidth=0;
	
	int nFrameNum;
	int nYSize;	
	int nFrameSize;

	unsigned char *pFrame;
	unsigned char **pFrame2D;
	unsigned char *pNextFrame;
	unsigned char **pNextFrame2D;


	float *NewSIBuffer;
	float *NewTIBuffer;
	float flSI_A, flSI_S, flTI_A, flTI_S;

	int i, j, k, m, n;
	/****************初始化设置**********************/
	nHeight = height;
	nWidth = width;
	nYSize = nHeight*nWidth;

	i = 0;
	j = 0;
	k = 0;
	m = n = 0;
	flSI_A = flSI_S = flTI_A = flTI_S = 0;

	pFrame = new unsigned char [nYSize];
	pFrame2D = new unsigned char *[nHeight];
	for(i = 0; i < nHeight; i++)
		pFrame2D[i] = pFrame + i*nWidth;

	pNextFrame = new unsigned char [nYSize];
	pNextFrame2D = new unsigned char *[nHeight];
	for(i = 0; i < nHeight; i++)
		pNextFrame2D[i] = pNextFrame + i*nWidth;

	NewYBuffer = new unsigned char [nYSize];

	int nUVSize=nYSize/2;
	//if(sdlparam.graphically_si==true||sdlparam.graphically_ti==true){
	//	//UV
	//	NewUVBuffer= new unsigned char[nUVSize];
	//	memset(NewUVBuffer, 0x80, nUVSize);
	//}

	NewSIBuffer = new float [nYSize];
	NewTIBuffer = new float [nYSize];

	//Check
	if(mark_exit==1){
		return -1;
	}

	/********************计算SI***********************/
	memcpy(pFrame,ydata,nYSize);

	for(j = 0; j < nHeight; j++)
		for(i = 0; i < nWidth; i++)
		{
			float XSobel, YSobel, SobelValue;

			if((j == 0) && (i == 0))//(0, 0)
			{
				XSobel = -3*pFrame2D[0][0] + 3*pFrame2D[0][i+1]				
				-pFrame2D[j+1][0] + pFrame2D[j+1][i+1];

				YSobel = 3*pFrame2D[0][0] + pFrame2D[0][i+1]
				-3*pFrame2D[j+1][0] - pFrame2D[j+1][i+1];
			}
			else if((j == 0) && (i == (nWidth - 1)))//(nWidth - 1, 0)
			{
				XSobel = -3*pFrame2D[0][i-1] + 3*pFrame2D[0][i]
				-pFrame2D[j+1][i-1] + pFrame2D[j+1][i];

				YSobel = pFrame2D[0][i-1] + 3*pFrame2D[0][i]
				-pFrame2D[j+1][i-1] - 3*pFrame2D[j+1][i];
			}
			else if((j == (nHeight - 1)) && (i == 0))//(0, nHeight - 1)
			{
				XSobel = -pFrame2D[j-1][0] + pFrame2D[j-1][i+1]
				-3*pFrame2D[j][0] + 3*pFrame2D[j][i+1];				

				YSobel = 3*pFrame2D[j-1][0] + pFrame2D[j-1][i+1]
				-3*pFrame2D[j][0] - pFrame2D[j][i+1];
			}
			else if((j == (nHeight - 1)) && (i == (nWidth - 1)))//(nWidth - 1, nHeight - 1)
			{
				XSobel = -pFrame2D[j-1][i-1] + pFrame2D[j-1][i]
				-3*pFrame2D[j][i-1] + 3*pFrame2D[j][i];				

				YSobel = pFrame2D[j-1][i-1] + 3*pFrame2D[j-1][i]
				-pFrame2D[j][i-1] - 3*pFrame2D[j][i];
			}
			else if(j == 0)//上面一排
			{
				XSobel = -3*pFrame2D[0][i-1] + 3*pFrame2D[0][i+1]
				-pFrame2D[j+1][i-1] + pFrame2D[j+1][i+1];

				YSobel = pFrame2D[0][i-1] + 2*pFrame2D[0][i] + pFrame2D[0][i+1]
				-pFrame2D[j+1][i-1] - 2*pFrame2D[j+1][i] - pFrame2D[j+1][i+1];
			}
			else if(j == (nHeight - 1))//下面一排
			{
				XSobel = -pFrame2D[j-1][i-1] + pFrame2D[j-1][i+1]
				-3*pFrame2D[j][i-1] + 3*pFrame2D[j][i+1];

				YSobel = pFrame2D[j-1][i-1] + 2*pFrame2D[j-1][i] + pFrame2D[j-1][i+1]
				-pFrame2D[j][i-1] - 2*pFrame2D[j][i] - pFrame2D[j][i+1];
			}
			else if(i == 0)//左边一排
			{
				XSobel = -pFrame2D[j-1][0] + pFrame2D[j-1][i+1]
				-2*pFrame2D[j][0] + 2*pFrame2D[j][i+1]				
				-pFrame2D[j+1][0] + pFrame2D[j+1][i+1];

				YSobel = 3*pFrame2D[j-1][0] + pFrame2D[j-1][i+1]
				-3*pFrame2D[j+1][0] - pFrame2D[j+1][i+1];
			}
			else if(i == (nWidth - 1))//右面一排
			{
				XSobel = -pFrame2D[j-1][i-1] + pFrame2D[j-1][i]
				-2*pFrame2D[j][i-1] + 2*pFrame2D[j][i]				
				-pFrame2D[j+1][i-1] + pFrame2D[j+1][i];

				YSobel = pFrame2D[j-1][i-1] + 3*pFrame2D[j-1][i]
				-pFrame2D[j+1][i-1] - 3*pFrame2D[j+1][i];
			}
			else
			{
				XSobel = -pFrame2D[j-1][i-1] + pFrame2D[j-1][i+1]
				-2*pFrame2D[j][i-1] + 2*pFrame2D[j][i+1]				
				-pFrame2D[j+1][i-1] + pFrame2D[j+1][i+1];

				YSobel = pFrame2D[j-1][i-1] + 2*pFrame2D[j-1][i] + pFrame2D[j-1][i+1]
				-pFrame2D[j+1][i-1] - 2*pFrame2D[j+1][i] - pFrame2D[j+1][i+1];
			}

			SobelValue = sqrt(pow(XSobel, 2) + pow(YSobel, 2));

			NewYBuffer[j*nWidth + i] = (unsigned char)SobelValue;
			NewSIBuffer[j*nWidth + i] = SobelValue;
		}

		for(m = 1; m < (nHeight - 1); m++)
			for(n = 1; n < (nWidth - 1); n++)
			{
				flSI_A = flSI_A + NewSIBuffer[m*nWidth + n];
			}
		flSI_A = flSI_A/((nWidth - 2)*(nHeight - 2));

		for(m = 1; m < (nHeight - 1); m++)
			for(n = 1; n < (nWidth - 1); n++)
			{
				flSI_S = flSI_S + pow((NewSIBuffer[m*nWidth+n]-flSI_A), 2);
			}
		flSI_S = sqrt(flSI_S/((nWidth - 2)*(nHeight - 2) - 1));
		//输出SI
		//printf("SI(%d) : %.3f\n",k+1,flSI_S);
		si=flSI_S;
		//画图
		if(sdlparam.graphically_si==true&&sdlparam.isinterval==false){
			memcpy(sdlparam.show_YBuffer,NewYBuffer,nHeight*nWidth);
			//memcpy(sdlparam.show_UVBuffer,NewUVBuffer,nHeight*nWidth/4);
			SDL_Event event;
			event.type = REFRESH_EVENT;
			SDL_PushEvent(&event);
		}

		memcpy(pFrame,prev_ydata,nYSize);
		memcpy(pNextFrame,ydata,nYSize);

		for(j = 0; j < nHeight; j++)
			for(i = 0; i < nWidth; i++)
			{
				NewTIBuffer[j*nWidth + i] = pNextFrame2D[j][i] - pFrame2D[j][i];
				NewYBuffer[j*nWidth + i] = abs(pNextFrame2D[j][i] - pFrame2D[j][i]);

				flTI_A = flTI_A + NewTIBuffer[j*nWidth + i];
			}

		flTI_A = flTI_A/nYSize;

		for(j = 0; j < nHeight; j++)
			for(i = 0; i < nWidth; i++)
			{
				flTI_S = flTI_S + pow((NewTIBuffer[j*nWidth + i]-flTI_A), 2);
			}

		flTI_S = sqrt(flTI_S/(nYSize - 1));
		//输出TI
		ti=flTI_S;

		//画图
		if(sdlparam.graphically_ti==true&&sdlparam.isinterval==false){
			memcpy(sdlparam.show_YBuffer,NewYBuffer,nHeight*nWidth);
			SDL_Event event;
			event.type = REFRESH_EVENT;
			SDL_PushEvent(&event);
		}

		delete []pFrame;
		delete []pFrame2D;
		delete []pNextFrame;
		delete []pNextFrame2D;
		delete []NewYBuffer;
		delete []NewTIBuffer;
		delete []NewSIBuffer;

		return 0;
}


int main(int argc, char* argv[])
{
	char in_url[500]={0};
	char out_url[500]={0};
	int limit_num=0;
	bool limit_is=false;
	bool graphically_ti=false;
	bool graphically_si=false;
	bool isinterval=true;
	int intervalcnt=5;
	//接收参数------------------
	extern char *optarg;
	int opt;
	//--------------------------
	if(argc==1){
		tisip_usage();
		return 0;
	}
	while ((opt =getopt(argc, argv,"i:o:g:l:n:h")) != -1)
	{
		switch (opt)
		{
		case 'h':{
			tisip_usage();
			return 0;
				 }
		case 'i':{
			strcpy(in_url,optarg);
			break;
				 }
		case 'o':{
			strcpy(out_url,optarg);
			break;
				 }
		case 'l':{
			limit_num=atoi(optarg);
			limit_is=true;
			break;
				 }
		case 'n':{
			intervalcnt=atoi(optarg);
			break;
				 }
		case 'g':{
			if(strcmp(optarg,"ti")==0){
				graphically_ti=true;
			}else if(strcmp(optarg,"si")==0){
				graphically_si=true;
			}
			break;
				 }
		default:
			printf("Unknown: %c\n", opt);
			tisip_usage();
			return 0;
		}

	}
	if(strcmp(in_url,"")==0){
		printf("Input Video's URL is not set. Exit.\n");
		return 0;
	}

	if(strcmp(out_url,"")==0){
		printf("Output .csv file is not set. Default is {Input Name}.csv\n");
		char *suffix=(char *)strrchr(in_url, '.');//
		*suffix='\0';
		strcpy(out_url,in_url);
		*suffix='.';
		sprintf(out_url,"%s.csv",out_url);
	}
	


	
	AVFormatContext	*pFormatCtx;
	int				i, video_stream,audio_stream;
	AVCodecContext	*pCodecCtx,*pCodecCtx_au;
	AVCodec			*pCodec,*pCodec_au;

	av_register_all();
	//avformat_network_init();
	pFormatCtx = avformat_alloc_context();
	if(avformat_open_input(&pFormatCtx,in_url,NULL,NULL)!=0){
		printf("Couldn't open file.\n");
		return FALSE;
	}
	if(av_find_stream_info(pFormatCtx)<0)
	{
		printf("Couldn't find stream information.\n");
		return FALSE;
	}
	video_stream=-1;
	audio_stream=-1;
	for(i=0; i<pFormatCtx->nb_streams; i++) {
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
			video_stream=i;
		}if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO){
			audio_stream=i;
		}
	}
	if(video_stream==-1){
		printf("Could not find a video stream.\n");
		return FALSE;
	}
	

	pCodecCtx=pFormatCtx->streams[video_stream]->codec;
	pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
	if(pCodec==NULL)
	{
		printf("Codec not found.\n");
		return FALSE;
	}
	if(avcodec_open2(pCodecCtx, pCodec,NULL)<0)
	{
		printf("Could not open codec.\n");
		return FALSE;
	}

	//------------SDL----------------
	SDLParam sdlparam={NULL,NULL,{0,0,0,0},graphically_ti,graphically_si,isinterval,NULL,NULL,0,0,0,0};
	if(graphically_ti==true||graphically_si==true){
		sdlparam.graphically_si=graphically_si;
		sdlparam.graphically_ti=graphically_ti;
		sdlparam.show_w=pCodecCtx->width;
		sdlparam.show_h=pCodecCtx->height;
		sdlparam.pixel_w=pCodecCtx->width;
		sdlparam.pixel_h=pCodecCtx->height;
		//FIX
		sdlparam.show_YBuffer=(char *)malloc(sdlparam.pixel_w*sdlparam.pixel_h);
		sdlparam.show_UVBuffer=(char *)malloc(sdlparam.pixel_w*sdlparam.pixel_h/2);
		memset(sdlparam.show_UVBuffer,0x80,sdlparam.pixel_w*sdlparam.pixel_h/2);

		SDL_Thread *video_tid = SDL_CreateThread(show_thread,&sdlparam);
	}
	//---------------

	//vtype=VE_TIL_SIL;
	std::vector<float> silist;
	std::vector<float> tilist;

	AVFrame	*pFrame,*pFrameYUV;
	pFrame=avcodec_alloc_frame();
	pFrameYUV=avcodec_alloc_frame();
	uint8_t *out_buffer;
	out_buffer=(uint8_t *)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
	avpicture_fill((AVPicture *)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);

	int ret, got_picture;
	static struct SwsContext *img_convert_ctx;
	int y_size = pCodecCtx->width * pCodecCtx->height;

	AVPacket *packet=(AVPacket *)av_malloc(sizeof(AVPacket));
	av_new_packet(packet, y_size);
	//计算TI的时候使用
	int prev_has=0;
	uint8_t *prev_ydata=(uint8_t *)av_malloc(pCodecCtx->width*pCodecCtx->height);

	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BILINEAR, NULL, NULL, NULL); 
	//打开文件
	FILE *fp=fopen(out_url,"wb+");
	fprintf(fp,"TI,SI\n");
	//记个数
	int framecnt=0;
	while(av_read_frame(pFormatCtx, packet)>=0&&(framecnt<limit_num||!limit_is))
	{
		if(packet->stream_index==video_stream)
		{
			ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
			if(ret < 0)
			{
				printf("Decode Error.\n");
				return -1;
			}
			if(got_picture)
			{
				sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
				//有前面的帧，才能计算TI
				if(prev_has==1){
					if(framecnt%intervalcnt==0){
						sdlparam.isinterval=false;
					}else{
						sdlparam.isinterval=true;
					}
					float ti=0,si=0;
					int retval=tisip_TISI((char *)pFrameYUV->data[0],(char *)prev_ydata,pCodecCtx->width,pCodecCtx->height,sdlparam,ti,si);
					if(retval==-1)
						break;

					tilist.push_back(ti);
					silist.push_back(si);
					printf("%f,%f\n",ti,si);
					fprintf(fp,"%f,%f\n",ti,si);
					framecnt++;
				}else{
					prev_has=1;
				}
				//拷贝亮度数据
				memcpy(prev_ydata,pFrameYUV->data[0],pCodecCtx->width*pCodecCtx->height);
			}
		}
		av_free_packet(packet);
	}
	sws_freeContext(img_convert_ctx);
	//计算平均值
	int num=0;
	float sum=0;
	num=silist.size();
	for (int i=0;i<num;i++)
		sum +=silist[i];
	float siavg=sum/num;
	num=0;
	sum=0;
	num=tilist.size();
	for (int i=0;i<num;i++)
		sum +=tilist[i];
	float tiavg=sum/num;

	fprintf(fp,"TI_AVG,SI_AVG\n");
	fprintf(fp,"%f,%f\n",tiavg,siavg);
	fclose(fp);

	av_free(out_buffer);
	av_free(pFrameYUV);
	avcodec_close(pCodecCtx);

	if(graphically_ti==true||graphically_si==true){
		free(sdlparam.show_YBuffer);
		free(sdlparam.show_UVBuffer);
		SDL_Event event;
		event.type=SDL_QUIT;
		SDL_PushEvent(&event);
	}


	avformat_close_input(&pFormatCtx);
	return 0;
}