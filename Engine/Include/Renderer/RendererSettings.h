/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_RENDERER_SETTINGS_H_
#define _MONSOON_RENDERER_SETTINGS_H_

#include <platform/types.h>

namespace Monsoon {
	namespace Renderer {

		/**
		* Renderer Settings
		*/
		struct RendererSettings
		{
			RendererSettings() {
				screenWidth = 1920;
				screenHeight = 1080;
				fullscreen = false;
				showCursor = true;
				windowName = "Monsoon Game Engine";
			}

			RendererSettings(U16 scrWidth, U16 scrHeight, std::string windName, bool scrFull = false, bool showCur = false) {
				screenWidth = scrWidth;
				screenHeight = scrHeight;
				windowName = windName;
				fullscreen = scrFull;
				showCursor = showCur;
			}

			U16 screenWidth;
			U16 screenHeight;
			bool fullscreen;
			bool showCursor;
			std::string windowName;
		};

	} // namespace renderer
} // namespace monsoon

#endif // _MONSOON_RENDERER_SETTINGS_H_