/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_DX11_RENDERER_
#define _MONSOON_DX11_RENDERER_

#include <vector>

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

			int CreateVertexBuffer(ColorVertex* vertices, int vertexCount, unsigned long* indicies, int indexCount);
			void DestroyVertexBuffer(int vbHandle);

		private:
			D3D11Window mWindow;
			D3D mD3d;
			std::vector<D3D11VertexBuffer> mVertexBuffers;
			std::vector<int> mFreeIndexList;
			D3D11ColorMaterial mColorMaterial;
		};
	}
}

#endif