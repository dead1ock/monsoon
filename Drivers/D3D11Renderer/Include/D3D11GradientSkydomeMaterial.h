/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef _MONSOON_GRADIENT_SKYDOME_MATERIAL_H_
#define _MONSOON_GRADIENT_SKYDOME_MATERIAL_H_

#include "D3D11VertexBuffer.h"

#include <d3dx11async.h>

namespace Monsoon {
	namespace Renderer {

		class D3D11GradientSkydomeMaterial
		{
		public:
			D3D11GradientSkydomeMaterial();
			~D3D11GradientSkydomeMaterial();

			bool Load(ID3D11Device* device, HWND windowHandle);
			void Release();
			void Render(ID3D11DeviceContext* deviceContext, D3DXVECTOR4 centerColor, D3DXVECTOR4 apexColor, D3DXMATRIX worldMatrix,
				D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

		private:
			struct MatrixBufferType
			{
				D3DXMATRIX world;
				D3DXMATRIX view;
				D3DXMATRIX projection;
			};

			struct GradientBuffer
			{
				D3DXVECTOR4 apexColor;
				D3DXVECTOR4 centerColor;
			};

			ID3D11VertexShader* mVertexShader;
			ID3D11PixelShader* mPixelShader;
			ID3D11InputLayout* mLayout;
			ID3D11Buffer* mMatrixBuffer;
			ID3D11Buffer* mGradientBuffer;
		};

	}
}

#endif // _MONSOON_GRADIENT_SKYDOME_MATERIAL_H_