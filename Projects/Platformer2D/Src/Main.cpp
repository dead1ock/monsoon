/**
* Copyright (c) 2014-2016 Dallin Wellington
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
using namespace Monsoon::Math;


class Platformer2DApp : public Application
{
public:
	Platformer2DApp()
		: Application((Monsoon::Renderer::Renderer*)(new D3D11Renderer(RendererSettings(), &mEventManager, &mTransformSystem))) {
		mLastFrameChange = 0.0f;
		mLastJump = 0.0f;
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
		mCharacter = mEntityManager.CreateEntity();

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
		mCharacterTexture = mRenderer->LoadTexture("Character/character.dds");

		//
		// Create Atlas Sheets
		//
		AtlasSheet characterAtlasSheet;
		characterAtlasSheet.SrcRects.push_back(AtlasSprite(2.0f / 2660.0f, 2.0f / 1553.0f, 440.0f, 730.0f)); // Run (frame 1)
		characterAtlasSheet.SrcRects.push_back(AtlasSprite(444.0f / 2660.0f, 2.0f / 1553.0f, 440.0f, 716.0f)); // Run (frame 2)
		characterAtlasSheet.SrcRects.push_back(AtlasSprite(886.0f / 2660.0f, 2.0f / 1553.0f, 440.0f, 726.0f)); // Run (frame 3)
		characterAtlasSheet.SrcRects.push_back(AtlasSprite(1328.0f / 2660.0f, 2.0f / 1553.0f, 440.0f, 744.0f)); // Run (frame 4)
		characterAtlasSheet.SrcRects.push_back(AtlasSprite(1770.0f / 2660.0f, 2.0f / 1553.0f, 446.0f, 747.0f)); // Run (frame 5)
		characterAtlasSheet.SrcRects.push_back(AtlasSprite(2218.0f / 2660.0f, 2.0f / 1553.0f, 440.0f, 735.0f)); // Run (frame 6)
		characterAtlasSheet.SrcRects.push_back(AtlasSprite(510.0f / 2660.0f, 751.0f / 1553.0f, 462.0f, 684.0f)); // Idle 1
		characterAtlasSheet.SrcRects.push_back(AtlasSprite(974.0f / 2660.0f, 751.0f/ 1553.0f, 468.0f, 674.0f)); // Idle 2
		characterAtlasSheet.SrcRects.push_back(AtlasSprite(1981.0f / 2660.0f, 751.0f / 1553.0f, 451.0f, 759.0f)); // Jump Up
		characterAtlasSheet.SrcRects.push_back(AtlasSprite(1444.0f / 2660.0f, 751.0f / 1553.0f, 535.0f, 727.0f)); // Jump Down

		mCharacterAtlas = mRenderer->CreateAtlasSheet(characterAtlasSheet);


		//
		// Create Sprite Animations
		//
		mLog.Debug("Creating Animations...");
		std::vector<SpriteAnimationFrame> idleFrames;
		idleFrames.push_back(SpriteAnimationFrame(mCharacterTexture, mCharacterAtlas, 6));
		idleFrames.push_back(SpriteAnimationFrame(mCharacterTexture, mCharacterAtlas, 7));

		std::vector<SpriteAnimationFrame> runFrames;
		runFrames.push_back(SpriteAnimationFrame(mCharacterTexture, mCharacterAtlas, 0));
		runFrames.push_back(SpriteAnimationFrame(mCharacterTexture, mCharacterAtlas, 1));
		runFrames.push_back(SpriteAnimationFrame(mCharacterTexture, mCharacterAtlas, 2));
		runFrames.push_back(SpriteAnimationFrame(mCharacterTexture, mCharacterAtlas, 3));
		runFrames.push_back(SpriteAnimationFrame(mCharacterTexture, mCharacterAtlas, 4));
		runFrames.push_back(SpriteAnimationFrame(mCharacterTexture, mCharacterAtlas, 5));

		std::vector<SpriteAnimationFrame> jumpUpFrames;
		jumpUpFrames.push_back(SpriteAnimationFrame(mCharacterTexture, mCharacterAtlas, 8));

		std::vector<SpriteAnimationFrame> jumpDownFrames;
		jumpDownFrames.push_back(SpriteAnimationFrame(mCharacterTexture, mCharacterAtlas, 9));

		mIdleAnimation = mAnimationSystem.CreateAnimation2d(SpriteAnimation("characterIdle", 1.5f, idleFrames));
		mRunAnimation = mAnimationSystem.CreateAnimation2d(SpriteAnimation("characterRun", 8.0f, runFrames));
		mJumpUpAnimation = mAnimationSystem.CreateAnimation2d(SpriteAnimation("characterJumpUp", 1.0f, jumpUpFrames));
		mJumpDownAnimation = mAnimationSystem.CreateAnimation2d(SpriteAnimation("characterJumpDown", 1.0f, jumpDownFrames));


		//
		// Spawn Objects
		//
		mLog.Debug("Creating Sprites...");
		// Background
		SpriteComponent backgroundSprite;
		backgroundSprite.Texture = mBackgroundTexture;
		backgroundSprite.ZOrder = 4;
		mRenderer->AttachSpriteComponent(mBackground, backgroundSprite);
		mTransformSystem.AttachComponent(mBackground, TransformComponent());

		// Forground Tiles
		SpriteComponent tiles[3];
		tiles[0].Texture = mTileTextures[13];
		tiles[1].Texture = mTileTextures[14];
		tiles[2].Texture = mTileTextures[15];

		TransformComponent tilePositions[3];
		tilePositions[0].position.mX = -128.0f;
		tilePositions[2].position.mX = 128.0f;

		mRenderer->AttachSpriteComponent(mTiles[0], tiles[0]);
		mRenderer->AttachSpriteComponent(mTiles[1], tiles[1]);
		mRenderer->AttachSpriteComponent(mTiles[2], tiles[2]);

		mTransformSystem.AttachComponent(mTiles[0], tilePositions[0]);
		mTransformSystem.AttachComponent(mTiles[1], tilePositions[1]);
		mTransformSystem.AttachComponent(mTiles[2], tilePositions[2]);

		// Ground Tiles
		
		SpriteComponent groundTileSprites[12];
		for (int x = 0; x < 12; x++) {
			groundTileSprites[x].Texture = mTileTextures[1];
			groundTileSprites[x].ZOrder = 3;
		}

		TransformComponent groundTilePositions[12];
		for (int x = 0; x < 6; x++) {
			groundTilePositions[x].position.mX = (x * 128) - (1800.0f / 2);
			groundTilePositions[x].position.mY = -400.0f;
		}

		for (int x = 6; x < 12; x++) {
			groundTilePositions[x].position.mX = ((x + 3) * 128) - (1800.0f / 2);
			groundTilePositions[x].position.mY = -400.0f;
		}

		for (int x = 0; x < 12; x++)
			mRenderer->AttachSpriteComponent(mGroundTiles[x], groundTileSprites[x]);

		for (int x = 0; x < 12; x++)
			mTransformSystem.AttachComponent(mGroundTiles[x], groundTilePositions[x]);

		// Props
		SpriteComponent crystalSprite;
		crystalSprite.Texture = mCrystalTexture;
		crystalSprite.ZOrder = 1;

		TransformComponent crystalPosition;
		crystalPosition.position.mX = 400.0f;
		crystalPosition.position.mY = -300.0f;

		mRenderer->AttachSpriteComponent(mCrystal, crystalSprite);
		mTransformSystem.AttachComponent(mCrystal, crystalPosition);

		SpriteComponent crateSprite;
		crateSprite.Texture = mCrateTexture;
		crateSprite.ZOrder = 1;

		TransformComponent cratePosition;
		cratePosition.position.mY += 95.0f;
		cratePosition.position.mX += 100.0f;

		mRenderer->AttachSpriteComponent(mCrate, crateSprite);
		mTransformSystem.AttachComponent(mCrate, cratePosition);

		SpriteComponent stoneSprite;
		stoneSprite.Texture = mStoneTexture;
		stoneSprite.ZOrder = 1;

		TransformComponent stonePosition;
		stonePosition.position.mX = -550.0f;
		stonePosition.position.mY = -300.0f;

		mRenderer->AttachSpriteComponent(mStone, stoneSprite);
		mTransformSystem.AttachComponent(mStone, stonePosition);

		// Trees
		SpriteComponent treeComponents[2];
		treeComponents[0].Texture = mTreeTextures[0];
		treeComponents[1].Texture = mTreeTextures[1];
		treeComponents[1].ZOrder = 2;
		treeComponents[0].ZOrder = 2;

		TransformComponent treePositions[2];
		treePositions[0].position.mY = 180.0f;
		treePositions[1].position.mX = -500.0f;
		treePositions[1].position.mY = -200.0f;

		mRenderer->AttachSpriteComponent(mTrees[0], treeComponents[0]);
		mRenderer->AttachSpriteComponent(mTrees[1], treeComponents[1]);

		mTransformSystem.AttachComponent(mTrees[0], treePositions[0]);
		mTransformSystem.AttachComponent(mTrees[1], treePositions[1]);

		// Character
		SpriteComponent characterComponent;
		characterComponent.Mode = characterComponent.ATLAS;
		characterComponent.Texture = mCharacterTexture;
		characterComponent.AtlasSheet = mCharacterAtlas;
		characterComponent.AtlasIndex = 6;
		characterComponent.ZOrder = 0;

		TransformComponent characterPosition;
		characterPosition.position.mX -= 800.0f;
		characterPosition.position.mY -= 305.0f;
		characterPosition.scale = Math::Vector3(0.10f, 0.10f, 0.0f);

		mRenderer->AttachSpriteComponent(mCharacter, characterComponent);
		mTransformSystem.AttachComponent(mCharacter, characterPosition);
	
		mLog.Debug("Done.");
	}

	void OnUpdate() {
		U16 upKeyState = GetAsyncKeyState(VK_UP);
		U16 leftKeyState = GetAsyncKeyState(VK_LEFT);
		U16 rightKeyState = GetAsyncKeyState(VK_RIGHT);
		U16 downKeyState = GetAsyncKeyState(VK_DOWN);
		U16 escKeyState = GetAsyncKeyState(VK_ESCAPE);

		if (escKeyState)
			Quit();

		auto& characterSprite = mRenderer->GetSpriteComponent(mCharacter);
		
		Vector3 scale = mTransformSystem.GetScale(mCharacter);
		Vector3 position = mTransformSystem.GetPosition(mCharacter);

		if (rightKeyState) {
			if (scale.mX < 0.0f)
				mTransformSystem.Scale(mCharacter, Math::Vector3(-1.0f, 1.0f, 1.0f));

			mAnimationSystem.Play2d(mCharacter, mRunAnimation);
			mTransformSystem.Translate(mCharacter, Vector3(200.0f * mGameClock.getDeltaTime(), 0.0f, 0.0f));
		}

		if (leftKeyState) {
			if (scale.mX > 0.0f)
				mTransformSystem.Scale(mCharacter, Math::Vector3(-1.0f, 1.0f, 1.0f));

			mAnimationSystem.Play2d(mCharacter, mRunAnimation);
			mTransformSystem.Translate(mCharacter, Vector3(-200.0f * mGameClock.getDeltaTime(), 0.0f, 0.0f));
		}

		if (upKeyState)
		{
			if ((mLastJump + 0.5f) < mGameClock.getTime()) {
				mLastJump = mGameClock.getTime();
			}

			if ((mGameClock.getTime() - mLastJump) < 0.25f) {
				mAnimationSystem.Play2d(mCharacter, mJumpUpAnimation);
				mTransformSystem.Translate(mCharacter, Vector3(0.0f, 400.0f * mGameClock.getDeltaTime(), 0.0f));
			}
		}
		if (position.mY > -305.0f && (mGameClock.getTime() - mLastJump) > 0.25f) {
			mAnimationSystem.Play2d(mCharacter, mJumpDownAnimation);
			mTransformSystem.Translate(mCharacter, Vector3(0.0f, -450.0f * mGameClock.getDeltaTime(), 0.0f));
		}
		else if (position.mY < -305.0f) {
			mTransformSystem.SetPosition(mCharacter, Math::Vector3(position.mX, -305.0f, 0.0f));
		}

		if (!leftKeyState && !rightKeyState && !upKeyState && position.mY == -305.0f) {
			mAnimationSystem.Play2d(mCharacter, mIdleAnimation);
		}

		mRenderer->GetCamera().x = position.mX + 400.0f;
		mRenderer->GetCamera().lookAtX = position.mX + 400.0f;
		mTransformSystem.SetPosition(mBackground, Math::Vector3(position.mX + 400.0f, 0.0f, 0.0f));


	}

	void OnShutdown() {
		//
		// Clean Up Components
		//
		mRenderer->DetachSpriteComponent(mStone);
		mTransformSystem.DetachComponent(mStone);

		mRenderer->DetachSpriteComponent(mCrate);
		mTransformSystem.DetachComponent(mCrate);

		mRenderer->DetachSpriteComponent(mCrystal);
		mTransformSystem.DetachComponent(mCrystal);

		for (int x = 0; x < 12; x++)
			mRenderer->DetachSpriteComponent(mGroundTiles[x]);

		for (int x = 0; x < 12; x++)
			mTransformSystem.DetachComponent(mGroundTiles[x]);

		for (int x = 0; x < 2; x++)
			mTransformSystem.DetachComponent(mTrees[x]);

		for (int x = 0; x < 3; x++)
			mTransformSystem.DetachComponent(mTiles[x]);

		mTransformSystem.DetachComponent(mBackground);

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
	Entity mCharacter;

	double mLastFrameChange;
	double mLastJump;

	SpriteAnimationHandle mIdleAnimation;
	SpriteAnimationHandle mRunAnimation;
	SpriteAnimationHandle mJumpUpAnimation;
	SpriteAnimationHandle mJumpDownAnimation;

	AtlasSheetHandle mTilesetAtlas;
	AtlasSheetHandle mCharacterAtlas;

	TextureHandle mBackgroundTexture;
	TextureHandle mTileTextures[18];
	TextureHandle mTreeTextures[2];
	TextureHandle mCrystalTexture;
	TextureHandle mStoneTexture;
	TextureHandle mCrateTexture;
	TextureHandle mTilesetTexture;
	TextureHandle mCharacterTexture;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	Platformer2DApp application;
	application.Run();
	return 0;
}
