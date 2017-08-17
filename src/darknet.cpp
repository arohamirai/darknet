#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "utils.h"
#include "cuda.h"
#include "blas.h"
#include "connected_layer.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "utils.h"
#include "cuda.h"
#include "blas.h"
#include "connected_layer.h"

#ifdef OPENCV
#include "opencv2/highgui/highgui_c.h"
#endif
#include "HikMeaDis.h"-

extern int lPort;

extern "C" void run_detector(int argc, char **argv);

int main(int argc, char **argv)
{
//---------------------------------------
	// ³õÊ¼»¯
	NET_DVR_Init();
	//ÉèÖÃÁ¬½ÓÊ±¼äÓëÖØÁ¬Ê±¼ä
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);

	//---------------------------------------
	//ÉèÖÃÒì³£ÏûÏ¢»Øµ÷º¯Êý
	NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);

	LONG lUserID;

	//µÇÂ¼²ÎÊý£¬°üÀ¨Éè±¸µØÖ·¡¢µÇÂ¼ÓÃ»§¡¢ÃÜÂëµÈ
	NET_DVR_USER_LOGIN_INFO struLoginInfo = { 0 };
	struLoginInfo.bUseAsynLogin = 0; //Í¬²½µÇÂ¼·½Ê½
	strcpy(struLoginInfo.sDeviceAddress, "192.168.1.64"); //Éè±¸IPµØÖ·
	struLoginInfo.wPort = 8000; //Éè±¸·þÎñ¶Ë¿Ú
	strcpy(struLoginInfo.sUserName, "admin"); //Éè±¸µÇÂ¼ÓÃ»§Ãû
	strcpy(struLoginInfo.sPassword, "admin1234"); //Éè±¸µÇÂ¼ÃÜÂë

												  //Éè±¸ÐÅÏ¢, Êä³ö²ÎÊý
	NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = { 0 };

	lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
	if (lUserID < 0)
	{
		printf("Login failed, error code: %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		return 0;
	}

	//---------------------------------------
	//Æô¶¯Ô¤ÀÀ²¢ÉèÖÃ»Øµ÷Êý¾ÝÁ÷
	LONG lRealPlayHandle;

	NET_DVR_PREVIEWINFO struPlayInfo = { 0 };
	struPlayInfo.hPlayWnd = NULL;         //ÐèÒªSDK½âÂëÊ±¾ä±úÉèÎªÓÐÐ§Öµ£¬½öÈ¡Á÷²»½âÂëÊ±¿ÉÉèÎª¿Õ
	struPlayInfo.lChannel = 1;       //Ô¤ÀÀÍ¨µÀºÅ
	struPlayInfo.dwStreamType = 0;       //0-Ö÷ÂëÁ÷£¬1-×ÓÂëÁ÷£¬2-ÂëÁ÷3£¬3-ÂëÁ÷4£¬ÒÔ´ËÀàÍÆ
	struPlayInfo.dwLinkMode = 0;       //0- TCP·½Ê½£¬1- UDP·½Ê½£¬2- ¶à²¥·½Ê½£¬3- RTP·½Ê½£¬4-RTP/RTSP£¬5-RSTP/HTTP

	lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, g_RealDataCallBack_V30, NULL);
	if (lRealPlayHandle < 0)
	{
		printf("NET_DVR_RealPlay_V40 error, %d\n", NET_DVR_GetLastError());
		NET_DVR_Logout(lUserID);
		NET_DVR_Cleanup();
		return 0;
	}

	

    gpu_index = 0;
	
	int myargc = 7;
	char *myargv[] = {"/home/keetsky/Desktop/darknet-master",
					  "detector",
					  "demo",
					  "cfg/coco.data",
					  "cfg/yolo.cfg",
					  "yolo.weights",
					  "newfile.avi",
					//  "-c",
					//  "0"
					};

	run_detector(myargc, myargv);

	//---------------------------------------
	//¹Ø±ÕÔ¤ÀÀ
	NET_DVR_StopRealPlay(lRealPlayHandle);

	//ÊÍ·Å²¥·Å¿â×ÊÔ´
	PlayM4_Stop(lPort);
	PlayM4_CloseStream(lPort);
	PlayM4_FreePort(lPort);

	//×¢ÏúÓÃ»§
	NET_DVR_Logout(lUserID);
	NET_DVR_Cleanup();
	return 0;


	return 0;


}

