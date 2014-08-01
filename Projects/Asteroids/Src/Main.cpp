/**
* Copyright (c) 2014 Sonora Games
*
*/


#include <Platform/Platform.h>

#ifdef MONSOON_OS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ctime>

#include <App/Application.h>
#include <D3D11Renderer.h>

using namespace Monsoon;

float tick = 0;

float rand_FloatRange(float a, float b)
{
	return ((b - a)*((float)rand() / RAND_MAX)) + a;
}

class Asteroids : public Application
{
public:
	Asteroids()
		: Application((Renderer::Renderer*)(new Renderer::D3D11Renderer(Renderer::RendererSettings()))) {
		mAstroids.resize(20);
		mBullets.resize(100);
	}

	~Asteroids() {

	}

protected:

	void OnInitialize() {
		mRenderer->GetCamera().z = -30.0f;

		//
		// Assign Entity Ids
		//
		player = 1;

		U32 nextId = 2;
		for (int x = 0; x < mAstroids.size(); x++)
			mAstroids[x] = nextId++;

		for (int x = 0; x < mBullets.size(); x++)
			mBullets[x] = nextId++;

		//
		// Create Meshes
		//
		playerVB = mRenderer->CreatePyramid(0.8f, 1.0f);
		bulletVB = mRenderer->CreatePyramid(0.5f, 1.0f);
		astroidVB = mRenderer->CreateCube(0.75f);

		//
		// Attach Mesh Components
		//
		Renderer::MeshComponent playerMesh;
		playerMesh.VertexBuffer = playerVB;
		mRenderer->AttachMeshComponent(player, playerMesh);

		//
		// Generate starting astroids.
		//
		for (int x = 0; x < 10; x++)
		{
			Renderer::MeshComponent astroidMesh;
			astroidMesh.VertexBuffer = astroidVB;
			astroidMesh.x = rand_FloatRange(-10.0f, 10.0f);
			astroidMesh.y = rand_FloatRange(-10.0f, 10.0f);
			mRenderer->AttachMeshComponent(mAstroids[x], astroidMesh);
		}
	}

	void OnUpdate() {
		U16 upKeyState = GetAsyncKeyState(VK_UP);
		U16 leftKeyState = GetAsyncKeyState(VK_LEFT);
		U16 rightKeyState = GetAsyncKeyState(VK_RIGHT);
		U16 downKeyState = GetAsyncKeyState(VK_DOWN);

	}

	void OnShutdown() {
		mRenderer->DestroyVertexBuffer(astroidVB);
		mRenderer->DestroyVertexBuffer(bulletVB);
		mRenderer->DestroyVertexBuffer(playerVB);
	}

private:
	Renderer::VertexBufferHandle playerVB;
	Renderer::VertexBufferHandle bulletVB;
	Renderer::VertexBufferHandle astroidVB;

	Entity player;
	std::vector<Entity> mAstroids;
	std::vector<Entity> mBullets;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	Asteroids application;
	application.Run();
	return 0;
}

#endif