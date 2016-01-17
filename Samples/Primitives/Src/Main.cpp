/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/


#include <Platform/Platform.h>

#ifdef MONSOON_OS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <App/Application.h>
#include <D3D11Renderer.h>

using namespace Monsoon;

class PrimitivesApplication : public Application
{
public:
	PrimitivesApplication()
		: Application((Renderer::Renderer*)(new Renderer::D3D11Renderer(Renderer::RendererSettings(), &mEventManager, &mSpatialSystem))) {

	}

	~PrimitivesApplication() {

	}

protected:
	float cameraTheta;

	void OnInitialize() {
		Renderer::VertexBufferHandle pyramidVB = mRenderer->CreatePyramid(1.75f, 1.75f);
		Renderer::VertexBufferHandle cubeVB = mRenderer->CreateCube(1.0f);
		Renderer::VertexBufferHandle planeVB = mRenderer->CreatePlane(10.0f, 7.0f);


		Renderer::MeshComponent triangle_one;
		Scene::SpatialComponent triangleOnePosition;
		triangle_one.VertexBuffer = pyramidVB;
		triangleOnePosition.position.mX = 2.0f;
		mRenderer->AttachMeshComponent(0, triangle_one);
		mSpatialSystem.AttachComponent(0, triangleOnePosition);

		Renderer::MeshComponent cube_one;
		Scene::SpatialComponent cubeOnePosition;
		cube_one.VertexBuffer = cubeVB;
		cubeOnePosition.position.mX = -2.0f;
		mRenderer->AttachMeshComponent(1, cube_one);
		mSpatialSystem.AttachComponent(1, cubeOnePosition);

		Renderer::MeshComponent plane;
		Scene::SpatialComponent planeOnePosition;
		plane.VertexBuffer = planeVB;
		planeOnePosition.position.mY = -1.0f;
		planeOnePosition.pitch = 1.57f;
		plane.TextureId = -1;
		mRenderer->AttachMeshComponent(2, plane);
		mSpatialSystem.AttachComponent(2, planeOnePosition);

		cameraTheta = 0.0f;
	}

	void OnUpdate() {
		Renderer::Camera& camera = mRenderer->GetCamera();
		auto& pyramidTransform = *mSpatialSystem.GetComponent(0);

		mSpatialSystem.SetOrientation(0, pyramidTransform.yaw + (mGameClock.getDeltaTime() * 2.0f), pyramidTransform.pitch, pyramidTransform.roll);
		mSpatialSystem.SetPosition(0, pyramidTransform.position.mX, (cos(cameraTheta) + 1.0f), pyramidTransform.position.mZ);

		camera.x = cos(cameraTheta) * 15.0f;
		camera.y = 8.0f;
		camera.z = sin(cameraTheta) * 15.0f;

		cameraTheta += mGameClock.getDeltaTime();
	}

	void OnShutdown() {
		mSpatialSystem.DetachComponent(2);
		mRenderer->DetachMeshComponent(2);

		mSpatialSystem.DetachComponent(1);
		mRenderer->DetachMeshComponent(1);

		mSpatialSystem.DetachComponent(0);
		mRenderer->DetachMeshComponent(0);

		mRenderer->ReleaseTexture(0);
	}

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	PrimitivesApplication application;
	application.Run();
	return 0;
}

#endif