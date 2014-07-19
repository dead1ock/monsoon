/**
 * Copyright (c) 2014 Sonora Games
 *
 */

#ifndef _MONSOON_COLOR_MATERIAL_H_
#define _MONSOON_COLOR_MATERIAL_H_

#include "D3D11VertexBuffer.h"

namespace Monsoon {
	namespace Renderer {

		class ColorMaterial
		{
		public:
			ColorMaterial();
			~ColorMaterial();

			bool Load(ID3D11Device* device, HWND windowHandle);
			void Release();
			void Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix,
				D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

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
		};

	}
}

#endif