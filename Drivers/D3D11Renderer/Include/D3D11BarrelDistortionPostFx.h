/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef _MONSOON_DX11_BARREL_DISTORTION_POSTFX_
#define _MONSOON_DX11_BARREL_DISTORTION_POSTFX_

#include "D3D.h"
#include "OVR.h"

class D3D11BarrelDistortionPostFx
{
public:
	D3D11BarrelDistortionPostFx();
	~D3D11BarrelDistortionPostFx();

	bool Load(ID3D11Device* device, HWND windowHandle);
	void Release();

	void Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
		D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, OVR::Util::Render::StereoConfig& steroConfig, bool left = true);

private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
		D3DXMATRIX textureMatrix;
	};

	struct DistortionBufferType
	{
		D3DXVECTOR2 lensCenter;
		D3DXVECTOR2 screenCenter;
		D3DXVECTOR2 scale;
		D3DXVECTOR2 scaleIn;
		D3DXVECTOR4 hmdWarpParam;
	};

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;
	ID3D11Buffer* mMatrixBuffer;
	ID3D11Buffer* mDistortionBuffer;
	ID3D11SamplerState* mSamplerState;
};

#endif // _MONSOON_DX11_BARREL_DISTORTION_POSTFX_