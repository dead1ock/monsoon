/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_RENDERER_SETTINGS_H_
#define _MONSOON_RENDERER_SETTINGS_H_

#include <platform/types.h>

namespace monsoon {
	namespace renderer {

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

			U16 screenWidth;
			U16 screenHeight;
			bool fullscreen;
			bool showCursor;
			std::string windowName;
		};

	} // namespace renderer
} // namespace monsoon

#endif // _MONSOON_RENDERER_SETTINGS_H_