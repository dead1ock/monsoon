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
		: Application((Renderer::Renderer*)(new Renderer::D3D11Renderer(Renderer::RendererSettings(), &mEventManager, &mTransformSystem))) {

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
		Scene::TransformComponent triangleOnePosition;
		triangle_one.VertexBuffer = pyramidVB;
		triangleOnePosition.position = Math::Vector3(2.0f, 0.0f, 0.0f);
		mRenderer->AttachMeshComponent(0, triangle_one);
		mTransformSystem.AttachComponent(0, triangleOnePosition);

		Renderer::MeshComponent cube_one;
		Scene::TransformComponent cubeOnePosition;
		cube_one.VertexBuffer = cubeVB;
		cubeOnePosition.position = Math::Vector3(-2.0f, 0.0f, 0.0f);
		mRenderer->AttachMeshComponent(1, cube_one);
		mTransformSystem.AttachComponent(1, cubeOnePosition);

		Renderer::MeshComponent plane;
		Scene::TransformComponent planeOnePosition;
		plane.VertexBuffer = planeVB;
		planeOnePosition.position = Math::Vector3(0.0f, -1.0f, 0.0f);
		planeOnePosition.pitch = 1.57f;
		plane.TextureId = -1;
		mRenderer->AttachMeshComponent(2, plane);
		mTransformSystem.AttachComponent(2, planeOnePosition);

		cameraTheta = 0.0f;
	}

	void OnUpdate() {
		Renderer::Camera& camera = mRenderer->GetCamera();

		mTransformSystem.Rotate(0, Math::Vector3(0.0f, mGameClock.getDeltaTime() * 2.0f, 0.0f));
		mTransformSystem.SetPosition(0, Math::Vector3(mTransformSystem.GetPosition(0).mX, (cos(cameraTheta) + 1.0f), mTransformSystem.GetPosition(0).mZ));

		camera.x = cos(cameraTheta) * 15.0f;
		camera.y = 8.0f;
		camera.z = sin(cameraTheta) * 15.0f;

		cameraTheta += mGameClock.getDeltaTime();
	}

	void OnShutdown() {
		mTransformSystem.DetachComponent(2);
		mRenderer->DetachMeshComponent(2);

		mTransformSystem.DetachComponent(1);
		mRenderer->DetachMeshComponent(1);

		mTransformSystem.DetachComponent(0);
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