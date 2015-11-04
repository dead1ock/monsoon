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
const float PLAYER_RESPAWN_TIME = 2.5f;

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
		, currentPlayerLives(0) {
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

		currentPlayerLives = 3;
		playerLastShotTime = mGameClock.getTime();
		playerDead = false;
		level = 1;

		//
		// Create Meshes
		//
		playerVB = mRenderer->CreatePyramid(0.8f, 1.0f);
		bulletVB = mRenderer->CreatePyramid(0.25f, 0.5f);
		astroidVB = mRenderer->CreateCube(0.75f);

		SpawnPlayer();

		Renderer::MeshComponent playerLiveMesh;
		playerLiveMesh.VertexBuffer = playerVB;

		Scene::SpatialComponent playerLiveTransform;
		playerLiveTransform.position += Math::Vector3(14.0f, -19.0f, 0.0f);
		playerLiveTransform.scaleX = 0.7f;
		playerLiveTransform.scaleY = 0.7f;
		playerLiveTransform.scaleZ = 0.7f;

		playerLives[0] = mEntityManager.CreateEntity();
		mRenderer->AttachMeshComponent(playerLives[0], playerLiveMesh);
		mSpatialSystem.AttachComponent(playerLives[0], playerLiveTransform);
		playerLiveTransform.position += Math::Vector3(0.75f, 0.0f, 0.0f);

		playerLives[1] = mEntityManager.CreateEntity();
		mRenderer->AttachMeshComponent(playerLives[1], playerLiveMesh);
		mSpatialSystem.AttachComponent(playerLives[1], playerLiveTransform);
		playerLiveTransform.position += Math::Vector3(0.75f, 0.0f, 0.0f);

		playerLives[2] = mEntityManager.CreateEntity();
		mRenderer->AttachMeshComponent(playerLives[2], playerLiveMesh);
		mSpatialSystem.AttachComponent(playerLives[2], playerLiveTransform);
		playerLiveTransform.position += Math::Vector3(0.75f, 0.0f, 0.0f);

		GenerateAsteroids();

		mEventListener = mEventManager.Subscribe("Entity::Destroyed", [this](void* arg) {
			mRenderer->DetachMeshComponent((Monsoon::Entity)arg);
			return 0;
		});
	}

	int mActiveBullets = 0;

	void OnUpdate() {

		if (!playerDead)
			UpdatePlayerPosition();

		UpdateAsteroidPositions();
		UpdateBulletPositions();

		// Check for "fire"
		if (((mGameClock.getTime() - playerLastShotTime) > 0.25f) && GetAsyncKeyState(VK_SPACE) && !playerDead)
		{
			FireBullet();
		}

		CheckCollisions();


		//
		// If we've run out of asteroids, generate some more.
		//
		if (mAstroids.size() == 0)
		{
			level++;
			GenerateAsteroids();
		}

		//
		// Respawn the player if it has been over 2 seconds.
		//
		if ((mGameClock.getTime() - playerLastDeathTime) > 2.0f && currentPlayerLives > 0 && playerDead)
		{
			SpawnPlayer();
		}
	}

	void GenerateAsteroids() {
		int numAsteroids = pow(level + 1, 2);

		//
		// Generate
		//
		for (int x = 0; x < numAsteroids; x++)
		{
			mAstroids.push_back(mEntityManager.CreateEntity());

			Renderer::MeshComponent asteroidMesh;
			asteroidMesh.VertexBuffer = astroidVB;

			Scene::SpatialComponent asteroidPosition;
			asteroidPosition.position += Math::Vector3(rand_FloatRange(-10.0f, 10.0f), rand_FloatRange(-10.0f, 10.0f), 0.0f);
			asteroidPosition.pitch = rand_FloatRange(0.0f, 2.0f * D3DX_PI);
			asteroidPosition.yaw = rand_FloatRange(0.0f, 2.0f * D3DX_PI);
			asteroidPosition.roll = rand_FloatRange(0.0f, 2.0f * D3DX_PI);

			mRenderer->AttachMeshComponent(mAstroids[x], asteroidMesh);
			mSpatialSystem.AttachComponent(mAstroids[x], asteroidPosition);
			mAsteroidAABBs.push_back(Math::AABB(asteroidPosition.position.mX, asteroidPosition.position.mY, 0.75f, 0.75f));
		}

	}

	void SpawnPlayer() {
		player = mEntityManager.CreateEntity("player");
		Renderer::MeshComponent playerMesh;
		playerMesh.VertexBuffer = playerVB;
		mRenderer->AttachMeshComponent(player, playerMesh);
		mSpatialSystem.AttachComponent(player, Scene::SpatialComponent());
		playerDead = false;
	}

	void FireBullet() {
		Renderer::MeshComponent bullet;
		Scene::SpatialComponent bulletSpatialComponent = *mSpatialSystem.GetComponent(player); // Copy player position.
		bullet.VertexBuffer = bulletVB;

		bulletSpatialComponent.position += Math::Vector3(cos(bulletSpatialComponent.roll + (D3DX_PI / 2.0f)), sin(bulletSpatialComponent.roll + (D3DX_PI / 2.0f)), 0.0f);

		auto id = mEntityManager.CreateEntity();
		mBullets.push_back(id);
		mBulletAABBs.push_back(Math::AABB(bulletSpatialComponent.position.mX, bulletSpatialComponent.position.mY, 0.25f, 0.5f));
		mRenderer->AttachMeshComponent(id, bullet);
		mSpatialSystem.AttachComponent(id, bulletSpatialComponent);
		mActiveBullets++;
		playerLastShotTime = mGameClock.getTime();
	}

	void UpdatePlayerPosition() {
		U16 upKeyState = GetAsyncKeyState(VK_UP);
		U16 leftKeyState = GetAsyncKeyState(VK_LEFT);
		U16 rightKeyState = GetAsyncKeyState(VK_RIGHT);
		U16 downKeyState = GetAsyncKeyState(VK_DOWN);

		const auto& playerSpatialComponent = *mSpatialSystem.GetComponent(player);

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
			playerSpatialComponent.position.mX + (playerSpeedMod * PLAYER_BASE_SPEED * mGameClock.getDeltaTime() * cos(playerSpatialComponent.roll + (D3DX_PI / 2.0f))),
			playerSpatialComponent.position.mY + (playerSpeedMod * PLAYER_BASE_SPEED * mGameClock.getDeltaTime() * sin(playerSpatialComponent.roll + (D3DX_PI / 2.0f))),
			0.0f);

		playerAABB = Math::AABB(playerSpatialComponent.position.mX, playerSpatialComponent.position.mY, 0.8f, 1.0f);

		// Wrap coordinates around when the player leaves the screen.
		if (playerSpatialComponent.position.mX > 23.0f)
			mSpatialSystem.SetPosition(player, -23.0f, playerSpatialComponent.position.mY, 0.0f);
		else if (playerSpatialComponent.position.mX < -23.0f)
			mSpatialSystem.SetPosition(player, 23.0f, playerSpatialComponent.position.mY, 0.0f);

		if (playerSpatialComponent.position.mY > 16.0f)
			mSpatialSystem.SetPosition(player, playerSpatialComponent.position.mX, -16.0f, 0.0f);
		else if (playerSpatialComponent.position.mY < -16.0f)
			mSpatialSystem.SetPosition(player, playerSpatialComponent.position.mX, 16.0f, 0.0f);
	}

	void UpdateAsteroidPositions() {
		// Update Asteroids
		for (int x = 0; x < mAstroids.size(); x++)
		{
			const auto& asteroidSpatialComponent = *mSpatialSystem.GetComponent(mAstroids[x]);

			mSpatialSystem.SetPosition(mAstroids[x],
				asteroidSpatialComponent.position.mX + (ASTEROID_SPEED * mGameClock.getDeltaTime() * cos(asteroidSpatialComponent.roll + (D3DX_PI / 2.0f))),
				asteroidSpatialComponent.position.mY + (ASTEROID_SPEED * mGameClock.getDeltaTime() * sin(asteroidSpatialComponent.roll + (D3DX_PI / 2.0f))),
				0.0f);

			mAsteroidAABBs[x].mX = asteroidSpatialComponent.position.mX;
			mAsteroidAABBs[x].mY = asteroidSpatialComponent.position.mY;

			if (asteroidSpatialComponent.position.mX > 23.0f)
				mSpatialSystem.SetPosition(mAstroids[x], -23.0f, asteroidSpatialComponent.position.mY, 0.0f);
			else if (asteroidSpatialComponent.position.mX < -23.0f)
				mSpatialSystem.SetPosition(mAstroids[x], 23.0f, asteroidSpatialComponent.position.mY, 0.0f);

			if (asteroidSpatialComponent.position.mY > 16.0f)
				mSpatialSystem.SetPosition(mAstroids[x], asteroidSpatialComponent.position.mX, -16.0f, 0.0f);
			else if (asteroidSpatialComponent.position.mY < -16.0f)
				mSpatialSystem.SetPosition(mAstroids[x], asteroidSpatialComponent.position.mX, 16.0f, 0.0f);
		}
	}

	void UpdateBulletPositions() {
		// Update Bullets
		for (int x = (mBullets.size() - 1); x >= 0; x--)
		{
			auto& bulletSpatialComponent = *mSpatialSystem.GetComponent(mBullets[x]);
			mSpatialSystem.SetPosition(mBullets[x],
				bulletSpatialComponent.position.mX + (BULLET_SPEED * mGameClock.getDeltaTime() * cos(bulletSpatialComponent.roll + (D3DX_PI / 2.0f))),
				bulletSpatialComponent.position.mY + (BULLET_SPEED * mGameClock.getDeltaTime() * sin(bulletSpatialComponent.roll + (D3DX_PI / 2.0f))),
				0.0f);

			// Update AABB
			mBulletAABBs[x].mX = bulletSpatialComponent.position.mX;
			mBulletAABBs[x].mY = bulletSpatialComponent.position.mY;

			//
			if (bulletSpatialComponent.position.mX > 23.0f)
			{
				mEntityManager.DestroyEntity(mBullets[x]);
				mBullets.erase(mBullets.begin() + x);
				mBulletAABBs.erase(mBulletAABBs.begin() + x);
				mActiveBullets--;
			}
			else if (bulletSpatialComponent.position.mX < -23.0f)
			{
				mEntityManager.DestroyEntity(mBullets[x]);
				mBullets.erase(mBullets.begin() + x);
				mBulletAABBs.erase(mBulletAABBs.begin() + x);
				mActiveBullets--;
			}
			else if (bulletSpatialComponent.position.mY > 16.0f)
			{
				mEntityManager.DestroyEntity(mBullets[x]);
				mBullets.erase(mBullets.begin() + x);
				mBulletAABBs.erase(mBulletAABBs.begin() + x);
				mActiveBullets--;
			}
			else if (bulletSpatialComponent.position.mY < -16.0f)
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
			if (playerAABB.Intersects(mAsteroidAABBs[x]) && !playerDead && ((mGameClock.getTime() - playerLastDeathTime) > 4.0f))
			{
				mEntityManager.DestroyEntity(playerLives[currentPlayerLives-1]);
				playerLastDeathTime = mGameClock.getTime();
				currentPlayerLives--;

				mEntityManager.DestroyEntity("player");
				playerDead = true;
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
		mEventManager.Unsubscribe("Entity::Destroyed", mEventListener);

		mRenderer->DestroyVertexBuffer(astroidVB);
		mRenderer->DestroyVertexBuffer(bulletVB);
		mRenderer->DestroyVertexBuffer(playerVB);
	}

private:
	Renderer::VertexBufferHandle playerVB;
	Renderer::VertexBufferHandle bulletVB;
	Renderer::VertexBufferHandle astroidVB;

	Entity player;
	Entity playerLives[3];
	int currentPlayerLives;
	double playerLastShotTime;
	double playerLastDeathTime;
	bool playerDead;
	int level;

	Event::ListenerHandle mEventListener;

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