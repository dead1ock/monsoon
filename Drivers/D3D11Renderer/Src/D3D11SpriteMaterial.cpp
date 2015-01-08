/**
* Copyright (c) 2014 Sonora Games
*
*/

#include <d3dx11async.h>
#include <iostream>
#include <fstream>

#include "D3D11SpriteMaterial.h"

using namespace Monsoon::Renderer;

D3D11SpriteMaterial::D3D11SpriteMaterial()
{
	mVertexShader = nullptr;
	mPixelShader = nullptr;
	mLayout = nullptr;
	mMatrixBuffer = nullptr;
}

D3D11SpriteMaterial::~D3D11SpriteMaterial()
{

}

bool D3D11SpriteMaterial::Load(ID3D11Device* device, HWND windowHandle)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC layoutDescription[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDescription;
	D3D11_BUFFER_DESC spriteSheetBufferDescription;
	D3D11_SAMPLER_DESC samplerDesc;

	errorMessage = nullptr;
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer = nullptr;

	//
	// Compile Vertex Shader
	//
	result = D3DX11CompileFromFile("sprite.vs", NULL, NULL, "SpriteVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vertexShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
	{
		MessageBox(windowHandle, "Error Compiling Shader", "sprite.vs", MB_OK);
		return false;
	}

	//
	// Compile Pixel Shader
	//
	result = D3DX11CompileFromFile("sprite.ps", NULL, NULL, "SpritePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&pixelShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
	{
		MessageBox(windowHandle, "Error Compiling Shader", "sprite.ps", MB_OK);
		return false;
	}

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

	spriteSheetBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	spriteSheetBufferDescription.ByteWidth = sizeof(SpriteSheetType);
	spriteSheetBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	spriteSheetBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	spriteSheetBufferDescription.MiscFlags = 0;
	spriteSheetBufferDescription.StructureByteStride = 0;

	result = device->CreateBuffer(&spriteSheetBufferDescription, NULL, &mSpriteSheetBuffer);
	if (FAILED(result))
		return false;

	//
	// Describe and create sampler.
	//
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

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

void D3D11SpriteMaterial::Release()
{
	if (mSamplerState)
		mSamplerState->Release();

	if (mSpriteSheetBuffer)
		mSpriteSheetBuffer->Release();

	if (mMatrixBuffer)
		mMatrixBuffer->Release();

	if (mLayout)
		mLayout->Release();

	if (mPixelShader)
		mPixelShader->Release();

	if (mVertexShader)
		mVertexShader->Release();
}

void D3D11SpriteMaterial::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, int index, int sliceSizeX, int sliceSizeY, int sheetWidth, int sheetHeight)
{
	//
	// Map Material Inputs
	//
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	D3D11_MAPPED_SUBRESOURCE spriteSheetMappedResource;
	MatrixBufferType* dataPtr;
	SpriteSheetType* spriteSheetDataPtr;

	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	// Lock Matrix cBuffer
	deviceContext->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock Matrix cBuffer
	deviceContext->Unmap(mMatrixBuffer, 0);

	// Lock SpriteSheet cBuffer
	deviceContext->Map(mSpriteSheetBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &spriteSheetMappedResource);

	spriteSheetDataPtr = (SpriteSheetType*)spriteSheetMappedResource.pData;

	spriteSheetDataPtr->index = index;
	spriteSheetDataPtr->sliceSizeX = sliceSizeX;
	spriteSheetDataPtr->sliceSizeY = sliceSizeY;
	spriteSheetDataPtr->sheetWidth = sheetWidth;
	spriteSheetDataPtr->sheetHeight = sheetHeight;

	// Unload SpriteSheet cBuffer
	deviceContext->Unmap(mSpriteSheetBuffer, 0);


	deviceContext->VSSetConstantBuffers(0, 1, &mMatrixBuffer);
	deviceContext->VSSetConstantBuffers(1, 1, &mSpriteSheetBuffer);
	deviceContext->PSSetShaderResources(0, 1, &texture);

	//
	// Render the material.
	//

	deviceContext->IASetInputLayout(mLayout);
	deviceContext->VSSetShader(mVertexShader, NULL, 0);
	deviceContext->PSSetShader(mPixelShader, NULL, 0);
	deviceContext->PSSetSamplers(0, 1, &mSamplerState);
}