/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef _MONSOON_DX11_WINDOW_H_
#define _MONSOON_DX11_WINDOW_H_

#include <Windows.h>

#include <platform/types.h>

namespace Monsoon {
	namespace Renderer {
		class D3D11Window
		{
		public:
			D3D11Window(std::string windowName, unsigned short screenWidth, unsigned short screenHeight, bool fullscreen);
			~D3D11Window();

			bool Initialize();
			bool Initialize(unsigned short scrWidth, unsigned short scrHeight);

			void Shutdown();

			bool Update();

			std::string getWindowName() { return mWindowName; }
			U16 getWidth() { return mScreenWidth; }
			U16 getHeight() { return mScreenHeight; }
			bool isFullscreen() { return mFullscreen; }


			HWND getHandle() { return mHWND; }

		private:
			std::string mWindowName;
			U16 mScreenWidth;
			U16 mScreenHeight;
			bool mFullscreen;
			HWND mHWND;
			HINSTANCE mHINSTANCE;
			MSG msg;
		};
	} // namespace renderer
} // namespace monsoon

#endif // _MONSOON_DX11_WINDOW_H_