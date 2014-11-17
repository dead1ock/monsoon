/**
* Copyright (c) 2014 Sonora Games
*
*/

#include <Math/AABB.h>
#include <Platform/Platform.h>

#ifdef MONSOON_OS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <App/Application.h>
#include <D3D11Renderer.h>

using namespace Monsoon;

class MeshViewer : public Application
{
public:
	MeshViewer()
		: Application((Renderer::Renderer*)(new Renderer::D3D11Renderer(Renderer::RendererSettings(), &mSpatialSystem))) {

	}

	~MeshViewer() {

	}

protected:

	void OnInitialize() {
		mMeshVertexBuffer = mRenderer->CreateCube(5.0f);
		mMeshComponent.VertexBuffer = mMeshVertexBuffer;

		mRenderer->AttachMeshComponent(0, mMeshComponent);
		mSpatialSystem.AttachComponent(0, Scene::SpatialComponent());
	}

	void OnUpdate() {
		// Get key states.
		U16 upKeyState = GetAsyncKeyState(VK_UP);
		U16 leftKeyState = GetAsyncKeyState(VK_LEFT);
		U16 rightKeyState = GetAsyncKeyState(VK_RIGHT);
		U16 downKeyState = GetAsyncKeyState(VK_DOWN);

		// Get input.
		if (upKeyState)
			cameraRadius -= 10.0f * mGameClock.getDeltaTime();
		else if (downKeyState)
			cameraRadius += 10.0f * mGameClock.getDeltaTime();

		if (leftKeyState)
			cameraTheta -= 2.0f * mGameClock.getDeltaTime();
		else if (rightKeyState)
			cameraTheta += 2.0f * mGameClock.getDeltaTime();

		// Update Camera
		Renderer::Camera& camera = mRenderer->GetCamera();
		camera.x = cos(cameraTheta) * cameraRadius;
		camera.y = 5.0f;
		camera.z = sin(cameraTheta) * cameraRadius;
	}

	void OnShutdown() {
		mSpatialSystem.DetachComponent(0);
		mRenderer->DetachMeshComponent(0);
	}

private:
	Renderer::VertexBufferHandle mMeshVertexBuffer;
	Renderer::MeshComponent mMeshComponent;

	float cameraRadius = 10.0f;
	float cameraTheta = 0.0;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	MeshViewer application;
	application.Run();
	return 0;
}

#endif