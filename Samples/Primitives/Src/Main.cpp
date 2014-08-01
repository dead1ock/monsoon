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

		int triangle = mRenderer->CreatePyramid(1.75f, 1.75f);

		int cube = mRenderer->CreateCube(1.0f);

		Renderer::MeshComponent triangle_one;
		triangle_one.VertexBuffer = triangle;
		triangle_one.x = 2.0f;
		mRenderer->AttachMeshComponent(0, triangle_one);

		Renderer::MeshComponent cube_one;
		cube_one.VertexBuffer = cube;
		cube_one.x = -2.0f;
		mRenderer->AttachMeshComponent(1, cube_one);

		Renderer::MeshComponent plane;
		plane.VertexBuffer = mRenderer->CreatePlane(10.0f, 7.0f);
		plane.pitch = 1.57f;
		plane.z = 1.0f;
		mRenderer->AttachMeshComponent(2, plane);
	}

	void OnUpdate() {

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