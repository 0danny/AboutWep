#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C"
{
	//Ordinal 1
	__declspec(dllexport) INT_PTR CALLBACK AboutDlgProc(HWND p_hWnd, UINT p_message, WPARAM p_wParam, LPARAM p_lParam);
	//Ordinal 2
	__declspec(dllexport) void AboutWEP(HWND p_hWnd, HICON p_hIcon, LPSTR p_text1Content, LPSTR p_text2Content);
	//Ordinal 3
	__declspec(dllexport) void WEP();
}

//AboutDlgProc
const char* g_aboutString = "About ";

//AboutWEP
HICON g_hIcon;
LPSTR g_text1Content;
LPSTR g_text2Content;
LPSTR g_aboutBuffer;
int g_wndCount = 0;
HDC g_aboutWepHDC;
HBITMAP g_aboutWepImage;

//Entry
BOOL g_isSmallWindow;
BOOL g_bitsNot2;
HINSTANCE g_hInstance;

INT_PTR CALLBACK AboutDlgProc(HWND p_hWnd, UINT p_message, WPARAM p_wParam, LPARAM p_lParam)
{
	int l_screenHeight;
	int l_screenWidth;
	tagRECT l_wndRect;
	int l_cx;
	int l_cy;
	UINT l_uFlags;

	if (p_message == 43) {
		if ((*(int*)(p_lParam + 4) == 301) && (g_aboutWepHDC != (HDC)0x0)) {
			BitBlt(*(HDC*)(p_lParam + 0x18), 0, 0, 260, 65, g_aboutWepHDC, 0, 0, 0xcc0020);
		}
	}
	else {
		/* WM_SYSCOMMAND - This message is sent to a window when the user selects a command from the window's system menu. */
		if (p_message == 272) {
			/* Set the window text to "About" */
			SetWindowTextA(p_hWnd, g_aboutString);
			if (g_hIcon != 0) {
				SendDlgItemMessageA(p_hWnd, 201, 368, (WPARAM)g_hIcon, 0);
			}
			if (g_text1Content != (LPSTR)0x0) {
				/* Set the text 1 content. */
				SetDlgItemTextA(p_hWnd, 202, g_text1Content);
			}
			if (g_text2Content != (LPSTR)0x0) {
				/* Set the text 2 content. */
				SetDlgItemTextA(p_hWnd, 203, g_text2Content);
			}
			CreateWindowExA(0, "button", "", 0x5000000b, 10, 10, 260, 65, p_hWnd, (HMENU)301, g_hInstance, (LPVOID)0x0);
			GetWindowRect(p_hWnd, &l_wndRect);
			l_uFlags = 17;
			l_cy = 0;
			l_cx = 0;
			l_screenHeight = GetSystemMetrics(1);
			l_screenHeight = (l_screenHeight - (l_wndRect.bottom - l_wndRect.top)) / 3;
			l_screenWidth = GetSystemMetrics(0);
			SetWindowPos(p_hWnd, (HWND)0, l_screenWidth - (l_wndRect.right - l_wndRect.left) >> 1, l_screenHeight, l_cx, l_cy, l_uFlags);
			return 1;
		}
		/* WM_COMMAND - This message is sent to a window or control when the user selects a command from a menu, clicks a button. */
		if (((p_message == 273) && ((p_wParam & 0xffff) != 0)) && ((p_wParam & 0xffff) < 3)) {
			EndDialog(p_hWnd, 1);
			return 1;
		}
	}
	return 0;
}

void AboutWEP(HWND p_hWnd, HICON p_hIcon, LPSTR p_text1Content, LPSTR p_text2Content)
{
	int l_wndCount;

	/* 0x13d0  2  AboutWEP */
	g_hIcon = p_hIcon;
	g_text1Content = p_text1Content;
	g_text2Content = p_text2Content;
	/* Read the current window title into g_aboutText. */
	GetWindowTextA(p_hWnd, g_aboutBuffer, 73);
	l_wndCount = g_wndCount;
	/* Gonna assume this is to ensure we don't open more than 1 dialog window or something. */
	g_wndCount = g_wndCount + 1;
	if (l_wndCount == 0) {
		g_aboutWepHDC = CreateCompatibleDC((HDC)0x0);
		/* Load in either the 666 or 999 bitmap depending on color capabilities. */
		g_aboutWepImage = LoadBitmapA(g_hInstance, (LPCSTR)(((UINT)(g_bitsNot2 == 0) & 333) + 666));
		if ((g_aboutWepHDC != (HDC)0x0) && (g_aboutWepImage != (HBITMAP)0x0)) {
			SelectObject(g_aboutWepHDC, g_aboutWepImage);
		}
	}
	/* Open up the larger dialog if we have space. */
	DialogBoxParamA(g_hInstance, (LPCSTR)(101 - (UINT)(g_isSmallWindow == 0)), p_hWnd, AboutDlgProc, 0);

	g_wndCount = g_wndCount + -1;

	if ((g_wndCount == 0) && (g_aboutWepHDC != (HDC)0x0)) {
		DeleteDC(g_aboutWepHDC);
		DeleteObject(g_aboutWepImage);
	}
	return;
}

void WEP()
{
	//I have no idea what this is.
	return;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		//Start doing some shit
		int l_return;
		HWND l_desktopHWND;
		HDC l_hdc;

		/* Get the height of primary monitor in pixels. */
		l_return = GetSystemMetrics(1);

		/* If window is less than 351 pixels in height. */
		g_isSmallWindow = (BOOL)(l_return < 351);

		if (g_isSmallWindow == 0) {
			l_desktopHWND = GetDesktopWindow();
			l_hdc = GetDC(l_desktopHWND);

			/* Retrieve the bits per pixel of the current machine. */
			l_return = GetDeviceCaps(l_hdc, 24);
			g_bitsNot2 = (BOOL)(l_return != 2);
			l_desktopHWND = GetDesktopWindow();
			ReleaseDC(l_desktopHWND, l_hdc);
		}
		else {
			g_bitsNot2 = 0;
		}

		g_hInstance = hModule;

	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

