/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_DX11_VERTEX_BUFFER_H_
#define _MONSOON_DX11_VERTEX_BUFFER_H_

#include <Renderer/VertexBuffer.h>

#include "D3D.h"

namespace Monsoon {
	namespace Renderer {

		struct VertexType
		{
			D3DXVECTOR3 position;
			D3DXCOLOR color;
		};

		class D3D11VertexBuffer : public VertexBuffer
		{
		public:
			D3D11VertexBuffer();
			~D3D11VertexBuffer();

			void Allocate(ID3D11Device* device, ColorVertex* vertices, int vertexCount, unsigned int* indicies, int indexCount);
			void Free();

			void Render(ID3D11DeviceContext* deviceContext);

		private:
			ID3D11Device* mDevice;
			ID3D11Buffer* mVertexBuffer, *mIndexBuffer;
			int mVertexCount, mIndexCount;

		};
	}
}

#endif // _MONSOON_DX11_VERTEX_BUFFER_H_