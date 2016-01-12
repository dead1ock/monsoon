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

class FractalApplication : public Application
{
public:
	FractalApplication()
		: Application((Renderer::Renderer*)(new Renderer::D3D11Renderer(Renderer::RendererSettings(), &mSpatialSystem))) {

	}

	~FractalApplication() {

	}

protected:

	void OnInitialize() {
		mRenderer->GetCamera().z = -1.25f;
		Renderer::VertexBufferHandle planeVB = mRenderer->CreatePlane(2.0f, 2.0f);

		Renderer::MeshComponent plane;
		Scene::SpatialComponent planeOnePosition;
		plane.VertexBuffer = planeVB;
		plane.TextureId = mRenderer->LoadTexture("radl_mtns_grass_short_a1.dds");
		mRenderer->AttachMeshComponent(0, plane);
		mSpatialSystem.AttachComponent(0, planeOnePosition);

	}

	void OnUpdate() {
	}

	void OnShutdown() {

		mSpatialSystem.DetachComponent(0);

		mRenderer->DetachMeshComponent(0);

		mRenderer->ReleaseTexture(0);
	}

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	FractalApplication application;
	application.Run();
	return 0;
}

#endif