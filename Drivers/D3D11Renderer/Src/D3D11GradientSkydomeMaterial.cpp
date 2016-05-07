/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#include "D3D11GradientSkydomeMaterial.h"

using namespace Monsoon::Renderer;

D3D11GradientSkydomeMaterial::D3D11GradientSkydomeMaterial()
{
	mVertexShader = nullptr;
	mPixelShader = nullptr;
	mLayout = nullptr;
	mMatrixBuffer = nullptr;
}

D3D11GradientSkydomeMaterial::~D3D11GradientSkydomeMaterial()
{

}

bool D3D11GradientSkydomeMaterial::Load(ID3D11Device* device, HWND windowHandle)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC layoutDescription[1];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDescription;
	D3D11_BUFFER_DESC gradientBufferDescription;

	errorMessage = nullptr;
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer = nullptr;

	//
	// Compile Vertex Shader
	//
	result = D3DX11CompileFromFile("shaders/gradientskydome.vs", NULL, NULL, "SkyDomeVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vertexShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
	{
		MessageBox(windowHandle, "Error Compiling Shader", "gradientskydome.vs", MB_OK);
		return false;
	}

	//
	// Compile Pixel Shader
	//
	result = D3DX11CompileFromFile("shaders/gradientskydome.ps", NULL, NULL, "SkyDomePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&pixelShaderBuffer, &errorMessage, NULL);

	if (FAILED(result))
	{
		MessageBox(windowHandle, "Error Compiling Shader", "gradientskydome.ps", MB_OK);
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


	numElements = sizeof(layoutDescription) / sizeof(layoutDescription[0]);

	result = device->CreateInputLayout(layoutDescription, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &mLayout);
	if (FAILED(result))
		return false;

	//
	// Describe and create cbuffers. 
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

	gradientBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	gradientBufferDescription.ByteWidth = sizeof(GradientBuffer);
	gradientBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	gradientBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	gradientBufferDescription.MiscFlags = 0;
	gradientBufferDescription.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	result = device->CreateBuffer(&gradientBufferDescription, NULL, &mGradientBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//
	// Clean Up
	//
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	return true;
}

void D3D11GradientSkydomeMaterial::Release()
{
	if (mGradientBuffer)
		mGradientBuffer->Release();

	if (mMatrixBuffer)
		mMatrixBuffer->Release();

	if (mLayout)
		mLayout->Release();

	if (mPixelShader)
		mPixelShader->Release();

	if (mVertexShader)
		mVertexShader->Release();
}

void D3D11GradientSkydomeMaterial::Render(ID3D11DeviceContext* deviceContext, D3DXVECTOR4 centerColor, D3DXVECTOR4 apexColor, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	//
	// Map Material Inputs
	//
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	D3D11_MAPPED_SUBRESOURCE mappedResource2;
	MatrixBufferType* dataPtr;
	GradientBuffer* gradDataPtr;
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

	// Lock cBuffer
	deviceContext->Map(mGradientBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource2);

	gradDataPtr = (GradientBuffer*)mappedResource2.pData;

	gradDataPtr->centerColor = centerColor;
	gradDataPtr->apexColor = apexColor;

	// Unlock cBuffer
	deviceContext->Unmap(mGradientBuffer, 0);

	bufferNumber = 0;

	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &mGradientBuffer);

	//
	// Render the material.
	//

	deviceContext->IASetInputLayout(mLayout);
	deviceContext->VSSetShader(mVertexShader, NULL, 0);
	deviceContext->PSSetShader(mPixelShader, NULL, 0);
}