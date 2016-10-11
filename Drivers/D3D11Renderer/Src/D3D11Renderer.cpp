/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#include <Windows.h>

#include "D3D11Renderer.h"
#include "D3D11VertexBuffer.h"
#include "Math/Matrix4x4.h"

#include <d3dx11tex.h>

#include <fstream>

#include "OVR.h"

OVR::Ptr<OVR::DeviceManager>	mOvrManager;
OVR::Ptr<OVR::HMDDevice>		mOvrDevice;		
OVR::HMDInfo					deviceInfo;
OVR::Util::Render::StereoConfig config;

using namespace Monsoon;
using namespace Monsoon::Renderer;

D3D11Renderer::D3D11Renderer(RendererSettings& settings, Event::EventManager* eventManager, Scene::TransformSystem* transformSystem)
	: Renderer(settings, eventManager, transformSystem),
	mTransformSystem(transformSystem),
	mWindow(settings.windowName, settings.screenWidth, settings.screenHeight, settings.fullscreen),
	mSettings(settings),
	mEventManager(eventManager),
	mSkydomeVB(-1),
	mSkydomeApexColor(0.3f, 0.8f, 1.0f),
	mSkydomeBottomColor(1.0f, 1.0f, 1.0f),
	mScreenAlignedQuadVB(-1)
{
	mVertexBuffers.reserve(MONSOON_MAX_ENTITIES);
	mTextures.reserve(1024);
}

D3D11Renderer::~D3D11Renderer() {
	
}

bool D3D11Renderer::Initialize() {

	mEventManager->Subscribe("Entity::Destroyed", [this](void* arg) {

	if (mMeshComponents.Exists((Monsoon::Entity)arg))
		this->DetachMeshComponent((Monsoon::Entity)arg);
	if (mSpriteComponents.Exists((Monsoon::Entity)arg))
		this->DetachSpriteComponent((Monsoon::Entity)arg);

		return 0;
	});

	unsigned short scrWidth = mSettings.screenWidth;
	unsigned short scrHeight = mSettings.screenHeight;

	if (mSettings.enableVR)
	{
		OVR::System::Init(OVR::Log::ConfigureDefaultLog(OVR::LogMask_All));

		mOvrManager = OVR::DeviceManager::Create();
		mOvrDevice = mOvrManager->EnumerateDevices<OVR::HMDDevice>().CreateDevice();

		mOvrDevice->GetDeviceInfo(&deviceInfo);

		config.SetHMDInfo(deviceInfo);
		config.SetStereoMode(OVR::Util::Render::Stereo_LeftRight_Multipass);
		config.SetFullViewport(OVR::Util::Render::Viewport(0, 0, scrWidth, scrHeight));
		config.SetDistortionFitPointVP(-1.0f, 1.0f);
	}

	if (!mWindow.Initialize(scrWidth, scrHeight))
		return false;

	if(!mD3d.Initialize(mWindow))
		return false;

	if (!mColorMaterial.Load(mD3d.GetDevice(), mWindow.getHandle()))
		return false;

	if (!mTextureMaterial.Load(mD3d.GetDevice(), mWindow.getHandle()))
		return false;

	if (!mSpriteMaterial.Load(mD3d.GetDevice(), mWindow.getHandle()))
		return false;

	if (!mGradientSkydomeMaterial.Load(mD3d.GetDevice(), mWindow.getHandle()))
		return false;

	if (!mBarrelDistortionPostFx.Load(mD3d.GetDevice(), mWindow.getHandle()))
		return false;

	mSpritePlane = CreatePlane(1.0f, 1.0f);
	mScreenAlignedQuadVB = CreatePlane(1.0f, 1.0f);

	return true;
}

void D3D11Renderer::Shutdown() {

	mTextureFreeList.clear();
	for (std::vector<D3D11Texture>::iterator i = mTextures.begin(); i < mTextures.end(); i++)
		if(i->Resource != nullptr)
			i->Resource->Release();
	mTextures.clear();

	mFreeIndexList.clear();
	for (std::vector<D3D11VertexBuffer>::iterator i = mVertexBuffers.begin(); i < mVertexBuffers.end(); i++)
		i->Free();
	mVertexBuffers.clear();

	mBarrelDistortionPostFx.Release();
	mGradientSkydomeMaterial.Release();
	mSpriteMaterial.Release();
	mTextureMaterial.Release();
	mColorMaterial.Release();
	mD3d.Shutdown();
	mWindow.Shutdown();

	if (mSettings.enableVR)
		OVR::System::Destroy();
}

