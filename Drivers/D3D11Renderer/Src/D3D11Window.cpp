/**
* Copyright (c) 2014 Sonora Games
*
*/

#include "D3D11Window.h"

#include <Windows.h>

using namespace Monsoon::Renderer;

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

DX11Window::DX11Window(std::string windowName, unsigned short screenWidth, unsigned short screenHeight, bool fullscreen) {
	ZeroMemory(&msg, sizeof(MSG));
	mFullscreen = fullscreen;
	mWindowName = windowName;
	mScreenHeight = screenHeight;
	mScreenWidth = screenWidth;
}

DX11Window::~DX11Window() {

}

bool DX11Window::Initialize()
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	mHINSTANCE = GetModuleHandle(NULL);

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mHINSTANCE;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = mWindowName.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	if (mFullscreen)
	{
		// Query desktop resolution.
		mScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		mScreenHeight = GetSystemMetrics(SM_CYSCREEN);

		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)mScreenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)mScreenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else
	{
		posX = (GetSystemMetrics(SM_CXSCREEN) - mScreenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - mScreenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	mHWND = CreateWindowEx(WS_EX_APPWINDOW, mWindowName.c_str(), mWindowName.c_str(), WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, mScreenWidth, mScreenHeight, NULL, NULL, mHINSTANCE, NULL);

	ShowWindow(mHWND, SW_SHOW);
	SetForegroundWindow(mHWND);
	SetFocus(mHWND);

	return true;
}

void DX11Window::Shutdown()
{

	// Leave full screen mode.
	if (mFullscreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(mHWND);
	mHWND = NULL;

	UnregisterClass(mWindowName.c_str(), GetModuleHandle(NULL));
	mHINSTANCE = NULL;
	return;
}

bool DX11Window::Update()
{
	// Handle the windows messages.
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// If windows signals to end the application then exit out.
	if (msg.message == WM_QUIT)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uMessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		// Check if the window is being closed.
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, uMessage, wParam, lParam);
	}
}