/**
* Copyright (c) 2014 Sonora Games
*
*/

#include <Math/AABB.h>
#include <Platform/Platform.h>

#ifdef MONSOON_OS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ctime>

#include <App/Application.h>
#include <D3D11Renderer.h>

using namespace Monsoon;

float nextId = 1;
const float PLAYER_BASE_SPEED = 0.1f;
float playerSpeedMod = 1.0f;

float rand_FloatRange(float a, float b)
{
	return ((b - a)*((float)rand() / RAND_MAX)) + a;
}

enum GameState
{

};

class Asteroids : public Application
{
public:
	Asteroids()
		: Application((Renderer::Renderer*)(new Renderer::D3D11Renderer(Renderer::RendererSettings())))
		, playerAABB(Math::AABB(-5.0f, 0.0f, 1.0f, 1.0f)) {
		mAstroids.reserve(20);
		mAsteroidAABBs.reserve(20);
		mBullets.reserve(200);
		mBulletAABBs.reserve(200);
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
		player = nextId++;

		//
		// Create Meshes
		//
		playerVB = mRenderer->CreatePyramid(0.8f, 1.0f);
		bulletVB = mRenderer->CreatePyramid(0.25f, 0.5f);
		astroidVB = mRenderer->CreateCube(0.75f);

		//
		// Attach Mesh Components
		//
		Renderer::MeshComponent playerMesh;
		playerMesh.VertexBuffer = playerVB;
		mRenderer->AttachMeshComponent(player, playerMesh);

		GenerateAsteroids();
	}

	int mActiveBullets = 0;

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

		// Check for "fire"
		if ((mActiveBullets < 200) && spaceKeyState)
		{
			Renderer::MeshComponent bullet = mRenderer->GetMeshComponent(player);
			bullet.VertexBuffer = bulletVB;
			bullet.x += cos(bullet.roll + (D3DX_PI / 2.0f));
			bullet.y += sin(bullet.roll + (D3DX_PI / 2.0f));
			mBullets.push_back(nextId++);
			mBulletAABBs.push_back(Math::AABB(bullet.x, bullet.y, 0.25f, 0.5f));
			mRenderer->AttachMeshComponent(*(mBullets.end() - 1), bullet);
			mActiveBullets++;
		}

		// Update Asteroids
		for (int x = 0; x < mAstroids.size(); x++)
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

		// Update Bullets
		int z = 0;
		for (std::vector<Entity>::iterator i = mBullets.begin(); i != mBullets.end(); )
		{
			mRenderer->GetMeshComponent(*i).x += (PLAYER_BASE_SPEED * 4.0f) * cos(mRenderer->GetMeshComponent(*i).roll + (D3DX_PI / 2.0f));
			mRenderer->GetMeshComponent(*i).y += (PLAYER_BASE_SPEED * 4.0f) * sin(mRenderer->GetMeshComponent(*i).roll + (D3DX_PI / 2.0f));

			if (mRenderer->GetMeshComponent(*i).x > 23.0f)
			{
				mRenderer->DetachMeshComponent(*i);
				i = mBullets.erase(i);
				mBulletAABBs.erase(mBulletAABBs.begin() + z);
				mActiveBullets--;
			}
			else if (mRenderer->GetMeshComponent(*i).x < -23.0f)
			{
				mRenderer->DetachMeshComponent(*i);
				i = mBullets.erase(i);
				mBulletAABBs.erase(mBulletAABBs.begin() + z);
				mActiveBullets--;
			}
			else if (mRenderer->GetMeshComponent(*i).y > 16.0f)
			{
				mRenderer->DetachMeshComponent(*i);
				i = mBullets.erase(i);
				mBulletAABBs.erase(mBulletAABBs.begin() + z);
				mActiveBullets--;
			}
			else if (mRenderer->GetMeshComponent(*i).y < -16.0f)
			{
				mRenderer->DetachMeshComponent(*i);
				i = mBullets.erase(i);
				mBulletAABBs.erase(mBulletAABBs.begin() + z);
				mActiveBullets--;
			}
			else {
				i++;
				z++;
			}
		}

		//
		// Update AABBs
		//

		// Player
		playerAABB = Math::AABB(mRenderer->GetMeshComponent(player).x, mRenderer->GetMeshComponent(player).y, 0.8f, 1.0f);

		// Asteroids
		for (int x = 0; x < mAstroids.size(); x++)
		{
			auto asteroid = mRenderer->GetMeshComponent(mAstroids[x]);
			mAsteroidAABBs[x].mX = asteroid.x;
			mAsteroidAABBs[x].mY = asteroid.y;
		}

		// Bullets
		for (int x = 0; x < mBullets.size(); x++)
		{
			auto bullet = mRenderer->GetMeshComponent(mBullets[x]);
			mBulletAABBs[x].mX = bullet.x;
			mBulletAABBs[x].mY = bullet.y;
		}

		//
		// Check Collisions
		//

		// Player and Bullets -> Asteroids
		for (int x = 0; x < mAstroids.size();)
		{
			if (playerAABB.Intersects(mAsteroidAABBs[x]))
			{
				
			}

			bool destroy = false;
			for (int y = 0; y < mBulletAABBs.size();)
			{
				if (mAsteroidAABBs[x].Intersects(mBulletAABBs[y]))
				{
					// Destroy asteroid.
					mRenderer->DetachMeshComponent(mAstroids[x]);
					mAstroids.erase(mAstroids.begin() + x);
					mAsteroidAABBs.erase(mAsteroidAABBs.begin() + x);

					// Destroy bullet.
					mRenderer->DetachMeshComponent(mBullets[y]);
					mBullets.erase(mBullets.begin() + y);
					mBulletAABBs.erase(mBulletAABBs.begin() + y);
					mActiveBullets--;
					destroy = true;
					break;
				}

				y++;
			}

			if (destroy == false)
				x++;
		}


		//
		// If we've run out of asteroids, generate some more.
		//
		if (mAstroids.size() == 0)
		{
			GenerateAsteroids();
		}
	}

	void GenerateAsteroids() {
		for (int x = 0; x < rand() % 100; x++)
			mAstroids.push_back(nextId++);

		//
		// Generate
		//
		for (int x = 0; x < mAstroids.size(); x++)
		{
			Renderer::MeshComponent astroidMesh;
			astroidMesh.VertexBuffer = astroidVB;
			astroidMesh.x = rand_FloatRange(-10.0f, 10.0f);
			astroidMesh.y = rand_FloatRange(-10.0f, 10.0f);
			astroidMesh.pitch = rand_FloatRange(0.0f, 2.0f * D3DX_PI);
			astroidMesh.yaw = rand_FloatRange(0.0f, 2.0f * D3DX_PI);
			astroidMesh.roll = rand_FloatRange(0.0f, 2.0f * D3DX_PI);
			mRenderer->AttachMeshComponent(mAstroids[x], astroidMesh);
			mAsteroidAABBs.push_back(Math::AABB(astroidMesh.x, astroidMesh.y, 0.75f, 0.75f));
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
	Math::AABB playerAABB;
	std::vector<Entity> mAstroids;
	std::vector<Math::AABB> mAsteroidAABBs;
	std::vector<Entity> mBullets;
	std::vector<Math::AABB> mBulletAABBs;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	Asteroids application;
	application.Run();
	return 0;
}

#endif