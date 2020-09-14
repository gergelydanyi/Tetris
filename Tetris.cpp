#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#include <Windows.h>
#include <windowsx.h>
#include <string>
#include "Structures.h"

constexpr auto tmr_GameTick = 1;

LRESULT CALLBACK HandleMessages(HWND, UINT, WPARAM, LPARAM);
int DrawBlock(tGameInfo*, RECT, tTriColor);
int DrawTile(tGameInfo*, int);
int DrawBoard(tGameInfo*);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	tGameInfo* pGI = new tGameInfo;
	pGI->Init();
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
		700, 100, 450, 800,
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
	tGameInfo* pGI = NULL;
	if (uMsg == WM_CREATE)
	{
		CREATESTRUCTW *pcs = reinterpret_cast<CREATESTRUCTW*>(lParam);
		pGI = reinterpret_cast<tGameInfo*>(pcs->lpCreateParams);
		SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)pGI);
	}
	else
	{
		LONG_PTR lp = GetWindowLongPtrW(hwnd, GWLP_USERDATA);
		pGI = reinterpret_cast<tGameInfo*>(lp);
	}
	
	switch (uMsg)
	{
		case WM_TIMER:
		{
			pGI->ActiveTile.MoveDown();
			InvalidateRect(hwnd, NULL, TRUE);
		}
		return 0;
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_LEFT:
				{
					pGI->ActiveTile.MoveLeft();
					InvalidateRect(hwnd, NULL, TRUE);
				}
				return 0;
				case VK_RIGHT:
				{
					pGI->ActiveTile.MoveRight();
					InvalidateRect(hwnd, NULL, TRUE);
				}
				return 0;
				case VK_DOWN:
				{
					pGI->ActiveTile.MoveDown();
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
			DrawTile(pGI, 3);
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

int DrawBlock(tGameInfo* pGI, RECT r, tTriColor tricolor)
{
	POINT apt[3];
	{
		SetDCBrushColor(pGI->hdc, tricolor.colour[0]);
		SetDCPenColor(pGI->hdc, tricolor.colour[0]);
		Rectangle(pGI->hdc, r.left, r.top, r.right, r.bottom);
//		MoveToEx(pGI->hdc, r.left, r.top, NULL);
//		SetDCPenColor(pGI->hdc, RGB(243, 97, 97));
//		LineTo(pGI->hdc, r.left + 4, r.top + 4);
		SetPixel(pGI->hdc, r.left, r.top, pGI->GBColor.colour[2]);
	}
	{
		SetDCBrushColor(pGI->hdc, tricolor.colour[1]);
		SetDCPenColor(pGI->hdc, tricolor.colour[1]);
		apt[0].x = r.right - 1;
		apt[0].y = r.top + 1;
		apt[1].x = r.right - 1;
		apt[1].y = r.bottom - 1;
		apt[2].x = r.left + 1;
		apt[2].y = r.bottom - 1;
		Polygon(pGI->hdc, apt, 3);
	}
	{
		SetDCBrushColor(pGI->hdc, tricolor.colour[2]);
		SetDCPenColor(pGI->hdc, tricolor.colour[2]);
		MoveToEx(pGI->hdc, r.right - 1, r.top, NULL);
		LineTo(pGI->hdc, r.left - 1, r.bottom);
		SetPixel(pGI->hdc, r.right - 1, r.top, pGI->GBColor.colour[2]);
		SetPixel(pGI->hdc, r.left, r.bottom - 1, pGI->GBColor.colour[2]);
		SetPixel(pGI->hdc, r.right-1, r.bottom - 1, pGI->GBColor.colour[2]);
		r.left += 3;
		r.top += 3;
		r.right -= 3;
		r.bottom -= 3;
		Rectangle(pGI->hdc, r.left, r.top, r.right, r.bottom);
	}
	return 0;
}

int DrawTile(tGameInfo* pGI, int type)
{
	RECT r = {};
	r.left = pGI->GameBoard.left + pGI->GameBoard.borderWidth + pGI->ActiveTile.left * pGI->BlockSize;
	r.top = pGI->GameBoard.top + pGI->GameBoard.borderWidth + pGI->ActiveTile.bottom * pGI->BlockSize;
	r.right = r.left + pGI->BlockSize;
	r.bottom = r.top + pGI->BlockSize;

	for (int i = 0; i < 4; ++i)
	{
		r.left += pGI->Tiles[type].parts.p[i].x * pGI->BlockSize;
		r.top += pGI->Tiles[type].parts.p[i].y * pGI->BlockSize;
		r.right = r.left + pGI->BlockSize;
		r.bottom = r.top + pGI->BlockSize;
		if (r.top > pGI->GameBoard.top+pGI->GameBoard.borderWidth)
		{
			DrawBlock(pGI, r, pGI->TriColor[type]);
		}
	}
	return 0;
}

int DrawBoard(tGameInfo* pGI)
{
	RECT r = {};
	POINT apt[3] = {};
	{
		SetDCBrushColor(pGI->hdc, pGI->GBColor.colour[0]);
		SetDCPenColor(pGI->hdc, pGI->GBColor.colour[0]);
		r.left = pGI->GameBoard.left;
		r.top = pGI->GameBoard.top;
		r.right = r.left + pGI->GameBoard.borderWidth * 2 + pGI->GameBoard.width * pGI->BlockSize;
		r.bottom = r.top + pGI->GameBoard.borderWidth * 2 + pGI->GameBoard.height * pGI->BlockSize;
		Rectangle(pGI->hdc, r.left, r.top, r.right, r.bottom);
	}
	{
		SetDCBrushColor(pGI->hdc, pGI->GBColor.colour[1]);
		SetDCPenColor(pGI->hdc, pGI->GBColor.colour[1]);
		apt[0].x = r.right - 1;
		apt[0].y = r.top;
		apt[1].x = r.right - 1;
		apt[1].y = r.top + pGI->GameBoard.borderWidth-1;
		apt[2].x = r.right - pGI->GameBoard.borderWidth;
		apt[2].y = r.top + pGI->GameBoard.borderWidth - 1;
		Polygon(pGI->hdc, apt, 3);
		SetDCPenColor(pGI->hdc, pGI->GBColor.colour[2]);
		MoveToEx(pGI->hdc, apt[0].x, apt[0].y, NULL);
		LineTo(pGI->hdc, apt[2].x - 1, apt[2].y + 1);
	}
	{
		SetDCPenColor(pGI->hdc, pGI->GBColor.colour[1]);
		apt[0].x = r.left + pGI->GameBoard.borderWidth - 1;
		apt[0].y = r.bottom - pGI->GameBoard.borderWidth;
		apt[1].x = r.left + pGI->GameBoard.borderWidth - 1;
		apt[1].y = r.bottom - 1;
		apt[2].x = r.left;
		apt[2].y = r.bottom - 1;
		Polygon(pGI->hdc, apt, 3);
		SetDCPenColor(pGI->hdc, pGI->GBColor.colour[2]);
		MoveToEx(pGI->hdc, apt[0].x, apt[0].y, NULL);
		LineTo(pGI->hdc, apt[2].x - 1, apt[2].y + 1);
	}
	{
		SetDCPenColor(pGI->hdc, pGI->GBColor.colour[1]);
		r.left = pGI->GameBoard.left + pGI->GameBoard.borderWidth;
		r.right = r.left + pGI->GameBoard.borderWidth + pGI->GameBoard.width * pGI->BlockSize;
		r.top = pGI->GameBoard.top + pGI->GameBoard.borderWidth;
		r.bottom = r.top + pGI->GameBoard.height * pGI->BlockSize + pGI->GameBoard.borderWidth;
		Rectangle(pGI->hdc, r.left, r.top, r.right, r.bottom);
	}
	{
		SetDCBrushColor(pGI->hdc, pGI->GBColor.colour[2]);
		SetDCPenColor(pGI->hdc, pGI->GBColor.colour[2]);
		r.right = r.right - pGI->GameBoard.borderWidth;
		r.bottom = r.bottom - pGI->GameBoard.borderWidth;
		Rectangle(pGI->hdc, r.left, r.top, r.right, r.bottom);
	}
	return 0;
}
