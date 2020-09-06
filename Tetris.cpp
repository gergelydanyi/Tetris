#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#include <Windows.h>
#include <windowsx.h>
#include <string>

constexpr auto tmr_GameTick = 1;

struct sGameInfo
{
	unsigned short int SquareSize = 20;
	HDC hdc = NULL;
	int x = 100;
	int y = 100;
	int TickInterval = 500;
	HBRUSH bgBrush = (HBRUSH)COLOR_BACKGROUND;
};

LRESULT CALLBACK HandleMessages(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int DrawSquare(sGameInfo* pGI);
int DrawTile(sGameInfo* pGI, int type);
int DrawBoard(sGameInfo* pGI);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	sGameInfo* pGI = new sGameInfo;
	LOGBRUSH lbr = {};
	lbr.lbColor = RGB(50, 50, 50);
	pGI->bgBrush = CreateBrushIndirect(&lbr);

	const wchar_t CLASS_NAME[] = L"Tetris window class";
	const wchar_t WINDOW_NAME[] = L"TETRIS";

	WNDCLASSW wc = {};

	wc.lpszClassName = CLASS_NAME;
	wc.lpfnWndProc = HandleMessages;
	wc.hInstance = hInstance;
	wc.hbrBackground = pGI->bgBrush;

	RegisterClassW(&wc);

	HWND hwnd =	CreateWindowW(
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, pGI);

	SetTimer(hwnd, tmr_GameTick, pGI->TickInterval , NULL);

	ShowWindow(hwnd, nCmdShow);

	MSG msg;

	while (GetMessageW(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return 0;
}

LRESULT CALLBACK HandleMessages(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	sGameInfo* pGI = NULL;
	if (uMsg == WM_CREATE)
	{
		CREATESTRUCTW *pcs = reinterpret_cast<CREATESTRUCTW*>(lParam);
		pGI = reinterpret_cast<sGameInfo*>(pcs->lpCreateParams);
		SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)pGI);
	}
	else
	{
		LONG_PTR lp = GetWindowLongPtrW(hwnd, GWLP_USERDATA);
		pGI = reinterpret_cast<sGameInfo*>(lp);
	}
	
	switch (uMsg)
	{
		case WM_TIMER:
		{
			pGI->y += pGI->SquareSize;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		return 0;
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_LEFT:
				{
					pGI->x -= 20;
					InvalidateRect(hwnd, NULL, TRUE);
				}
				return 0;
				case VK_RIGHT:
				{
					pGI->x += 20;
					InvalidateRect(hwnd, NULL, TRUE);
				}
				return 0;
			}
		}
		return 0;
		case WM_PAINT:
		{
			PAINTSTRUCT ps = {};
			pGI->hdc = BeginPaint(hwnd, &ps);
			SelectObject(pGI->hdc, GetStockObject(DC_BRUSH));
			SelectObject(pGI->hdc, GetStockObject(DC_PEN));
			DrawBoard(pGI);
			DrawTile(pGI, 1);
			EndPaint(hwnd, &ps);
		}
		return 0;
		case WM_DESTROY:
		{
			KillTimer(hwnd, tmr_GameTick);
			PostQuitMessage(0);
		}
		return 0;
	}
	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

int DrawSquare(sGameInfo* pGI)
{
	SetDCBrushColor(pGI->hdc, RGB(230, 230, 230));
	SetDCPenColor(pGI->hdc, RGB(30, 30, 30));
	RECT r = {};
	r.left = pGI->x;
	r.top = pGI->y;
	r.right = r.left + pGI->SquareSize;
	r.bottom = r.top + pGI->SquareSize;
	Rectangle(pGI->hdc, r.left, r.top, r.right, r.bottom);
	r.left +=pGI->SquareSize;
	//r.top = pGI->y;
	r.right += pGI->SquareSize;
	//r.bottom = r.top + pGI->SquareSize;
	Rectangle(pGI->hdc, r.left, r.top, r.right, r.bottom);
	return 0;
}

int DrawTile(sGameInfo* pGI, int type)
{
	DrawSquare(pGI);
	return 0;
}

int DrawBoard(sGameInfo* pGI)
{
	RECT r = {};
	r.left = 50;
	r.top = 50;
	r.right = r.left + 400;
	r.bottom = r.top + 600;
	SetDCBrushColor(pGI->hdc, RGB(200, 200, 200));
	SetDCPenColor(pGI->hdc, RGB(200, 200, 200));
	Rectangle(pGI->hdc, r.left, r.top, r.right, r.bottom);
	POINT apt[3] = {};
	apt[0].x = r.right;
	apt[0].y = r.top;
	apt[1].x = r.right;
	apt[1].y = r.bottom;
	apt[2].x = r.left;
	apt[2].y = r.bottom;
	SetDCBrushColor(pGI->hdc, RGB(150, 150, 150));
	SetDCPenColor(pGI->hdc, RGB(150, 150, 150));
	Polygon(pGI->hdc, apt, 3);
	r.left += 5;
	r.top += 5;
	r.right -= 5;
	r.bottom -= 5;
	SetDCBrushColor(pGI->hdc, RGB(30, 230, 30));
	SetDCPenColor(pGI->hdc, RGB(30, 230, 30));
	Rectangle(pGI->hdc, r.left, r.top, r.right, r.bottom);
	
	return 0;
}