bool D3D11Renderer::Update() {

	if (!mWindow.Update())
		return false;

	mD3d.BeginScene();

	if (mSettings.enableVR)
	{
		mD3d.SetRenderTargetAsRenderTexture();
		Render(LEFT);
		mD3d.SetRenderTargetAsBackBuffer();
		RenderPostProcessingQuad(LEFT);

		mD3d.SetRenderTargetAsRenderTexture();
		Render(RIGHT);
		mD3d.SetRenderTargetAsBackBuffer();
		RenderPostProcessingQuad(RIGHT);
	}
	else {
		if (mSettings.enablePostProcessing)
			mD3d.SetRenderTargetAsRenderTexture();
		else
			mD3d.SetRenderTargetAsBackBuffer();

		Render(BOTH);

		if (mSettings.enablePostProcessing) {
			mD3d.SetRenderTargetAsBackBuffer();
			RenderPostProcessingQuad(BOTH);
		}
	}

	mD3d.EndScene();
	mD3d.Present();

	return true;
}

float worldTrans = -20.0f;

void D3D11Renderer::Render(D3D11Renderer::EYE eye) {
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;

	D3DXVECTOR3 up, position, lookAt;
	D3DXMATRIX rotationMatrix;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	position.x = defaultCamera.x;
	position.y = defaultCamera.y;
	position.z = defaultCamera.z;

	lookAt.x = defaultCamera.lookAtX;
	lookAt.y = defaultCamera.lookAtY;
	lookAt.z = defaultCamera.lookAtZ;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, defaultCamera.yaw, defaultCamera.pitch, defaultCamera.roll);

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);

	if (mSettings.enableVR)
	{
		float aspect = (float)deviceInfo.HResolution / (2 * (float)deviceInfo.VResolution);
		float fov = 2 * atan((float)deviceInfo.VScreenSize / (2 * (float)deviceInfo.EyeToScreenDistance));

		D3DXMatrixPerspectiveFovLH(&projectionMatrix, fov, aspect, defaultCamera.nearClip, defaultCamera.farClip);
		D3DXMATRIX lensSeperationTransformation;
		D3DXMATRIX interpupillaryDistTransformation;

		float h = (4.0f * ((float)deviceInfo.HScreenSize / 4.0f) - ((float)deviceInfo.LensSeparationDistance / 2.0f)) / (float)deviceInfo.HScreenSize;

		if (GetAsyncKeyState(VK_F1))
			worldTrans += 0.01f;
		if (GetAsyncKeyState(VK_F2))
			worldTrans -= 0.01f;
		if (GetAsyncKeyState(VK_F3))
			worldTrans = worldTrans;

		if (eye == LEFT) {
			mD3d.SetViewport(0, 0, (int)ceil((mWindow.getWidth() / 2) * RENDERSCALE), (int)ceil(mWindow.getHeight() * RENDERSCALE));
			D3DXMatrixTranslation(&lensSeperationTransformation, h, 0.0f, 0.0f);
			D3DXMatrixTranslation(&interpupillaryDistTransformation, ((float)deviceInfo.InterpupillaryDistance / 2.0f) * worldTrans, 0.0f, 0.0f);
		}
		else if (eye == RIGHT) {
			mD3d.SetViewport((int)ceil((mWindow.getWidth() / 2) * RENDERSCALE), 0, (int)ceil((mWindow.getWidth() / 2) * RENDERSCALE), (int)ceil(mWindow.getHeight() * RENDERSCALE));
			D3DXMatrixTranslation(&lensSeperationTransformation, -h, 0.0f, 0.0f);
			D3DXMatrixTranslation(&interpupillaryDistTransformation, -((float)deviceInfo.InterpupillaryDistance / 2.0f) * worldTrans, 0.0f, 0.0f);

		}

		D3DXMatrixMultiply(&projectionMatrix, &projectionMatrix, &lensSeperationTransformation);
		D3DXMatrixMultiply(&projectionMatrix, &projectionMatrix, &interpupillaryDistTransformation);
	}
	else {
		float fieldOfView = (float)D3DX_PI / 4.0f;
		float screenAspect = (float)mWindow.getWidth() / (float)mWindow.getHeight();

		if (defaultCamera.mode == Camera::PERSPECTIVE)
			D3DXMatrixPerspectiveFovLH(&projectionMatrix, fieldOfView, screenAspect, defaultCamera.nearClip, defaultCamera.farClip);
		else if (defaultCamera.mode == Camera::ORTHOGRAPHIC)
			D3DXMatrixOrthoLH(&projectionMatrix, defaultCamera.orthoWidth, defaultCamera.orthoHeight, defaultCamera.nearClip, defaultCamera.farClip);
	}

	D3DXMATRIX translation, rotation, scale;

	// Render Skydome
	if (mSkydomeVB != -1)
	{
		mD3d.TurnOffCulling();
		mD3d.TurnOffZBuffer();
		Math::Vector3 position = Math::Vector3(defaultCamera.x, defaultCamera.y, defaultCamera.z);

		D3DXMatrixIdentity(&worldMatrix);

		D3DXMatrixTranslation(&worldMatrix, position.mX, position.mY, position.mZ);

		mVertexBuffers[mSkydomeVB].Render(mD3d.GetContext());
		mGradientSkydomeMaterial.Render(mD3d.GetContext(), D3DXVECTOR4(mSkydomeBottomColor.mX, mSkydomeBottomColor.mY, mSkydomeBottomColor.mZ, 0.0f), D3DXVECTOR4(mSkydomeApexColor.mX, mSkydomeApexColor.mY, mSkydomeApexColor.mZ, 0.0f), worldMatrix, viewMatrix, projectionMatrix);
		mD3d.GetContext()->DrawIndexed(mVertexBuffers[mSkydomeVB].GetIndexCount(), 0, 0);
		mD3d.TurnOnZBuffer();
		mD3d.TurnOnCulling();
	}

	// Render 3d Meshes
	for (int x = 0; x < mMeshComponents.Size(); x++) {
		Entity entity = mMeshComponents.IndexToId(x);
		if (mTransformSystem->Exists(entity))
		{
			// Gather relevent transform information.
			Math::Vector3 position = mTransformSystem->GetPosition(entity);
			float yaw = mTransformSystem->GetRotation(entity).mY;
			float pitch = mTransformSystem->GetRotation(entity).mX;
			float roll = mTransformSystem->GetRotation(entity).mZ;
			Math::Vector3 mscale = mTransformSystem->GetScale(entity);

			D3DXMatrixIdentity(&worldMatrix);
			D3DXMatrixIdentity(&translation);
			D3DXMatrixIdentity(&rotation);
			D3DXMatrixIdentity(&scale);

			D3DXMatrixTranslation(&translation, position.mX, position.mY, position.mZ);
			D3DXMatrixRotationYawPitchRoll(&rotation, yaw, pitch, roll);
			D3DXMatrixScaling(&scale, mscale.mX, mscale.mY, mscale.mZ);

			D3DXMatrixMultiply(&worldMatrix, &rotation, &scale);
			D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translation);

			mVertexBuffers[mMeshComponents.At(x).VertexBuffer].Render(mD3d.GetContext());

			// This is BAD BAD BAD, temporary code. We shouldn't be switching
			// render states for every object, but this will work until a 
			// proper material system can be put into place.
			if (mMeshComponents.At(x).TextureId != -1)
				mTextureMaterial.Render(mD3d.GetContext(), worldMatrix, viewMatrix, projectionMatrix, mTextures[mMeshComponents.At(x).TextureId].Resource);
			else
				mColorMaterial.Render(mD3d.GetContext(), worldMatrix, viewMatrix, projectionMatrix);

			mD3d.GetContext()->DrawIndexed(mVertexBuffers[mMeshComponents.At(x).VertexBuffer].GetIndexCount(), 0, 0);
		}
	}

	// Render 2d Sprites
	TextureHandle currentTexture = -1;
	Float uOffset, vOffset = 0.0f;
	Float spriteWidth, spriteHeight = 0.0f;

	mD3d.EnableAlphaBlending();
	mVertexBuffers[mSpritePlane].Render(mD3d.GetContext());
	mSpriteMaterial.Render(mD3d.GetContext());

	// Render sprites from back to front.
	for (int x = mSpriteComponents.Size() - 1; x >= 0; x--) {
		Entity entity = mSpriteComponents.IndexToId(x);
		if (mTransformSystem->Exists(entity))
		{
			// Gather relevent transform information.
			Math::Vector3 position = mTransformSystem->GetPosition(entity);
			float yaw = mTransformSystem->GetRotation(entity).mY;
			float pitch = mTransformSystem->GetRotation(entity).mX;
			float roll = mTransformSystem->GetRotation(entity).mZ;
			Math::Vector3 mscale = mTransformSystem->GetScale(entity);

			const SpriteComponent& spriteComponent = mSpriteComponents[mSpriteComponents.IndexToId(x)];
			const Texture& spriteTexture = mTextures[spriteComponent.Texture];

			if (spriteComponent.Mode == spriteComponent.ATLAS)
			{
				AtlasSprite atlasSprite = mAtlasSheets[spriteComponent.AtlasSheet].SrcRects[spriteComponent.AtlasIndex];
				spriteWidth = atlasSprite.spriteWidth;
				spriteHeight = atlasSprite.spriteHeight;
				uOffset = atlasSprite.uOffset;
				vOffset = atlasSprite.vOffset;
			}
			else
			{
				uOffset = 0.0f;
				vOffset = 0.0f;
				spriteWidth = (Float)spriteTexture.Width;
				spriteHeight = (Float)spriteTexture.Height;
			}

			D3DXMatrixIdentity(&worldMatrix);
			D3DXMatrixIdentity(&translation);
			D3DXMatrixIdentity(&rotation);
			D3DXMatrixIdentity(&scale);

			D3DXMatrixTranslation(&translation, position.mX, position.mY, (Float)spriteComponent.ZOrder);
			D3DXMatrixRotationYawPitchRoll(&rotation, yaw, pitch, roll);

			D3DXMatrixScaling(&scale, mscale.mX * spriteWidth, mscale.mY * spriteHeight, mscale.mZ);
			D3DXMatrixMultiply(&worldMatrix, &rotation, &scale);
			D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translation);

			// Check for texture change.
			if (currentTexture != spriteComponent.Texture)
			{
				currentTexture = spriteComponent.Texture;
				mSpriteMaterial.SetTexture(mD3d.GetContext(), mTextures[spriteComponent.Texture].Resource);
			}

			mSpriteMaterial.SetAtlasBuffer(mD3d.GetContext(), spriteWidth, spriteHeight, uOffset, vOffset);
			mSpriteMaterial.SetMatrixBuffer(mD3d.GetContext(), worldMatrix, viewMatrix, projectionMatrix);

			mD3d.GetContext()->DrawIndexed(12, 0, 0);
		}
	}

	mD3d.DisableAlphaBlending();
}

