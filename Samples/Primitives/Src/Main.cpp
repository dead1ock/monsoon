/**
* Copyright (c) 2014 Sonora Games
*
*/


#include <Platform/Platform.h>

#ifdef MONSOON_OS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <App/Application.h>
#include <D3D11Renderer.h>

using namespace monsoon;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	renderer::DX11Renderer* renderer = new renderer::DX11Renderer(renderer::RendererSettings());
	Application application((renderer::Renderer*)renderer);
	application.Run();
	return 0;
}

#endif