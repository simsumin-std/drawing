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
//큰 사각형 3개
RECT rc[3];

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
	int i=0, j = 0, count =0;
	switch (iMessage)
	{
	case WM_CREATE: 
		srand(time(NULL));
		//박스 초기화
		rc[BOX_FIRST] = { WINSIZE_X / 2-50, WINSIZE_Y / 2-50, WINSIZE_X / 2 +50, WINSIZE_Y / 2 +50} ;
		rc[BOX_SECOND] = { WINSIZE_X / 2 - 200, WINSIZE_Y / 2 - 200, WINSIZE_X / 2 - 100, WINSIZE_Y / 2 - 100 };
		rc[BOX_END] = { WINSIZE_X / 2 + 50, WINSIZE_Y / 2 + 50, WINSIZE_X / 2 + 150, WINSIZE_Y / 2 + 150 };

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


		for (int i = 0; i < 3; i++)
		{
			Rectangle(hdc, rc[i].left, rc[i].top, rc[i].right, rc[i].bottom);
		}

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_LBUTTONDOWN:
	{
			pt_x = LOWORD(lParam);
			pt_y = HIWORD(lParam);
			if (rc[BOX_FIRST].right > pt_x && rc[BOX_FIRST].left < pt_x && rc[BOX_FIRST].top < pt_y && rc[BOX_FIRST].bottom > pt_y)
			{
				currentBox = BOX_FIRST;
				draw = true;
			}
			if (rc[BOX_SECOND].right > pt_x && rc[BOX_SECOND].left < pt_x && rc[BOX_SECOND].top < pt_y && rc[BOX_SECOND].bottom > pt_y)
			{
				currentBox = BOX_SECOND;
				draw = true;
			}
			if (rc[BOX_END].right > pt_x && rc[BOX_END].left < pt_x && rc[BOX_END].top < pt_y &&  rc[BOX_END].bottom > pt_y)
			{
				currentBox = BOX_END;
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
				/*for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						if (i == j)
						{
						}
						else if (rc[i].right > rc[j].left &&
							rc[i].left < rc[j].right &&
							rc[i].top < rc[j].bottom &&
							rc[i].bottom > rc[j].top)
						{
							if (rc[i].right > rc[j].left)
							{
								rc[j].left += (tempx - pt_x);
								rc[j].right += (tempx - pt_x);
							}
						}
					}
				}*/
				while (1)
				{
					if (rc[0].right > rc[1].left &&
						rc[0].left < rc[1].right &&
						rc[0].top < rc[1].bottom &&
						rc[0].bottom > rc[1].top)
					{
						if (rc[0].right > rc[1].left)
						{
							rc[1].left += (tempx - pt_x);
							rc[1].right += (tempx - pt_x);
						}

					}
					if (rc[0].right > rc[2].left &&
						rc[0].left < rc[2].right &&
						rc[0].top < rc[2].bottom &&
						rc[0].bottom > rc[2].top)
					{
						if (rc[0].right > rc[2].left)
						{
							rc[2].left += (tempx - pt_x);
							rc[2].right += (tempx - pt_x);
						}

					}
					if (rc[1].right > rc[0].left &&
						rc[1].left < rc[0].right &&
						rc[1].top < rc[0].bottom &&
						rc[1].bottom > rc[0].top)
					{
						if (rc[1].right > rc[0].left)
						{
							rc[0].left += (tempx - pt_x);
							rc[0].right += (tempx - pt_x);
						}

					}
					if (rc[1].right > rc[2].left &&
						rc[1].left < rc[2].right &&
						rc[1].top < rc[2].bottom &&
						rc[1].bottom > rc[2].top)
					{
						if (rc[1].right > rc[2].left)
						{
							rc[2].left += (tempx - pt_x);
							rc[2].right += (tempx - pt_x);
						}

					}
					if (rc[2].right > rc[0].left &&
						rc[2].left < rc[0].right &&
						rc[2].top < rc[0].bottom &&
						rc[2].bottom > rc[0].top)
					{
						if (rc[2].right > rc[0].left)
						{
							rc[0].left += (tempx - pt_x);
							rc[0].right += (tempx - pt_x);
						}

					}
					if (rc[2].right > rc[1].left &&
						rc[2].left < rc[1].right &&
						rc[2].top < rc[1].bottom &&
						rc[2].bottom > rc[1].top)
					{
						if (rc[2].right > rc[1].left)
						{
							rc[1].left += (tempx - pt_x);
							rc[1].right += (tempx - pt_x);
						}

					}
				}
				pt_x = tempx;
			}
			if (tempy > pt_y)
			{
				rc[currentBox].top = rc[currentBox].top + (tempy - pt_y);
				rc[currentBox].bottom= rc[currentBox].bottom + (tempy - pt_y);
				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						if (i == j)
						{
						}
						else if (rc[i].right > rc[j].left &&
							rc[i].left < rc[j].right &&
							rc[i].top < rc[j].bottom &&
							rc[i].bottom > rc[j].top)
						{
							if (rc[i].top < rc[j].bottom)
							{
								rc[j].top += (tempy - pt_y);
								rc[j].bottom += (tempy - pt_y);
							}
						}
					}
				}
/*				if (rc[0].right > rc[1].left &&
					rc[0].left < rc[1].right &&
					rc[0].top < rc[1].bottom &&
					rc[0].bottom > rc[1].top)
				{
					if (rc[0].top < rc[1].bottom)
					{
						rc[1].top += (tempy - pt_y);
						rc[1].bottom += (tempy - pt_y);
					}
				}
				if (rc[0].right > rc[2].left &&
					rc[0].left < rc[2].right &&
					rc[0].top < rc[2].bottom &&
					rc[0].bottom > rc[2].top)
				{
					if (rc[0].top < rc[2].bottom)
					{
						rc[2].top += (tempy - pt_y);
						rc[2].bottom += (tempy - pt_y);
					}
				}*/
				pt_y = tempy;
			}			
			if (tempx < pt_x)
			{
				rc[currentBox].left = rc[currentBox].left - (pt_x-tempx);
				rc[currentBox].right = rc[currentBox].right - (pt_x -tempx);
				
				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						if (i == j)
						{
						}
						else if (rc[i].right > rc[j].left &&
							rc[i].left < rc[j].right &&
							rc[i].top < rc[j].bottom &&
							rc[i].bottom > rc[j].top)
						{
							if (rc[i].left > rc[j].right)
							{
								rc[j].left += (tempx - pt_x);
								rc[j].right += (tempx - pt_x);
							}
						}
					}
				}
			/*	if (rc[0].right > rc[1].left &&
					rc[0].left < rc[1].right &&
					rc[0].top < rc[1].bottom &&
					rc[0].bottom > rc[1].top)
				{
					if (rc[0].left < rc[1].right)
					{
						rc[1].left += (tempx - pt_x);
						rc[1].right += (tempx - pt_x);
					}
				}
				if (rc[0].right > rc[2].left &&
					rc[0].left < rc[2].right &&
					rc[0].top < rc[2].bottom &&
					rc[0].bottom > rc[2].top)
				{
					if (rc[0].left < rc[2].right)
					{
						rc[2].left += (tempx - pt_x);
						rc[2].right += (tempx - pt_x);
					}
				}*/
				pt_x = tempx;
			}
			if (tempy < pt_y)
			{
				rc[currentBox].top = rc[currentBox].top - (pt_y - tempy);
				rc[currentBox].bottom = rc[currentBox].bottom - (pt_y - tempy);
				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						if (i == j)
						{
						}
						else if (rc[i].right > rc[j].left &&
							rc[i].left < rc[j].right &&
							rc[i].top < rc[j].bottom &&
							rc[i].bottom > rc[j].top)
						{
							if (rc[i].bottom > rc[j].top)
							{
								rc[j].top += (tempy - pt_y);
								rc[j].bottom += (tempy - pt_y);
							}
						}
					}
				}
			/*	if (rc[0].right > rc[1].left &&
					rc[0].left < rc[1].right &&
					rc[0].top < rc[1].bottom &&
					rc[0].bottom > rc[1].top)
				{
					if (rc[0].bottom > rc[1].top)
					{
						rc[1].top += (tempy - pt_y);
						rc[1].bottom += (tempy - pt_y);
					}
				}
				if (rc[0].right > rc[2].left &&
					rc[0].left < rc[2].right &&
					rc[0].top < rc[2].bottom &&
					rc[0].bottom > rc[2].top)
				{
					if (rc[0].bottom > rc[2].top)
					{
						rc[2].top += (tempy - pt_y);
						rc[2].bottom += (tempy - pt_y);
					}
				}*/
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

				
			}
			break;

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
