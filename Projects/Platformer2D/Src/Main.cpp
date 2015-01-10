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
		mLog.Debug("Initializing Renderer...");
		mRenderer->GetCamera().z = -1.0f;
		mRenderer->GetCamera().mode = mRenderer->GetCamera().ORTHOGRAPHIC;
		mRenderer->GetCamera().orthoWidth = 1800;
		mRenderer->GetCamera().orthoHeight = 893;

		//
		// Create Scene Entities
		//
		mLog.Debug("Creating Entities...");
		mBackground = mEntityManager.CreateEntity();
		mTiles[0] = mEntityManager.CreateEntity();
		mTiles[1] = mEntityManager.CreateEntity();
		mTiles[2] = mEntityManager.CreateEntity();
		mTrees[0] = mEntityManager.CreateEntity();
		mTrees[1] = mEntityManager.CreateEntity();

		for (int x = 0; x < 12; x++)
			mGroundTiles[x] = mEntityManager.CreateEntity();

		mCrystal = mEntityManager.CreateEntity();
		mCrate = mEntityManager.CreateEntity();
		mStone = mEntityManager.CreateEntity();
		mTileset = mEntityManager.CreateEntity();

		//
		// Load Textures
		//
		mLog.Debug("Loading Textures...");
		mBackgroundTexture = mRenderer->LoadTexture("Textures/BG/BG.dds");
		mTileTextures[0] = mRenderer->LoadTexture("Textures/Tiles/1.dds");
		mTileTextures[1] = mRenderer->LoadTexture("Textures/Tiles/2.dds");
		mTileTextures[2] = mRenderer->LoadTexture("Textures/Tiles/3.dds");
		mTileTextures[3] = mRenderer->LoadTexture("Textures/Tiles/4.dds");
		mTileTextures[4] = mRenderer->LoadTexture("Textures/Tiles/5.dds");
		mTileTextures[5] = mRenderer->LoadTexture("Textures/Tiles/6.dds");
		mTileTextures[6] = mRenderer->LoadTexture("Textures/Tiles/7.dds");
		mTileTextures[7] = mRenderer->LoadTexture("Textures/Tiles/8.dds");
		mTileTextures[8] = mRenderer->LoadTexture("Textures/Tiles/9.dds");
		mTileTextures[9] = mRenderer->LoadTexture("Textures/Tiles/10.dds");
		mTileTextures[10] = mRenderer->LoadTexture("Textures/Tiles/11.dds");
		mTileTextures[11] = mRenderer->LoadTexture("Textures/Tiles/12.dds");
		mTileTextures[12] = mRenderer->LoadTexture("Textures/Tiles/13.dds");
		mTileTextures[13] = mRenderer->LoadTexture("Textures/Tiles/14.dds");
		mTileTextures[14] = mRenderer->LoadTexture("Textures/Tiles/15.dds");
		mTileTextures[15] = mRenderer->LoadTexture("Textures/Tiles/16.dds");
		mTileTextures[16] = mRenderer->LoadTexture("Textures/Tiles/17.dds");
		mTileTextures[17] = mRenderer->LoadTexture("Textures/Tiles/18.dds");
		mTreeTextures[0] = mRenderer->LoadTexture("Textures/Object/Tree_1.dds");
		mTreeTextures[1] = mRenderer->LoadTexture("Textures/Object/Tree_2.dds");
		mCrystalTexture = mRenderer->LoadTexture("Textures/Object/Crystal.dds");
		mStoneTexture = mRenderer->LoadTexture("Textures/Object/Stone.dds");
		mCrateTexture = mRenderer->LoadTexture("Textures/Object/Crate.dds");
		mTilesetTexture = mRenderer->LoadTexture("Textures/Tiles/Tileset.dds");

		//
		// Create Atlas Sheets
		//
		AtlasSheet groundTileAtlas;
		AtlasSprite groundTileAtlasSprite;
		groundTileAtlasSprite.spriteWidth = 192.0f/3;
		groundTileAtlasSprite.spriteHeight = 192.0f/3;
		groundTileAtlasSprite.uOffset = 0.0f;
		groundTileAtlasSprite.vOffset = 0.67f;
		groundTileAtlas.SrcRects.push_back(groundTileAtlasSprite);

		mTilesetAtlas = mRenderer->CreateAtlasSheet(groundTileAtlas);

		//
		// Spawn Objects
		//
		mLog.Debug("Creating Sprites...");
		// Background
		SpriteComponent backgroundSprite;
		backgroundSprite.Texture = mBackgroundTexture;
		backgroundSprite.ZOrder = 2;
		mRenderer->AttachSpriteComponent(mBackground, backgroundSprite);
		mSpatialSystem.AttachComponent(mBackground, SpatialComponent());

		// Forground Tiles
		SpriteComponent tiles[3];
		tiles[0].Texture = mTileTextures[13];
		tiles[1].Texture = mTileTextures[14];
		tiles[2].Texture = mTileTextures[15];

		SpatialComponent tilePositions[3];
		tilePositions[0].x = -128.0f;
		tilePositions[2].x = 128.0f;

		mRenderer->AttachSpriteComponent(mTiles[0], tiles[0]);
		mRenderer->AttachSpriteComponent(mTiles[1], tiles[1]);
		mRenderer->AttachSpriteComponent(mTiles[2], tiles[2]);

		mSpatialSystem.AttachComponent(mTiles[0], tilePositions[0]);
		mSpatialSystem.AttachComponent(mTiles[1], tilePositions[1]);
		mSpatialSystem.AttachComponent(mTiles[2], tilePositions[2]);

		// Ground Tiles
		//
		//
		//
		SpriteComponent tilesetSprite;
		SpatialComponent tilesetPosition;
		tilesetSprite.Mode = tilesetSprite.ATLAS;
		tilesetSprite.Texture = mTilesetTexture;
		tilesetSprite.AtlasSheet = mTilesetAtlas;
		tilesetPosition.y -= 300.0f;

		mRenderer->AttachSpriteComponent(mTileset, tilesetSprite);
		mSpatialSystem.AttachComponent(mTileset, tilesetPosition);
		//
		//
		//
		SpriteComponent groundTileSprites[12];
		for (int x = 0; x < 12; x++)
			groundTileSprites[x].Texture = mTileTextures[1];

		SpatialComponent groundTilePositions[12];
		for (int x = 0; x < 6; x++) {
			groundTilePositions[x].x = (x * 128) - (1800.0f / 2);
			groundTilePositions[x].y = -400.0f;
		}

		for (int x = 6; x < 12; x++) {
			groundTilePositions[x].x = ((x + 3) * 128) - (1800.0f / 2);
			groundTilePositions[x].y = -400.0f;
		}

		for (int x = 0; x < 12; x++)
			mRenderer->AttachSpriteComponent(mGroundTiles[x], groundTileSprites[x]);

		for (int x = 0; x < 12; x++)
			mSpatialSystem.AttachComponent(mGroundTiles[x], groundTilePositions[x]);

		// Props
		SpriteComponent crystalSprite;
		crystalSprite.Texture = mCrystalTexture;

		SpatialComponent crystalPosition;
		crystalPosition.x = 400.0f;
		crystalPosition.y = -300.0f;

		mRenderer->AttachSpriteComponent(mCrystal, crystalSprite);
		mSpatialSystem.AttachComponent(mCrystal, crystalPosition);

		SpriteComponent crateSprite;
		crateSprite.Texture = mCrateTexture;

		SpatialComponent cratePosition;
		cratePosition.y += 95.0f;
		cratePosition.x += 100.0f;

		mRenderer->AttachSpriteComponent(mCrate, crateSprite);
		mSpatialSystem.AttachComponent(mCrate, cratePosition);

		SpriteComponent stoneSprite;
		stoneSprite.Texture = mStoneTexture;

		SpatialComponent stonePosition;
		stonePosition.x = -550.0f;
		stonePosition.y = -300.0f;

		mRenderer->AttachSpriteComponent(mStone, stoneSprite);
		mSpatialSystem.AttachComponent(mStone, stonePosition);

		// Trees
		SpriteComponent treeComponents[2];
		treeComponents[0].Texture = mTreeTextures[0];
		treeComponents[1].Texture = mTreeTextures[1];
		treeComponents[1].ZOrder = 1;

		SpatialComponent treePositions[2];
		treePositions[0].y = 180.0f;
		treePositions[1].x = -500.0f;
		treePositions[1].y = -200.0f;

		mRenderer->AttachSpriteComponent(mTrees[0], treeComponents[0]);
		mRenderer->AttachSpriteComponent(mTrees[1], treeComponents[1]);

		mSpatialSystem.AttachComponent(mTrees[0], treePositions[0]);
		mSpatialSystem.AttachComponent(mTrees[1], treePositions[1]);
	
		mLog.Debug("Done.");
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
		mRenderer->DetachSpriteComponent(mStone);
		mSpatialSystem.DetachComponent(mStone);

		mRenderer->DetachSpriteComponent(mCrate);
		mSpatialSystem.DetachComponent(mCrate);

		mRenderer->DetachSpriteComponent(mCrystal);
		mSpatialSystem.DetachComponent(mCrystal);

		for (int x = 0; x < 12; x++)
			mRenderer->DetachSpriteComponent(mGroundTiles[x]);

		for (int x = 0; x < 12; x++)
			mSpatialSystem.DetachComponent(mGroundTiles[x]);

		for (int x = 0; x < 2; x++)
			mSpatialSystem.DetachComponent(mTrees[x]);

		for (int x = 0; x < 3; x++)
			mSpatialSystem.DetachComponent(mTiles[x]);

		mSpatialSystem.DetachComponent(mBackground);

		for (int x = 0; x < 2; x++)
			mRenderer->DetachSpriteComponent(mTrees[x]);

		for (int x = 0; x < 3; x++)
			mRenderer->DetachSpriteComponent(mTiles[x]);

		mRenderer->DetachSpriteComponent(mBackground);

		//
		// Clean Up Textures
		//

		//mRenderer->ReleaseTexture(mCrystalTexture);
		//mRenderer->ReleaseTexture(mCrateTexture);
		//mRenderer->ReleaseTexture(mStoneTexture);

		//for (int x = 0; x < 2; x++)
		//	mRenderer->ReleaseTextures(mTreeTextures[x]);

		//for (int x = 0; x < 18; x++)
		//	mRenderer->ReleaseTexture(mTileTextures[x]);

		//mRenderer->ReleaseTexture(mBackgroundTexture);

		//
		// Clean Up Entities
		//
		mEntityManager.DestroyEntity(mCrystal);
		mEntityManager.DestroyEntity(mCrate);
		mEntityManager.DestroyEntity(mStone);

		for (int x = 0; x < 2; x++)
			mEntityManager.DestroyEntity(mTrees[x]);

		for (int x = 0; x < 3; x++)
			mEntityManager.DestroyEntity(mTiles[x]);

		mEntityManager.DestroyEntity(mBackground);

	}

private:
	Entity mBackground;
	Entity mTiles[3];
	Entity mGroundTiles[12];
	Entity mTrees[2];
	Entity mCrate;
	Entity mCrystal;
	Entity mStone;
	Entity mTileset;

	AtlasSheetHandle mTilesetAtlas;

	TextureHandle mBackgroundTexture;
	TextureHandle mTileTextures[18];
	TextureHandle mTreeTextures[2];
	TextureHandle mCrystalTexture;
	TextureHandle mStoneTexture;
	TextureHandle mCrateTexture;
	TextureHandle mTilesetTexture;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	Platformer2DApp application;
	application.Run();
	return 0;
}