void D3D11Renderer::RenderPostProcessingQuad(EYE eye)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	D3DXVECTOR3 up, position, lookAt;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	position.x = 0.0f;
	position.y = 0.0f;
	position.z = -1.0f;

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 0.0f;

	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);
	D3DXMatrixOrthoLH(&projectionMatrix, 1.0f, 1.0f, 0.1f, 1000.0f);

	if (eye == LEFT) {
		mD3d.SetViewport(0, 0, (float)mWindow.getWidth() / 2, (float)mWindow.getHeight());
	}
	else if (eye == RIGHT) {
		mD3d.SetViewport((float)mWindow.getWidth() / 2, 0, (float)mWindow.getWidth() / 2, (float)mWindow.getHeight());
	}
	else {
		mD3d.SetViewport(0, 0, (float)mWindow.getWidth(), (float)mWindow.getHeight());
	}

	// Render Screen Aligned Quad
	if (mScreenAlignedQuadVB != -1)
	{
		D3DXMatrixIdentity(&worldMatrix);

		mVertexBuffers[mScreenAlignedQuadVB].Render(mD3d.GetContext());
		
		if (mSettings.enableVR) {
			if (eye == LEFT) {
				mBarrelDistortionPostFx.Render(mD3d.GetContext(), worldMatrix, viewMatrix, projectionMatrix, mD3d.GetRenderTextureShaderResource(), config, true);
			}
			else if (eye == RIGHT) {
				mBarrelDistortionPostFx.Render(mD3d.GetContext(), worldMatrix, viewMatrix, projectionMatrix, mD3d.GetRenderTextureShaderResource(), config, false);
			}
		}
		else {
			mTextureMaterial.Render(mD3d.GetContext(), worldMatrix, viewMatrix, projectionMatrix, mD3d.GetRenderTextureShaderResource());
		}
		
		mD3d.GetContext()->DrawIndexed(mVertexBuffers[mScreenAlignedQuadVB].GetIndexCount(), 0, 0);
	}
}

