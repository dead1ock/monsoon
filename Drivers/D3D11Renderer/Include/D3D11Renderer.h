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
		class DRIVER D3D11Renderer : Renderer
		{
		public:
			D3D11Renderer(RendererSettings& settings);
			~D3D11Renderer();

			bool Initialize();
			void Shutdown();
			bool Update();

		private:
			D3D11Window mWindow;
			D3D mD3d;
			D3D11VertexBuffer mVertexBuffer;
			D3D11ColorMaterial mColorMaterial;
		};
	}
}

#endif