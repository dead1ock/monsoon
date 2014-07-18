/**
* Copyright (c) 2014 Sonora Games
*
*/

#include "D3D11VertexBuffer.h"

using namespace monsoon::renderer;


DX11VertexBuffer::DX11VertexBuffer() {
	
}

DX11VertexBuffer::~DX11VertexBuffer() {

}

void DX11VertexBuffer::Allocate(ID3D11Device* device) {
	mDevice = device;

	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	mVertexCount = 4;
	mIndexCount = 6;

	vertices = new VertexType[mVertexCount];
	indices = new unsigned long[mIndexCount];

	//
	// Temporary Vertex Buffer Input
	//
	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);  // Top left
	vertices[1].color = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);  // Top right.
	vertices[2].color = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

	vertices[3].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[3].color = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	//
	// Temporary Index Buffer Input
	//
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top left.
	indices[2] = 2;  // Top right.
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* mVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* mIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
}

void DX11VertexBuffer::Free()
{
	if (mIndexBuffer)
	{
		mIndexBuffer->Release();
		mIndexBuffer = 0;
	}

	if (mVertexBuffer)
	{
		mVertexBuffer->Release();
		mVertexBuffer = 0;
	}
}

void DX11VertexBuffer::Render(ID3D11DeviceContext* context) {
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}