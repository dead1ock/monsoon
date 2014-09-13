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

const float PLAYER_BASE_SPEED = 4.0f;
float playerSpeedMod = 2.0f;
const float BULLET_SPEED = PLAYER_BASE_SPEED * 4.0f;
const float ASTEROID_SPEED = 6.0f;

float rand_FloatRange(float a, float b)
{
	return ((b - a)*((float)rand() / RAND_MAX)) + a;
}

class Asteroids : public Application
{
public:
	Asteroids()
		: Application((Renderer::Renderer*)(new Renderer::D3D11Renderer(Renderer::RendererSettings(), &mSpatialSystem)))
		, playerAABB(Math::AABB(-5.0f, 0.0f, 1.0f, 1.0f))
		, playerLives(0) {
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
		player = mEntityManager.CreateEntity("player");

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
		mSpatialSystem.AttachSpatialComponent(player, Scene::SpatialComponent());

		GenerateAsteroids();

		mEventManager.Subscribe("Entity::Destroyed", [this](void* arg) {
			mSpatialSystem.DetachSpatialComponent((Monsoon::Entity)arg);
			mRenderer->DetachMeshComponent((Monsoon::Entity)arg);
			return 0;
		});

		playerLives = 3;
		playerLastShotTime = mGameClock.getTime();
	}

	int mActiveBullets = 0;

	void OnUpdate() {

		UpdatePlayerPosition();
		UpdateAsteroidPositions();
		UpdateBulletPositions();

		// Check for "fire"
		if (((mGameClock.getTime() - playerLastShotTime) > 0.25f) && GetAsyncKeyState(VK_SPACE))
		{
			FireBullet();
		}

		CheckCollisions();


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
			mAstroids.push_back(mEntityManager.CreateEntity());

		//
		// Generate
		//
		for (int x = 0; x < mAstroids.size(); x++)
		{
			Renderer::MeshComponent asteroidMesh;
			asteroidMesh.VertexBuffer = astroidVB;

			Scene::SpatialComponent asteroidPosition;
			asteroidPosition.x = rand_FloatRange(-10.0f, 10.0f);
			asteroidPosition.y = rand_FloatRange(-10.0f, 10.0f);
			asteroidPosition.pitch = rand_FloatRange(0.0f, 2.0f * D3DX_PI);
			asteroidPosition.yaw = rand_FloatRange(0.0f, 2.0f * D3DX_PI);
			asteroidPosition.roll = rand_FloatRange(0.0f, 2.0f * D3DX_PI);

			mRenderer->AttachMeshComponent(mAstroids[x], asteroidMesh);
			mSpatialSystem.AttachSpatialComponent(mAstroids[x], asteroidPosition);
			mAsteroidAABBs.push_back(Math::AABB(asteroidPosition.x, asteroidPosition.y, 0.75f, 0.75f));
		}

	}

	void FireBullet() {
		Renderer::MeshComponent bullet;
		Scene::SpatialComponent bulletSpatialComponent = mSpatialSystem.GetSpatialComponent(player).first; // Copy player position.
		bullet.VertexBuffer = bulletVB;

		bulletSpatialComponent.x += cos(bulletSpatialComponent.roll + (D3DX_PI / 2.0f));
		bulletSpatialComponent.y += sin(bulletSpatialComponent.roll + (D3DX_PI / 2.0f));

		auto id = mEntityManager.CreateEntity();
		mBullets.push_back(id);
		mBulletAABBs.push_back(Math::AABB(bulletSpatialComponent.x, bulletSpatialComponent.y, 0.25f, 0.5f));
		mRenderer->AttachMeshComponent(id, bullet);
		mSpatialSystem.AttachSpatialComponent(id, bulletSpatialComponent);
		mActiveBullets++;
		playerLastShotTime = mGameClock.getTime();
	}

	void UpdatePlayerPosition() {
		U16 upKeyState = GetAsyncKeyState(VK_UP);
		U16 leftKeyState = GetAsyncKeyState(VK_LEFT);
		U16 rightKeyState = GetAsyncKeyState(VK_RIGHT);
		U16 downKeyState = GetAsyncKeyState(VK_DOWN);

		const auto& playerSpatialComponent = mSpatialSystem.GetSpatialComponent(player).first;

		if (leftKeyState)
			mSpatialSystem.SetOrientation(player,
			playerSpatialComponent.yaw,
			playerSpatialComponent.pitch,
			playerSpatialComponent.roll + (mGameClock.getDeltaTime() * 3.0f));
		if (rightKeyState)
			mSpatialSystem.SetOrientation(player,
			playerSpatialComponent.yaw,
			playerSpatialComponent.pitch,
			playerSpatialComponent.roll - (mGameClock.getDeltaTime() * 3.0f));
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
		mSpatialSystem.SetPosition(player,
			playerSpatialComponent.x + (playerSpeedMod * PLAYER_BASE_SPEED * mGameClock.getDeltaTime() * cos(playerSpatialComponent.roll + (D3DX_PI / 2.0f))),
			playerSpatialComponent.y + (playerSpeedMod * PLAYER_BASE_SPEED * mGameClock.getDeltaTime() * sin(playerSpatialComponent.roll + (D3DX_PI / 2.0f))),
			0.0f);

		playerAABB = Math::AABB(playerSpatialComponent.x, playerSpatialComponent.y, 0.8f, 1.0f);

		// Wrap coordinates around when the player leaves the screen.
		if (playerSpatialComponent.x > 23.0f)
			mSpatialSystem.SetPosition(player, -23.0f, playerSpatialComponent.y, 0.0f);
		else if (playerSpatialComponent.x < -23.0f)
			mSpatialSystem.SetPosition(player, 23.0f, playerSpatialComponent.y, 0.0f);

		if (playerSpatialComponent.y > 16.0f)
			mSpatialSystem.SetPosition(player, playerSpatialComponent.x, -16.0f, 0.0f);
		else if (playerSpatialComponent.y < -16.0f)
			mSpatialSystem.SetPosition(player, playerSpatialComponent.x, 16.0f, 0.0f);
	}

