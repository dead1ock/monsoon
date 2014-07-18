/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_DX11_WINDOW_H_
#define _MONSOON_DX11_WINDOW_H_

#include <Windows.h>

#include <platform/types.h>

namespace monsoon {
	namespace renderer {
		class DX11Window
		{
		public:
			DX11Window(std::string windowName, unsigned short screenWidth, unsigned short screenHeight, bool fullscreen);
			~DX11Window();

			bool Initialize();

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