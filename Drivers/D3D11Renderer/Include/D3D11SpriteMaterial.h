/**
* Copyright (c) 2014 Sonora Games
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
			void Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
				D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, 
				int index, int sliceSizeX, int sliceSizeY, int sheetWidth, int sheetHeight);

		private:
			struct MatrixBufferType
			{
				D3DXMATRIX world;
				D3DXMATRIX view;
				D3DXMATRIX projection;
			};

			struct SpriteSheetType
			{
				int index;
				int sliceSizeX;
				int sliceSizeY;
				int sheetWidth;
				int sheetHeight;
				int padding1;
				long long padding2;
			};

			ID3D11VertexShader* mVertexShader;
			ID3D11PixelShader* mPixelShader;
			ID3D11InputLayout* mLayout;
			ID3D11Buffer* mMatrixBuffer;
			ID3D11Buffer* mSpriteSheetBuffer;
			ID3D11SamplerState* mSamplerState;
		};
	}
}

#endif // _MONSOON_SPRITE_MATERIAL_H_