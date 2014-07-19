/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_DX11_RENDERER_
#define _MONSOON_DX11_RENDERER_

#include <Renderer/Renderer.h>

#include "D3D.h"
#include "D3D11Window.h"
#include "D3D11ColorMaterial.h"


namespace Monsoon {
	namespace Renderer{
		class DRIVER DX11Renderer : Renderer
		{
		public:
			DX11Renderer(RendererSettings& settings);
			~DX11Renderer();

			bool Initialize();
			void Shutdown();
			bool Update();

		private:
			DX11Window mWindow;
			D3D mD3d;
			DX11VertexBuffer mVertexBuffer;
			ColorMaterial mColorMaterial;
		};
	}
}

#endif