	void UpdateAsteroidPositions() {
		// Update Asteroids
		for (int x = 0; x < mAstroids.size(); x++)
		{
			const auto& asteroidSpatialComponent = mSpatialSystem.GetSpatialComponent(mAstroids[x]).first;

			mSpatialSystem.SetPosition(mAstroids[x],
				asteroidSpatialComponent.x + (ASTEROID_SPEED * mGameClock.getDeltaTime() * cos(asteroidSpatialComponent.roll + (D3DX_PI / 2.0f))),
				asteroidSpatialComponent.y + (ASTEROID_SPEED * mGameClock.getDeltaTime() * sin(asteroidSpatialComponent.roll + (D3DX_PI / 2.0f))),
				0.0f);

			mAsteroidAABBs[x].mX = asteroidSpatialComponent.x;
			mAsteroidAABBs[x].mY = asteroidSpatialComponent.y;

			if (asteroidSpatialComponent.x > 23.0f)
				mSpatialSystem.SetPosition(mAstroids[x], -23.0f, asteroidSpatialComponent.y, 0.0f);
			else if (asteroidSpatialComponent.x < -23.0f)
				mSpatialSystem.SetPosition(mAstroids[x], 23.0f, asteroidSpatialComponent.y, 0.0f);

			if (asteroidSpatialComponent.y > 16.0f)
				mSpatialSystem.SetPosition(mAstroids[x], asteroidSpatialComponent.x, -16.0f, 0.0f);
			else if (asteroidSpatialComponent.y < -16.0f)
				mSpatialSystem.SetPosition(mAstroids[x], asteroidSpatialComponent.x, 16.0f, 0.0f);
		}
	}

	void UpdateBulletPositions() {
		// Update Bullets
		for (int x = (mBullets.size() - 1); x >= 0; x--)
		{
			auto& bulletSpatialComponent = mSpatialSystem.GetSpatialComponent(mBullets[x]).first;
			mSpatialSystem.SetPosition(mBullets[x],
				bulletSpatialComponent.x + (BULLET_SPEED * mGameClock.getDeltaTime() * cos(bulletSpatialComponent.roll + (D3DX_PI / 2.0f))),
				bulletSpatialComponent.y + (BULLET_SPEED * mGameClock.getDeltaTime() * sin(bulletSpatialComponent.roll + (D3DX_PI / 2.0f))),
				0.0f);

			// Update AABB
			mBulletAABBs[x].mX = bulletSpatialComponent.x;
			mBulletAABBs[x].mY = bulletSpatialComponent.y;

			//
			if (bulletSpatialComponent.x > 23.0f)
			{
				mEntityManager.DestroyEntity(mBullets[x]);
				mBullets.erase(mBullets.begin() + x);
				mBulletAABBs.erase(mBulletAABBs.begin() + x);
				mActiveBullets--;
			}
			else if (bulletSpatialComponent.x < -23.0f)
			{
				mEntityManager.DestroyEntity(mBullets[x]);
				mBullets.erase(mBullets.begin() + x);
				mBulletAABBs.erase(mBulletAABBs.begin() + x);
				mActiveBullets--;
			}
			else if (bulletSpatialComponent.y > 16.0f)
			{
				mEntityManager.DestroyEntity(mBullets[x]);
				mBullets.erase(mBullets.begin() + x);
				mBulletAABBs.erase(mBulletAABBs.begin() + x);
				mActiveBullets--;
			}
			else if (bulletSpatialComponent.y < -16.0f)
			{
				mEntityManager.DestroyEntity(mBullets[x]);
				mBullets.erase(mBullets.begin() + x);
				mBulletAABBs.erase(mBulletAABBs.begin() + x);
				mActiveBullets--;
			}
		}
	}
	
	void CheckCollisions() {
		//
		// Check Collisions
		//

		// Player and Bullets -> Asteroids
		for (int x = 0; x < mAstroids.size();)
		{
			if (playerAABB.Intersects(mAsteroidAABBs[x]))
			{
				//playerLives--;
			}

			bool destroy = false;
			for (int y = 0; y < mBulletAABBs.size();)
			{
				if (mAsteroidAABBs[x].Intersects(mBulletAABBs[y]))
				{
					// Destroy asteroid.
					mEntityManager.DestroyEntity(mAstroids[x]);
					mAstroids.erase(mAstroids.begin() + x);
					mAsteroidAABBs.erase(mAsteroidAABBs.begin() + x);

					// Destroy bullet.
					mEntityManager.DestroyEntity(mBullets[y]);
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
	}

	void OnShutdown() {
		mEventManager.Unsubscribe("Entity::Destroyed", 0);

		mRenderer->DestroyVertexBuffer(astroidVB);
		mRenderer->DestroyVertexBuffer(bulletVB);
		mRenderer->DestroyVertexBuffer(playerVB);
	}

private:
	Renderer::VertexBufferHandle playerVB;
	Renderer::VertexBufferHandle bulletVB;
	Renderer::VertexBufferHandle astroidVB;

	Entity player;
	int playerLives;
	double playerLastShotTime;
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