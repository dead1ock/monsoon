/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef _MONSOON_DX11_VERTEX_BUFFER_H_
#define _MONSOON_DX11_VERTEX_BUFFER_H_

#include <Renderer/VertexBuffer.h>

#include "D3D.h"

namespace Monsoon {
	namespace Renderer {

		struct D3D11VertexType
		{
			D3DXVECTOR3 position;
			D3DXCOLOR color;
			D3DXVECTOR2 texture;
		};

		class D3D11VertexBuffer : public VertexBuffer
		{
		public:
			D3D11VertexBuffer();
			~D3D11VertexBuffer();

			void Allocate(ID3D11Device* device, VertexType vertices[], int vertexCount, unsigned int indicies[], int indexCount);
			void Free();
			int GetVertexCount() { return mVertexCount; }
			int GetIndexCount() { return mIndexCount; }

			void Render(ID3D11DeviceContext* deviceContext);

		private:
			ID3D11Device* mDevice;
			ID3D11Buffer* mVertexBuffer, *mIndexBuffer;
			int mVertexCount, mIndexCount;

		};
	}
}

#endif // _MONSOON_DX11_VERTEX_BUFFER_H_