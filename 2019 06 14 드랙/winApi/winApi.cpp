#include "stdafx.h"
#include <time.h>

HINSTANCE _hInstance;
HWND _hWnd;
POINT _ptMouse = { 0, 0 };

//렉트 관련된 이넘문을 만든다.
enum BOX
{
	BOX_FIRST = 0,
	BOX_SECOND,
	BOX_END
};

BOX currentBox;
//큰 사각형 2개, 작은 사각형 1개
RECT rc[2], rcIn;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam, int nCmdShow)
{
	_hInstance = hInstance;

	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.lpszClassName = WIN_NAME;
	wndClass.lpszMenuName = NULL;
	wndClass.style = WINSTYLE;

	RegisterClass(&wndClass);

	_hWnd = CreateWindow(
		WIN_NAME,
		WIN_NAME,
		WS_OVERLAPPEDWINDOW,	
		WINSTART_X,		
		WINSTART_Y,		
		WINSIZE_X,		
		WINSIZE_Y,		
		NULL,		
		(HMENU)NULL,
		hInstance,	
		NULL);		

					
	ShowWindow(_hWnd, nCmdShow);

	MSG message;

	while (GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam,
	LPARAM lParam)
{
	HDC hdc;		
	PAINTSTRUCT ps; 

	static int pt_x = 0;
	static int pt_y =0;
	char strPT[128];
	static int tempx = 0;
	static int tempy = 0;
	static bool draw = false;
	int i=0, j = 0;
	switch (iMessage)
	{
	case WM_CREATE: 
		srand(time(NULL));
		//박스 초기화
		rc[BOX_FIRST] = { WINSIZE_X / 2-50, WINSIZE_Y / 2-50, WINSIZE_X / 2 +50, WINSIZE_Y / 2 +50} ;
		rc[BOX_SECOND] = RectMakeCenter(WINSIZE_X / 2 + 200, WINSIZE_Y / 2, 100, 100);
		rcIn = RectMakeCenter(WINSIZE_X / 2 + 200, WINSIZE_Y / 2, 50, 50);

		//현재 움직일 사각형이 뭔가?
		currentBox = BOX_FIRST;

		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		wsprintf(strPT, "X: %d, Y: %d", pt_x, pt_y);
		TextOut(hdc, 10, 10, strPT, strlen(strPT));
		wsprintf(strPT, "X: %d, Y: %d", tempx, tempy);
		TextOut(hdc, 10, 30, strPT, strlen(strPT));


		Rectangle(hdc, rc[BOX_FIRST].left, rc[BOX_FIRST].top, rc[BOX_FIRST].right, rc[BOX_FIRST].bottom);		
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_LBUTTONDOWN:
	{
			pt_x = LOWORD(lParam);
			pt_y = HIWORD(lParam);
			if (rc[currentBox].right > pt_x && rc[currentBox].left < pt_x && rc[currentBox].top < pt_y &&  rc[currentBox].bottom > pt_y)
			{
				draw = true;
			}
			InvalidateRect(hWnd, NULL, true);
			
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (draw == true)
		{
			hdc = GetDC(hWnd);
			tempx = LOWORD(lParam);
			tempy = HIWORD(lParam);
			
			if (tempx > pt_x)
			{
				rc[currentBox].left = rc[currentBox].left + (tempx - pt_x);
				rc[currentBox].right = rc[currentBox].right + (tempx - pt_x);
				pt_x = tempx;
			}
			if (tempy > pt_y)
			{
				rc[currentBox].top = rc[currentBox].top + (tempy - pt_y);
				rc[currentBox].bottom= rc[currentBox].bottom + (tempy - pt_y);
				pt_y = tempy;
			}			
			if (tempx < pt_x)
			{
				rc[currentBox].left = rc[currentBox].left - (pt_x-tempx);
				rc[currentBox].right = rc[currentBox].right - (pt_x -tempx);
				pt_x = tempx;
			}
			if (tempy < pt_y)
			{
				rc[currentBox].top = rc[currentBox].top - (pt_y - tempy);
				rc[currentBox].bottom = rc[currentBox].bottom - (pt_y - tempy);
				pt_y = tempy;
			}
			
			
			Rectangle(hdc, rc[currentBox].left, rc[currentBox].top, rc[currentBox].right, rc[currentBox].bottom);
			InvalidateRect(hWnd, NULL, true);
			ReleaseDC(hWnd, hdc);
		}
	}
	break;
	case WM_LBUTTONUP:
	{
			draw = false;
	}
	break;
	case WM_KEYDOWN:
		switch (wParam)
		{
			case VK_LEFT:
			{
				rc[currentBox].left  -= 10;
				rc[currentBox].right -= 10;
				//* 작은사각형은 큰 사각형을 벗어나지만 않으면 됨.
				if (rcIn.right > rc[currentBox].right)
				{
					//작은 사각형의 너비
					int width = rcIn.right - rcIn.left; //50
					rcIn.right = rc[currentBox].right;
					rcIn.left = rcIn.right - width;
				}
				
			}
			break;
			case VK_RIGHT:
			{
				rc[currentBox].left  += 10;
				rc[currentBox].right += 10;
				//* 작은사각형은 큰 사각형을 벗어나지만 않으면 됨.
				if (rcIn.left < rc[currentBox].left)
				{
					//작은 사각형의 너비
					int width = rcIn.right - rcIn.left; //50
					rcIn.left = rc[currentBox].left;
					rcIn.right = rcIn.left + width;
				}
			}
			break;
			case VK_ESCAPE:
				PostQuitMessage(0);
			break;
		}

		//사각형 두개가 충돌하는 조건
		if (rc[0].right > rc[1].left &&
			rc[0].left < rc[1].right &&
			rc[0].top < rc[1].bottom &&
			rc[0].bottom > rc[1].top)
		{
			//주도권 변경!
			switch (currentBox)
			{
				case BOX_FIRST:
				currentBox = BOX_SECOND;
				break;
				case BOX_SECOND:
				currentBox = BOX_FIRST;
				break;

				//작은 사각형의 중점
				//...left + (right - left)/2 => x좌표..
				rcIn = RectMakeCenter(rc[currentBox].left +
					(rc[currentBox].right - rc[currentBox].left) / 2,
					rc[currentBox].top +
					(rc[currentBox].bottom - rc[currentBox].top) / 2,
					50, 50);
			}
		}


		InvalidateRect(hWnd, NULL, true);

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
