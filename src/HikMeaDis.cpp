

#include"HikMeaDis.h"
int lPort = 0; // 全局播放通道
//void *hWnd = NULL;


volatile bool bBusy = false,bReady = false;
IplImage *g_frame = cvCreateImage(cvSize(1280,720),IPL_DEPTH_8U,3);
IplImage *g_frame1 = cvCreateImage(cvSize(1280,720),IPL_DEPTH_8U,3);

void CALLBACK DecCBFun(int nPort, char * pBuf, int nSize, FRAME_INFO * pFrameInfo, void *nReserved1, int nReserved2)
{
   // printf("%d, %d\n",pFrameInfo->nHeight,pFrameInfo->nWidth);
    IplImage *picYV12 = cvCreateImageHeader(cvSize(pFrameInfo->nWidth,pFrameInfo->nHeight * 3 / 2), IPL_DEPTH_8U,1);
    cvSetData(picYV12,pBuf,pFrameInfo->nWidth);
	
	cvCvtColor(picYV12, g_frame1, CV_YUV2BGR_YV12);
	//cvShowImage("imgaa",g_frame);

	if(!bBusy)
	{
	    bBusy = true;
	  cvCopy(g_frame1,g_frame);
	   bBusy = false;
	    
	}
	else
	return ;
	cv::waitKey(1);
	return;
}


void CALLBACK g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void* dwUser)
{
	//hWnd = GetConsoleWindow();

	switch (dwDataType)
	{
	case NET_DVR_SYSHEAD: //ÏµÍ³Í·


		if (!PlayM4_GetPort(&lPort))  //»ñÈ¡²¥·Å¿âÎŽÊ¹ÓÃµÄÍšµÀºÅ
		{
			break;
		}
		//m_iPort = lPort; //µÚÒ»ŽÎ»Øµ÷µÄÊÇÏµÍ³Í·£¬œ«»ñÈ¡µÄ²¥·Å¿âportºÅž³ÖµžøÈ«ŸÖport£¬ÏÂŽÎ»Øµ÷ÊýŸÝÊ±ŒŽÊ¹ÓÃŽËportºÅ²¥·Å
		if (dwBufSize > 0)
		{
			if (!PlayM4_SetStreamOpenMode(lPort, STREAME_REALTIME))  //ÉèÖÃÊµÊ±Á÷²¥·ÅÄ£Êœ
			{
				break;
			}

			if (!PlayM4_OpenStream(lPort, pBuffer, dwBufSize, 1024 * 1024)) //Žò¿ªÁ÷œÓ¿Ú
			{
				break;
			}

			//ÉèÖÃœâÂë»Øµ÷º¯Êý Ö»œâÂë²»ÏÔÊŸ
			if (!PlayM4_SetDecCallBack(lPort, DecCBFun))
			{
				break;
			}
			struct aa{int unuser;} b;
			b.unuser = 1;
			if (!PlayM4_Play(lPort, NULL)) //²¥·Å¿ªÊŒ
			{
				break;
			}
		}
		break;
	case NET_DVR_STREAMDATA:   //ÂëÁ÷ÊýŸÝ
		if (dwBufSize > 0 && lPort != -1)
		{
			// cout << "ÂëÁ÷ÊýŸÝ" << endl;
			if (!PlayM4_InputData(lPort, pBuffer, dwBufSize))
			{
				break;
			}
		}
		break;
	default: //ÆäËûÊýŸÝ
		if (dwBufSize > 0 && lPort != -1)
		{
			if (!PlayM4_InputData(lPort, pBuffer, dwBufSize))
			{
				break;
			}
		}
		break;
	}

}

void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
	char tempbuf[256] = { 0 };
	switch (dwType)
	{
	case EXCEPTION_RECONNECT:    //Ô€ÀÀÊ±ÖØÁ¬
		printf("----------reconnect--------%d\n", time(NULL));
		break;
	default:
		break;
	}
}

unsigned int __stdcall readCam(void *param)
{
	
	return 0;
}


