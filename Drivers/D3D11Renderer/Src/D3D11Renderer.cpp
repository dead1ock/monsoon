/**
* Copyright (c) 2014 Sonora Games
*
*/

#include <Windows.h>

#include "D3D11Renderer.h"
#include "D3D11VertexBuffer.h"

using namespace Monsoon::Renderer;

D3D11Renderer::D3D11Renderer(RendererSettings& settings)
: Renderer(settings),
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

	//
	// TEMP CAMERA CODE
	//
	const float SCREEN_DEPTH = 1000.0f;
	const float SCREEN_NEAR = 0.1f;
	float fieldOfView = (float)D3DX_PI / 4.0f;
	float screenAspect = (float)mWindow.getWidth() / (float)mWindow.getHeight();

	D3DXMatrixPerspectiveFovLH(&projectionMatrix, fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);

	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = 0;
	position.y = 0;
	position.z = -10.0;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = 0 * 0.0174532925f;
	yaw = 0 * 0.0174532925f;
	roll = 0 * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);
	//
	// TEMP CAMERA CODE
	//

	D3DXMatrixIdentity(&worldMatrix);

	if (!mWindow.Update())
		return false;

	mD3d.BeginScene();

	for (int x = 0; x < mMeshComponents.Size(); x++) {
		D3DXMatrixIdentity(&worldMatrix);
		D3DXMatrixTranslation(&worldMatrix, mMeshComponents[x].x, mMeshComponents[x].y, mMeshComponents[x].z);

		mColorMaterial.Render(mD3d.GetContext(), worldMatrix, viewMatrix, projectionMatrix);
		mVertexBuffers[mMeshComponents[x].VertexBuffer].Render(mD3d.GetContext());
	}

	mD3d.EndScene();

	return true;
}

int nextFreeId = 0;

int D3D11Renderer::CreateVertexBuffer(ColorVertex* vertices, int vertexCount, unsigned int* indicies, int indexCount)
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

void D3D11Renderer::DestroyVertexBuffer(int vbHandle)
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