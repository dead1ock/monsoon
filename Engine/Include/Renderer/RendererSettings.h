/**
* Copyright (c) 2014-2016 Dallin Wellington
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
			/*
			 * Initializes the default settings for the Renderer.
			 */
			RendererSettings() {
				screenWidth = 2560;
				screenHeight = 1440;
				fullscreen = true;
				showCursor = true;
				windowName = "Monsoon Game Engine";
				enablePostProcessing = true;
				enableVR = false;
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
			bool enableVR;
			bool enablePostProcessing;
			std::string windowName;
		};

	} // namespace renderer
} // namespace monsoon

#endif // _MONSOON_RENDERER_SETTINGS_H_