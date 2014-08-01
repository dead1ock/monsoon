/**
* Copyright (c) 2014 Sonora Games
*
*/


#include <Platform/Platform.h>

#ifdef MONSOON_OS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <App/Application.h>
#include <D3D11Renderer.h>

using namespace Monsoon;

float tick = 0;

class PrimitivesApplication : public Application
{
public:
	PrimitivesApplication()
		: Application((Renderer::Renderer*)(new Renderer::D3D11Renderer(Renderer::RendererSettings()))) {

	}

	~PrimitivesApplication() {

	}

protected:

	void OnInitialize() {
		Renderer::VertexBufferHandle pyramidVB = mRenderer->CreatePyramid(1.75f, 1.75f);
		Renderer::VertexBufferHandle cubeVB = mRenderer->CreateCube(1.0f);
		Renderer::VertexBufferHandle planeVB = mRenderer->CreatePlane(10.0f, 7.0f);

		Renderer::MeshComponent triangle_one;
		triangle_one.VertexBuffer = pyramidVB;
		triangle_one.x = 2.0f;
		mRenderer->AttachMeshComponent(0, triangle_one);

		Renderer::MeshComponent cube_one;
		cube_one.VertexBuffer = cubeVB;
		cube_one.x = -2.0f;
		mRenderer->AttachMeshComponent(1, cube_one);

		Renderer::MeshComponent plane;
		plane.VertexBuffer = planeVB;
		plane.pitch = 1.57f;
		plane.y = -1.0f;
		mRenderer->AttachMeshComponent(2, plane);
	}

	void OnUpdate() {
		Renderer::Camera& camera = mRenderer->GetCamera();
		Renderer::MeshComponent& pyramidMesh = mRenderer->GetMeshComponent(0);

		pyramidMesh.yaw = tick;
		pyramidMesh.y = cos(tick * 4.0f) + 1.0f;

		camera.x = cos(tick) * 8.0f;
		camera.y = 5.0f;
		camera.z = sin(tick) * 8.0f;

		tick += 0.01f;
	}

	void OnShutdown() {
		mRenderer->DetachMeshComponent(2);
		mRenderer->DetachMeshComponent(1);
		mRenderer->DetachMeshComponent(0);
	}

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	PrimitivesApplication application;
	application.Run();
	return 0;
}

#endif