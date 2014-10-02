/**
* Copyright (c) 2014 Sonora Games
*
*/

#include <Windows.h>

#include "D3D11Renderer.h"
#include "D3D11VertexBuffer.h"

using namespace Monsoon;
using namespace Monsoon::Renderer;

D3D11Renderer::D3D11Renderer(RendererSettings& settings, Scene::SpatialSystem* spatialSystem)
: Renderer(settings, spatialSystem),
  mSpatialSystem(spatialSystem),
  mWindow(settings.windowName, settings.screenWidth, settings.screenHeight, settings.fullscreen)
{
	mVertexBuffers.reserve(MONSOON_MAX_ENTITIES);
}

D3D11Renderer::~D3D11Renderer() {
	
}

bool D3D11Renderer::Initialize() {
	if (!mWindow.Initialize())
		return false;

	if(!mD3d.Initialize(mWindow))
		return false;

	if (!mColorMaterial.Load(mD3d.GetDevice(), mWindow.getHandle()))
		return false;
}

void D3D11Renderer::Shutdown() {

	for (std::vector<D3D11VertexBuffer>::iterator i = mVertexBuffers.begin(); i < mVertexBuffers.end(); i++)
		i->Free();
	mVertexBuffers.empty();

	mColorMaterial.Release();
	mD3d.Shutdown();
	mWindow.Shutdown();
}

bool D3D11Renderer::Update() {
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	float fieldOfView = (float)D3DX_PI / 4.0f;
	float screenAspect = (float)mWindow.getWidth() / (float)mWindow.getHeight();

	if (defaultCamera.mode == Camera::PERSPECTIVE)
		D3DXMatrixPerspectiveFovLH(&projectionMatrix, fieldOfView, screenAspect, defaultCamera.nearClip, defaultCamera.farClip);
	else if (defaultCamera.mode == Camera::ORTHOGRAPHIC)
		D3DXMatrixOrthoLH(&projectionMatrix, defaultCamera.orthoWidth, defaultCamera.orthoHeight, defaultCamera.nearClip, defaultCamera.farClip);
	
	D3DXVECTOR3 up, position, lookAt;
	D3DXMATRIX rotationMatrix;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	position.x = defaultCamera.x;
	position.y = defaultCamera.y;
	position.z = defaultCamera.z;

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 0.0f;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, defaultCamera.yaw, defaultCamera.pitch, defaultCamera.roll);

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);

	if (!mWindow.Update())
		return false;

	mD3d.BeginScene();

	D3DXMATRIX translation, rotation, scale;

	for (int x = 0; x < mMeshComponents.Size(); x++) {
		if (auto component = mSpatialSystem->GetComponent(mMeshComponents.IndexToId(x)))
		{
			auto& spatialComponent = *component;
			D3DXMatrixIdentity(&worldMatrix);
			D3DXMatrixIdentity(&translation);
			D3DXMatrixIdentity(&rotation);
			D3DXMatrixIdentity(&scale);

			D3DXMatrixTranslation(&translation, spatialComponent.x, spatialComponent.y, spatialComponent.z);
			D3DXMatrixRotationYawPitchRoll(&rotation, spatialComponent.yaw, spatialComponent.pitch, spatialComponent.roll);
			D3DXMatrixScaling(&scale, spatialComponent.scaleX, spatialComponent.scaleY, spatialComponent.scaleZ);

			D3DXMatrixMultiply(&worldMatrix, &rotation, &scale);
			D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translation);

			mColorMaterial.Render(mD3d.GetContext(), worldMatrix, viewMatrix, projectionMatrix);
			mVertexBuffers[mMeshComponents.At(x).VertexBuffer].Render(mD3d.GetContext());
		}
	}

	mD3d.EndScene();

	return true;
}

int nextFreeId = 0;

VertexBufferHandle D3D11Renderer::CreateVertexBuffer(VertexType vertices[], int vertexCount, unsigned int indicies[], int indexCount)
{
	int vbHandle = 0;
	if (mFreeIndexList.size()) {
		vbHandle = mFreeIndexList.back();
		mFreeIndexList.pop_back();
	}
	else
	{
		vbHandle = mVertexBuffers.size();
		mVertexBuffers.push_back(D3D11VertexBuffer());
	}

	mVertexBuffers[vbHandle].Allocate(mD3d.GetDevice(), vertices, vertexCount, indicies, indexCount);
	return vbHandle;
}

void D3D11Renderer::DestroyVertexBuffer(VertexBufferHandle vbHandle)
{
	mVertexBuffers[vbHandle].Free();
	mFreeIndexList.push_back(vbHandle);
}

void D3D11Renderer::AttachMeshComponent(Monsoon::Entity entity, MeshComponent& component)
{
	mMeshComponents.Add(entity, component);
}

void D3D11Renderer::DetachMeshComponent(Monsoon::Entity entity)
{
	mMeshComponents.Remove(entity);
}

