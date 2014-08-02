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
const float PLAYER_BASE_SPEED = 0.1f;
float playerSpeedMod = 1.0f;

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
		mRenderer->GetCamera().mode = Renderer::Camera::ORTHOGRAPHIC;

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
			astroidMesh.pitch = rand_FloatRange(0.0f, 2.0f * D3DX_PI);
			astroidMesh.yaw = rand_FloatRange(0.0f, 2.0f * D3DX_PI);
			astroidMesh.roll = rand_FloatRange(0.0f, 2.0f * D3DX_PI);
			mRenderer->AttachMeshComponent(mAstroids[x], astroidMesh);
		}
	}

	void OnUpdate() {
		U16 upKeyState = GetAsyncKeyState(VK_UP);
		U16 leftKeyState = GetAsyncKeyState(VK_LEFT);
		U16 rightKeyState = GetAsyncKeyState(VK_RIGHT);
		U16 downKeyState = GetAsyncKeyState(VK_DOWN);
		U16 spaceKeyState = GetAsyncKeyState(VK_SPACE);

		if (leftKeyState)
			mRenderer->GetMeshComponent(player).roll += 0.05f;
		if (rightKeyState)
			mRenderer->GetMeshComponent(player).roll -= 0.05f;
		if (upKeyState) {
			if (playerSpeedMod < 2.0f)
				// Speed up.
				playerSpeedMod += 0.1f;
		}
		else {
			if (playerSpeedMod > 1.0f)
				// Slow down.
				playerSpeedMod -= 0.05f;
			else
				// Reset speed modifier.
				playerSpeedMod = 1.0f;
		}

		// Move player in the direction it is rotated.
		mRenderer->GetMeshComponent(player).x += playerSpeedMod * PLAYER_BASE_SPEED * cos(mRenderer->GetMeshComponent(player).roll + (D3DX_PI / 2.0f));
		mRenderer->GetMeshComponent(player).y += playerSpeedMod * PLAYER_BASE_SPEED * sin(mRenderer->GetMeshComponent(player).roll + (D3DX_PI / 2.0f));

		// Wrap coordinates around when the player leaves the screen.
		if (mRenderer->GetMeshComponent(player).x > 23.0f)
			mRenderer->GetMeshComponent(player).x = -23.0f;
		else if (mRenderer->GetMeshComponent(player).x < -23.0f)
			mRenderer->GetMeshComponent(player).x = 23.0f;
		
		if (mRenderer->GetMeshComponent(player).y > 16.0f)
			mRenderer->GetMeshComponent(player).y = -16.0f;
		else if (mRenderer->GetMeshComponent(player).y < -16.0f)
			mRenderer->GetMeshComponent(player).y = 16.0f;

		// Update Asteroids
		for (int x = 0; x < 10; x++)
		{
			mRenderer->GetMeshComponent(mAstroids[x]).x += PLAYER_BASE_SPEED * cos(mRenderer->GetMeshComponent(mAstroids[x]).roll + (D3DX_PI / 2.0f));
			mRenderer->GetMeshComponent(mAstroids[x]).y += PLAYER_BASE_SPEED * sin(mRenderer->GetMeshComponent(mAstroids[x]).roll + (D3DX_PI / 2.0f));

			if (mRenderer->GetMeshComponent(mAstroids[x]).x > 23.0f)
				mRenderer->GetMeshComponent(mAstroids[x]).x = -23.0f;
			else if (mRenderer->GetMeshComponent(mAstroids[x]).x < -23.0f)
				mRenderer->GetMeshComponent(mAstroids[x]).x = 23.0f;

			if (mRenderer->GetMeshComponent(mAstroids[x]).y > 16.0f)
				mRenderer->GetMeshComponent(mAstroids[x]).y = -16.0f;
			else if (mRenderer->GetMeshComponent(mAstroids[x]).y < -16.0f)
				mRenderer->GetMeshComponent(mAstroids[x]).y = 16.0f;
		}
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