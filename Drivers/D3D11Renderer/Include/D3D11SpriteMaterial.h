/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef _MONSOON_SPRITE_MATERIAL_H_
#define _MONSOON_SPRITE_MATERIAL_H_

#include "D3D11VertexBuffer.h"

namespace Monsoon {
	namespace Renderer {

		class D3D11SpriteMaterial
		{
		public:
			D3D11SpriteMaterial();
			~D3D11SpriteMaterial();

			bool Load(ID3D11Device* device, HWND windowHandle);
			void Release();
			void SetTexture(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture);
			void SetAtlasBuffer(ID3D11DeviceContext* deviceContext, float srcWidth, float srcHeight, float srcU, float srcV);
			void SetMatrixBuffer(ID3D11DeviceContext* deviceContext, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection);
			void Render(ID3D11DeviceContext* deviceContext);

		private:
			struct MatrixBufferType
			{
				D3DXMATRIX world;
				D3DXMATRIX view;
				D3DXMATRIX projection;
			};

			struct AtlasBuffer
			{
				float srcWidth;
				float srcHeight;
				float srcU;
				float srcV;
			};

			ID3D11VertexShader* mVertexShader;
			ID3D11PixelShader* mPixelShader;
			ID3D11InputLayout* mLayout;
			ID3D11Buffer* mMatrixBuffer;
			ID3D11Buffer* mAtlasBuffer;
			ID3D11SamplerState* mSamplerState;
		};
	}
}

#endif // _MONSOON_SPRITE_MATERIAL_H_