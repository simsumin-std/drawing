// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//
#pragma once
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

/*********   DEFINE   ***********/

#define WINSTART_X 100
#define WINSTART_Y 100
#define WINSIZE_X 640
#define WINSIZE_Y 480
#define WINSTYLE CS_HREDRAW | CS_VREDRAW
#define WIN_NAME (LPTSTR)(TEXT("���ͽ��ڶ� API!!!"))

/*********************************/


/*********   ��������   ***********/

extern HINSTANCE _hInstance;
extern HWND _hWnd;
extern POINT _ptMouse;

/*********************************/




// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "commonMacro.h"