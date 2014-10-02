/**
* Copyright (c) 2014 Sonora Games
*
*/

#include "D3D11VertexBuffer.h"

using namespace Monsoon::Renderer;


D3D11VertexBuffer::D3D11VertexBuffer() {
	
}

D3D11VertexBuffer::~D3D11VertexBuffer() {

}

void D3D11VertexBuffer::Allocate(ID3D11Device* device, VertexType vertices[], int vertexCount, unsigned int indices[], int indexCount) {
	mDevice = device;

	D3D11VertexType* d3dVertices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	mVertexCount = vertexCount;
	mIndexCount = indexCount;

	d3dVertices = new D3D11VertexType[mVertexCount];

	for (int x = 0; x < vertexCount; x++)
	{
		d3dVertices[x].position = D3DXVECTOR3(vertices[x].x, vertices[x].y, vertices[x].z);
		d3dVertices[x].color = D3DXCOLOR(vertices[x].r, vertices[x].g, vertices[x].b, vertices[x].a);
		d3dVertices[x].texture = D3DXVECTOR2(vertices[x].u, vertices[x].v);
	}

	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = sizeof(D3D11VertexType)* mVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = d3dVertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);

	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = sizeof(unsigned int)* mIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
}

void D3D11VertexBuffer::Free()
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

void D3D11VertexBuffer::Render(ID3D11DeviceContext* context) {
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(D3D11VertexType);
	offset = 0;

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(mIndexCount, 0, 0);
	
}