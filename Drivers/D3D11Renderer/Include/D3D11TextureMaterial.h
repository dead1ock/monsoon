/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef _MONSOON_TEXTURE_MATERIAL_H_
#define _MONSOON_TEXTURE_MATERIAL_H_

#include "D3D11VertexBuffer.h"

namespace Monsoon {
	namespace Renderer {

		class D3D11TextureMaterial
		{
		public:
			D3D11TextureMaterial();
			~D3D11TextureMaterial();

			bool Load(ID3D11Device* device, HWND windowHandle);
			void Release();
			void Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
				D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);

		private:
			struct MatrixBufferType
			{
				D3DXMATRIX world;
				D3DXMATRIX view;
				D3DXMATRIX projection;
			};

			ID3D11VertexShader* mVertexShader;
			ID3D11PixelShader* mPixelShader;
			ID3D11InputLayout* mLayout;
			ID3D11Buffer* mMatrixBuffer;
			ID3D11SamplerState* mSamplerState;
		};
	}
}

#endif // _MONSOON_TEXTURE_MATERIAL_H_