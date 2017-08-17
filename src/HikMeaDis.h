#include <time.h>
#include"iostream"
#include"stdio.h"
#include"opencv2/opencv.hpp"
#include "HCNetSDK.h"
#include "LinuxPlayM4.h"
#include <unistd.h>





void CALLBACK DecCBFun(int nPort, char * pBuf, int nSize, FRAME_INFO * pFrameInfo, void *nReserved1, int nReserved2);
void CALLBACK g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void* dwUser);
void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);
unsigned int __stdcall readCam(void *param);