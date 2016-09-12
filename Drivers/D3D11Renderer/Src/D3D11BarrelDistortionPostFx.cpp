/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#include "D3D11BarrelDistortionPostFx.h"

#include <d3dx11async.h>
#include <fstream>

using namespace Monsoon::Renderer;

D3D11BarrelDistortionPostFx::D3D11BarrelDistortionPostFx()
{
	mVertexShader = nullptr;
	mPixelShader = nullptr;
	mLayout = nullptr;
	mMatrixBuffer = nullptr;
}

D3D11BarrelDistortionPostFx::~D3D11BarrelDistortionPostFx()
{

}

bool D3D11BarrelDistortionPostFx::Load(ID3D11Device* device, HWND windowHandle)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC layoutDescription[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDescription;
	D3D11_BUFFER_DESC distortionBufferDescription;
	D3D11_SAMPLER_DESC samplerDesc;

	errorMessage = nullptr;
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer = nullptr;

	//
	// Compile Vertex Shader
	//
	result = D3DX11CompileFromFile("shaders/barrel_distortion_post_fx.vs", NULL, NULL, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vertexShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
		return false;

	//
	// Compile Pixel Shader
	//
	result = D3DX11CompileFromFile("shaders/barrel_distortion_post_fx.ps", NULL, NULL, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&pixelShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
		return false;

	//
	// Create Shader Objects
	//

	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &mVertexShader);
	if (FAILED(result))
		return false;


	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &mPixelShader);
	if (FAILED(result))
		return false;

	//
	// Describe input layout and create shader objects.
	//
	layoutDescription[0].SemanticName = "POSITION";
	layoutDescription[0].SemanticIndex = 0;
	layoutDescription[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layoutDescription[0].InputSlot = 0;
	layoutDescription[0].AlignedByteOffset = 0;
	layoutDescription[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layoutDescription[0].InstanceDataStepRate = 0;

	layoutDescription[1].SemanticName = "TEXCOORD";
	layoutDescription[1].SemanticIndex = 0;
	layoutDescription[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	layoutDescription[1].InputSlot = 0;
	layoutDescription[1].AlignedByteOffset = sizeof(D3DXVECTOR3) + sizeof(D3DXCOLOR);
	layoutDescription[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layoutDescription[1].InstanceDataStepRate = 0;


	numElements = sizeof(layoutDescription) / sizeof(layoutDescription[0]);

	result = device->CreateInputLayout(layoutDescription, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &mLayout);
	if (FAILED(result))
		return false;

	//
	// Describe and create cbuffer. 
	//
	matrixBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDescription.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDescription.MiscFlags = 0;
	matrixBufferDescription.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDescription, NULL, &mMatrixBuffer);
	if (FAILED(result))
		return false;

	distortionBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	distortionBufferDescription.ByteWidth = sizeof(DistortionBufferType);
	distortionBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	distortionBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	distortionBufferDescription.MiscFlags = 0;
	distortionBufferDescription.StructureByteStride = 0;

	result = device->CreateBuffer(&distortionBufferDescription, NULL, &mDistortionBuffer);
	if (FAILED(result))
		return false;

	//
	// Describe and create sampler.
	//
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.MaxLOD = 15;

	result = device->CreateSamplerState(&samplerDesc, &mSamplerState);
	if (FAILED(result))
		return false;


	//
	// Clean Up
	//
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	return true;
}

void D3D11BarrelDistortionPostFx::Release()
{
	if (mSamplerState)
		mSamplerState->Release();

	if (mMatrixBuffer)
		mMatrixBuffer->Release();

	if (mLayout)
		mLayout->Release();

	if (mPixelShader)
		mPixelShader->Release();

	if (mVertexShader)
		mVertexShader->Release();
}

void D3D11BarrelDistortionPostFx::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, OVR::Util::Render::StereoConfig& steroConfig, bool left)
{
	//
	// Map Material Inputs
	//
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	D3D11_MAPPED_SUBRESOURCE mappedResource2;
	DistortionBufferType* dataPtr2;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;
	unsigned int bufferNumber2;

	OVR::Util::Render::StereoEyeParams eye = (left) ? steroConfig.GetEyeRenderParams(OVR::Util::Render::StereoEye::StereoEye_Left) : steroConfig.GetEyeRenderParams(OVR::Util::Render::StereoEye::StereoEye_Right);
	OVR::Util::Render::Viewport VP = eye.VP;

	float w = float(VP.w) / float(steroConfig.GetFullViewport().w),
		h = float(VP.h) / float(steroConfig.GetFullViewport().h),
		x = float(VP.x) / float(steroConfig.GetFullViewport().w),
		y = float(VP.y) / float(steroConfig.GetFullViewport().h);

	float as = float(VP.w) / float(VP.h);
	//float scaleFactor = 1.0f / steroConfig.GetDistortionScale();
	float scaleFactor = 1.0f/1.45f;

	D3DXMATRIX texMat = { w, 0, 0, x,
		0, h, 0, y,
		0, 0, 0, 0,
		0, 0, 0, 1 };

	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);
	D3DXMatrixTranspose(&texMat, &texMat);

	// Lock cBuffer
	deviceContext->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;
	dataPtr->textureMatrix = texMat;

	// Unlock cBuffer
	deviceContext->Unmap(mMatrixBuffer, 0);


	deviceContext->Map(mDistortionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource2);
	dataPtr2 = (DistortionBufferType*)mappedResource2.pData;
	dataPtr2->hmdWarpParam = { steroConfig.GetDistortionK(0), steroConfig.GetDistortionK(1), steroConfig.GetDistortionK(2), steroConfig.GetDistortionK(3) };
	dataPtr2->lensCenter = { x + (w + ((left) ? steroConfig.GetDistortionConfig().XCenterOffset : -steroConfig.GetDistortionConfig().XCenterOffset) * 0.5f)*0.5f, y + h*0.5f };
	dataPtr2->scale = { ((w / 2) * scaleFactor), (h / 2) * scaleFactor * as };
	dataPtr2->scaleIn = { (2 / w), (2 / h) / as };
	dataPtr2->screenCenter = { x + w*0.5f, y + h*0.5f };

	deviceContext->Unmap(mDistortionBuffer, 0);

	deviceContext->VSSetConstantBuffers(0, 1, &mMatrixBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &mDistortionBuffer);
	deviceContext->PSSetShaderResources(0, 1, &texture);

	//
	// Render the material.
	//

	deviceContext->IASetInputLayout(mLayout);
	deviceContext->VSSetShader(mVertexShader, NULL, 0);
	deviceContext->PSSetShader(mPixelShader, NULL, 0);
	deviceContext->PSSetSamplers(0, 1, &mSamplerState);
}