int nextFreeId = 0;

VertexBufferHandle D3D11Renderer::CreateVertexBuffer(VertexType vertices[], int vertexCount, unsigned int indicies[], int indexCount)
{
	U32 vbHandle = 0;
	if (mFreeIndexList.size()) {
		vbHandle = mFreeIndexList.back();
		mFreeIndexList.pop_back();
	}
	else
	{
		vbHandle = (U32)mVertexBuffers.size();
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

void D3D11Renderer::AttachSpriteComponent(Monsoon::Entity entity, SpriteComponent& component)
{
	mSpriteComponents.Add(entity, component);
	mSpriteComponents.Sort([](SpriteComponent a, SpriteComponent b) -> bool {
		if (a.ZOrder == b.ZOrder)
			return (a.Texture < b.Texture);
		else
			return (a.ZOrder < b.ZOrder);
	});
}

void D3D11Renderer::DetachSpriteComponent(Monsoon::Entity entity)
{
	mSpriteComponents.Remove(entity);
	mSpriteComponents.Sort([](SpriteComponent a, SpriteComponent b) -> bool {
		if (a.ZOrder == b.ZOrder)
			return (a.Texture < b.Texture);
		else
			return (a.ZOrder < b.ZOrder);
	});
}

SpriteComponent& D3D11Renderer::GetSpriteComponent(Monsoon::Entity entity) {
	return mSpriteComponents[entity];
}

MeshComponent& D3D11Renderer::GetMeshComponent(Monsoon::Entity entity) {
	return mMeshComponents[entity];
}

VertexBufferHandle D3D11Renderer::CreatePlane(float width, float height) {
	VertexType vertices[4];

	vertices[0].SetPosition((width / 2.0f) * -1.0f, (height / 2.0f) * -1.0f, 0.0f);
	vertices[0].SetColor(1.0f, 1.0f, 0.0f, 1.0f);
	vertices[0].SetUV(0.0f, 1.0f);

	vertices[1].SetPosition((width / 2.0f) * -1.0f, (height / 2.0f), 0.0f);
	vertices[1].SetColor(1.0f, 0.0f, 1.0f, 1.0f);
	vertices[1].SetUV(0.0f, 0.0f);

	vertices[2].SetPosition((width / 2.0f), (height / 2.0f), 0.0f);
	vertices[2].SetColor(0.0f, 1.0f, 1.0f, 1.0f);
	vertices[2].SetUV(1.0f, 0.0f);

	vertices[3].SetPosition((width / 2.0f), (height / 2.0f) * -1.0f, 0.0f);
	vertices[3].SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[3].SetUV(1.0f, 1.0f);

	unsigned int indices[12] = {
		0, 1, 2,
		0, 2, 3,
		2, 1, 0,
		3, 2, 0
	};

	return CreateVertexBuffer(vertices, 4, indices, 12);
}

VertexBufferHandle D3D11Renderer::CreateCylinder(U32 sections) {

	//
	// Generate Vertices
	// 
	VertexType* vertices = new VertexType[2 + (sections * 2)];

	vertices[0].SetPosition(0.0f, 0.5f, 0.0f);
	vertices[0].SetColor(0.0f, 0.0f, 0.0f, 1.0f);

	for (U32 x = 0; x < sections; x++)
	{
		vertices[x+1].SetPosition(0.5f * (Float)cos(x * ((2 * D3DX_PI) / sections)), 0.5f, 0.5f * (Float)sin(x * ((2 * D3DX_PI) / sections)));
		vertices[x+1].SetColor(0.2f, 0.2f, 1.0f, 1.0f);
	}
	

	vertices[sections + 1].SetPosition(0.0f, -0.5f, 0.0f);
	vertices[sections + 1].SetColor(0.0f, 0.0f, 0.0f, 1.0f);

	for (U32 x = 0; x < sections; x++)
	{
		vertices[(x + sections) + 2].SetPosition(0.5f * (Float)cos(x * ((2 * D3DX_PI) / sections)), -0.5f, 0.5f * (Float)sin(x * ((2 * D3DX_PI) / sections)));
		vertices[(x + sections) + 2].SetColor(0.20f, 0.70f, 0.20f, 1.0f);
	}

	//
	// Generate Indices
	//
	U32 size = 3 * ((sections * 2) + (sections * 2)); // 
	unsigned int* indices = new unsigned int[size];

	// Top Section
	unsigned int index = 0;
	for (U32 x = 1; x <= sections; x++)
	{
		indices[index++] = 0;
		indices[index++] = (x == sections) ? 1 : (x + 1);
		indices[index++] = x;
	}

	// Mid Section
	for (U32 x = 1; x <= sections; x++)
	{
		indices[index++] = x;
		indices[index++] = (x == sections) ? 1: (x + 1);
		indices[index++] = (x == sections) ? (2 + sections) : (x + (2 + sections));
		indices[index++] = x;
		indices[index++] = (x == sections) ? (2 + sections) : (x + (2 + sections));
		indices[index++] = (x == sections) ? (1 + (sections * 2)) : (x + (1 + sections));
	}

	// Bottom Section
	for (U32 x = (sections + 1); x <= (sections * 2); x++)
	{
		indices[index++] = sections + 1;
		indices[index++] = (x == (sections * 2)) ? (1 + sections) : (x + 1);
		indices[index++] = x;
	}

	return CreateVertexBuffer(vertices, 2 + (sections * 2), indices, size);
}

VertexBufferHandle D3D11Renderer::CreateCube(float length) {
	VertexType vertices[8];
	
	float halfLength = length / 2.0f;

	vertices[0].SetPosition(-1.0f * halfLength, -1.0f * halfLength, -1.0f * halfLength);
	vertices[0].SetColor(1.0f, 1.0f, 0.0f, 1.0f);
	vertices[0].SetUV(0.0f, 1.0f);

	vertices[1].SetPosition(-1.0f * halfLength, 1.0f * halfLength, -1 * halfLength);
	vertices[1].SetColor(0.0f, 0.5f, 1.0f, 1.0f);
	vertices[1].SetUV(0.0f, 0.0f);

	vertices[2].SetPosition(1.0f * halfLength, 1.0f * halfLength, -1.0f * halfLength);
	vertices[2].SetColor(1.0f, 1.0f, 0.5f, 1.0f);
	vertices[2].SetUV(1.0f, 0.0f);

	vertices[3].SetPosition(1.0f * halfLength, -1.0f * halfLength, -1.0f * halfLength);
	vertices[3].SetColor(1.0f, 0.5f, 0.5f, 1.0f);
	vertices[3].SetUV(1.0f, 1.0f);

	vertices[4].SetPosition(-1.0f * halfLength, -1.0f * halfLength, 1.0f * halfLength);
	vertices[4].SetColor(0.25f, 1.0f, 0.70f, 1.0f);
	vertices[4].SetUV(0.0f, 1.0f);

	vertices[5].SetPosition(-1.0f * halfLength, 1.0f * halfLength, 1.0f * halfLength);
	vertices[5].SetColor(1.0f, 0.75f, 1.0f, 1.0f);
	vertices[5].SetUV(0.0f, 0.0f);

	vertices[6].SetPosition(1.0f * halfLength, 1.0f * halfLength, 1.0f * halfLength);
	vertices[6].SetColor(1.0f, 0.5f, 0.75f, 1.0f);
	vertices[6].SetUV(1.0f, 0.0f);

	vertices[7].SetPosition(1.0f * halfLength, -1.0f * halfLength, 1.0f * halfLength);
	vertices[7].SetColor(1.0f, 0.75f, 0.30f, 1.0f);
	vertices[7].SetUV(1.0f, 1.0f);

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

U32 D3D11Renderer::LoadTexture(std::string filename)
{
	HRESULT result;
	TextureHandle index = (TextureHandle)mTextures.size();
	D3D11_TEXTURE2D_DESC textureDesc;
	ID3D11Resource* resource = nullptr;
	D3D11Texture texture;
	ID3D11Texture2D* texture2d = nullptr;

	result = D3DX11CreateShaderResourceViewFromFile(mD3d.GetDevice(), filename.c_str(), NULL, NULL, &texture.Resource, NULL);

	if (FAILED(result))
		return -1;

	texture.Resource->GetResource(&resource);
	result = resource->QueryInterface(&texture2d);

	if (FAILED(result))
		return -1;

	texture2d->GetDesc(&textureDesc);


	texture.Filename = filename;
	texture.Height = textureDesc.Height;
	texture.Width = textureDesc.Width;

	texture2d->Release();
	resource->Release();

	if (mTextureFreeList.size())
	{
		index = mTextureFreeList.back();
		mTextures[index] = texture;
		mTextureFreeList.pop_back();
	}
	else
	{
		mTextures.push_back(texture);
	}

	return index;
}

void D3D11Renderer::ReleaseTexture(U32 textureId)
{
	// Release texture resource, set the texture's
	// state to invalid, then add the resource id
	// to the freelist.
	mTextures[textureId].Resource->Release();
	mTextures[textureId].Filename = "null";
	mTextures[textureId].Width = 0;
	mTextures[textureId].Height = 0;
	mTextures[textureId].Resource = nullptr;
	mTextureFreeList.push_back(textureId);
}

AtlasSheetHandle D3D11Renderer::CreateAtlasSheet(AtlasSheet sheet)
{
	AtlasSheetHandle index = (TextureHandle)mAtlasSheets.size();
	if (mAtlasSheetFreeList.size())
	{
		index = mAtlasSheetFreeList.back();
		mAtlasSheets[index] = sheet;
		mAtlasSheetFreeList.pop_back();
	}
	else
	{
		
		mAtlasSheets.push_back(sheet);
	}
	return index;
}

void D3D11Renderer::ReleaseAtlasSheet(AtlasSheetHandle sheet)
{
	mAtlasSheets.erase(mAtlasSheets.begin() + sheet);
	mAtlasSheetFreeList.push_back(sheet);
}

VertexBufferHandle D3D11Renderer::LoadMeshTxt(std::string filename)
{
	std::ifstream file(filename.c_str(), std::ios::binary);
	U32 vertexCount = 0;
	char input = '\0';
	VertexType* vertices;
	U32* indicies;

	// Dummy Vertex Normals
	float nx, ny, nz;

	// Read up to "Vertex Count:"
	do
	{
		file.get(input);
	} while (input != ':');

	file >> vertexCount;

	// Read up to "Data: "
	do
	{
		file.get(input);
	} while (input != ':');

	file.get(input);
	file.get(input);

	vertices = new VertexType[vertexCount];
	indicies = new U32[vertexCount];
	for (U32 count = 0; count < vertexCount; count++)
	{
		file >> vertices[count].x >> vertices[count].y >> vertices[count].z;
		file >> vertices[count].u >> vertices[count].v;
		file >> nx >> ny >> nz;
		indicies[count] = count;
	}

	return CreateVertexBuffer(vertices, vertexCount, indicies, vertexCount);
}

void D3D11Renderer::CreateGradientSkydome(Math::Vector3 apexColor, Math::Vector3 bottomColor)
{
	mSkydomeVB = LoadMeshTxt("Meshes/skydome.txt");
	
}