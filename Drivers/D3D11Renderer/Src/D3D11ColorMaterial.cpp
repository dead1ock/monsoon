/**
* Copyright (c) 2014 Sonora Games
*
*/

#include <d3dx11async.h>

#include "D3D11ColorMaterial.h"

using namespace Monsoon::Renderer;

D3D11ColorMaterial::D3D11ColorMaterial()
{
	mVertexShader = nullptr;
	mPixelShader = nullptr;
	mLayout = nullptr;
	mMatrixBuffer = nullptr;
}

D3D11ColorMaterial::~D3D11ColorMaterial()
{

}

bool D3D11ColorMaterial::Load(ID3D11Device* device, HWND windowHandle)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC layoutDescription[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDescription;

	errorMessage = nullptr;
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer = nullptr;

	//
	// Compile Vertex Shader
	//
	result = D3DX11CompileFromFile("color.vs", NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vertexShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
	{
		MessageBox(windowHandle, "Error Compiling Shader", "color.vs", MB_OK);
		return false;
	}

	//
	// Compile Pixel Shader
	//
	result = D3DX11CompileFromFile("color.ps", NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&pixelShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
	{
		MessageBox(windowHandle, "Error Compiling Shader", "color.ps", MB_OK);
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

	layoutDescription[1].SemanticName = "COLOR";
	layoutDescription[1].SemanticIndex = 0;
	layoutDescription[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	layoutDescription[1].InputSlot = 0;
	layoutDescription[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
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


	//
	// Clean Up
	//
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	return true;
}

void D3D11ColorMaterial::Release()
{
	if (mMatrixBuffer)
		mMatrixBuffer->Release();

	if (mLayout)
		mLayout->Release();

	if (mPixelShader)
		mPixelShader->Release();

	if (mVertexShader)
		mVertexShader->Release();
}

void D3D11ColorMaterial::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	//
	// Map Material Inputs
	//
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	// Lock cBuffer
	deviceContext->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock cBuffer
	deviceContext->Unmap(mMatrixBuffer, 0);

	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);

	//
	// Render the material.
	//

	deviceContext->IASetInputLayout(mLayout);
	deviceContext->VSSetShader(mVertexShader, NULL, 0);
	deviceContext->PSSetShader(mPixelShader, NULL, 0);
}