MeshComponent& D3D11Renderer::GetMeshComponent(Monsoon::Entity entity) {
	return mMeshComponents[entity];
}

VertexBufferHandle D3D11Renderer::CreatePlane(float width, float height) {
	VertexType vertices[4];

	vertices[0].SetPosition((width / 2.0f) * -1.0f, (height / 2.0f) * -1.0f, 0.0f);
	vertices[0].SetColor(1.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].SetPosition((width / 2.0f) * -1.0f, (height / 2.0f), 0.0f);
	vertices[1].SetColor(1.0f, 0.0f, 1.0f, 1.0f);

	vertices[2].SetPosition((width / 2.0f), (height / 2.0f), 0.0f);
	vertices[2].SetColor(0.0f, 1.0f, 1.0f, 1.0f);

	vertices[3].SetPosition((width / 2.0f), (height / 2.0f) * -1.0f, 0.0f);
	vertices[3].SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	unsigned int indices[6] = {
		0, 1, 2,
		0, 2, 3
	};

	return CreateVertexBuffer(vertices, 4, indices, 6);
}

VertexBufferHandle D3D11Renderer::CreateCube(float length) {
	VertexType vertices[8];

	float halfLength = length / 2.0f;

	vertices[0].SetPosition(-1.0f * halfLength, -1.0f * halfLength, -1.0f * halfLength);
	vertices[0].SetColor(1.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].SetPosition(-1.0f * halfLength, 1.0f * halfLength, -1 * halfLength);
	vertices[1].SetColor(1.0f, 0.5f, 1.0f, 1.0f);

	vertices[2].SetPosition(1.0f * halfLength, 1.0f * halfLength, -1.0f * halfLength);
	vertices[2].SetColor(1.0f, 0.5f, 0.5f, 1.0f);

	vertices[3].SetPosition(1.0f * halfLength, -1.0f * halfLength, -1.0f * halfLength);
	vertices[3].SetColor(1.0f, 0.5f, 0.0f, 1.0f);

	vertices[4].SetPosition(-1.0f * halfLength, -1.0f * halfLength, 1.0f * halfLength);
	vertices[4].SetColor(1.0f, 1.0f, 0.0f, 1.0f);

	vertices[5].SetPosition(-1.0f * halfLength, 1.0f * halfLength, 1.0f * halfLength);
	vertices[5].SetColor(1.0f, 0.5f, 1.0f, 1.0f);

	vertices[6].SetPosition(1.0f * halfLength, 1.0f * halfLength, 1.0f * halfLength);
	vertices[6].SetColor(1.0f, 0.5f, 0.5f, 1.0f);

	vertices[7].SetPosition(1.0f * halfLength, -1.0f * halfLength, 1.0f * halfLength);
	vertices[7].SetColor(1.0f, 0.5f, 0.0f, 1.0f);
	
	unsigned int indices[36] =
	{
			0, 1, 2,
			0, 2, 3,
			
			4, 6, 5,
			4, 7, 6,
			
			4, 5, 1,
			4, 1, 0,
			
			3, 2, 6,
			3, 6, 7,
			
			1, 5, 6,
			1, 6, 2,
			
			4, 0, 3,
			4, 3, 7
	};

	return CreateVertexBuffer(vertices, 8, indices, 36);
}

VertexBufferHandle D3D11Renderer::CreatePyramid(float base, float height)
{
	VertexType vertices[5];

	float halfHeight = height / 2.0f;
	float halfBase = base / 2.0f;

	vertices[0].SetPosition(-1.0f * halfBase, -1.0f * halfHeight, -1.0f * halfBase);
	vertices[0].SetColor(1.0f, 0.0f, 1.0f, 1.0f);

	vertices[1].SetPosition(-1.0f * halfBase, -1.0f * halfHeight, 1.0f * halfBase);
	vertices[1].SetColor(1.0f, 0.0f, 1.0f, 1.0f);

	vertices[2].SetPosition(1.0f * halfBase, -1.0f * halfHeight, 1.0f * halfBase);
	vertices[2].SetColor(1.0f, 0.5f, 1.0f, 1.0f);

	vertices[3].SetPosition(1.0f * halfBase, -1.0f * halfHeight, -1.0f * halfBase);
	vertices[3].SetColor(1.0f, 0.0f, 1.0f, 1.0f);

	vertices[4].SetPosition(0.0f, 1.0f * halfHeight, 0.0f);
	vertices[4].SetColor(1.0f, 0.0f, 1.0f, 1.0f);

	unsigned int indices[18] =
	{
		// Bottom
		0, 1, 2,
		0, 2, 3,

		0, 4, 1,
		1, 4, 2,
		2, 4, 3,
		3, 4, 0
	};

	return CreateVertexBuffer(vertices, 5, indices, 18);
}