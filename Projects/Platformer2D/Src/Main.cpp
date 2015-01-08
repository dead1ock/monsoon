/**
* Copyright (c) 2014 Sonora Games
*
*/

#include <Math/AABB.h>
#include <Platform/Platform.h>

#ifdef MONSOON_OS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#endif

#include <App/Application.h>
#include <D3D11Renderer.h>

using namespace Monsoon;
using namespace Monsoon::Renderer;
using namespace Monsoon::Scene;


class Platformer2DApp : public Application
{
public:
	Platformer2DApp()
		: Application((Monsoon::Renderer::Renderer*)(new D3D11Renderer(RendererSettings(), &mSpatialSystem))) {

	}

	~Platformer2DApp() {

	}

protected:

	void OnInitialize() {
		mRenderer->GetCamera().z = -1.0f;
		mRenderer->GetCamera().mode = mRenderer->GetCamera().ORTHOGRAPHIC;
		mRenderer->GetCamera().orthoWidth = 1800;
		mRenderer->GetCamera().orthoHeight = 893;

		//
		// Create Scene Entities
		//
		mBackground = mEntityManager.CreateEntity();

		//
		// Load Textures
		//
		mBackgroundTexture = mRenderer->LoadTexture("Textures/BG/BG.dds");

		//
		// Spawn Objects
		//

		// Background
		SpriteComponent backgroundSprite;
		backgroundSprite.Texture = mBackgroundTexture;
		mRenderer->AttachSpriteComponent(mBackground, backgroundSprite);
		mSpatialSystem.AttachComponent(mBackground, SpatialComponent());
	}

	void OnUpdate() {
		U16 upKeyState = GetAsyncKeyState(VK_UP);
		U16 leftKeyState = GetAsyncKeyState(VK_LEFT);
		U16 rightKeyState = GetAsyncKeyState(VK_RIGHT);
		U16 downKeyState = GetAsyncKeyState(VK_DOWN);

	}

	void OnShutdown() {
		//
		// Clean Up Components
		//
		mSpatialSystem.DetachComponent(mBackground);

		mRenderer->DetachSpriteComponent(mBackground);

		//
		// Clean Up Textures
		//
		mRenderer->ReleaseTexture(mBackgroundTexture);

		//
		// Clean Up Entities
		//
		mEntityManager.DestroyEntity(mBackground);
	}

private:
	Entity mBackground;
	TextureHandle mBackgroundTexture;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	Platformer2DApp application;
	application.Run();
	return 0;
}
