/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/


#include <Platform/Platform.h>

#ifdef MONSOON_OS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <sstream>

#include <App/Application.h>
#include <Math/Vector.h>
#include <D3D11Renderer.h>

#include <fstream>

using namespace Monsoon;
using namespace Monsoon::Math;

/*
 *
 */
const U32 size = 76;
const U32 numChunks = 4; // The number of chunks to render as a square. (Must be greater than 2).
const float chunkXScale = 65.0f;
const float chunkYScale = 65.0f;
const int chunkXOffset = 7;
const int chunkYOffset = 12;
const std::string filename = "N39W107";

const float peakViewX = -1897.81921;
const float peakViewZ = -3074.84082;

/**
 * Viewing Modes
 */
enum VIEW_MODE
{
	DEFAULT,
	PEAK,
	TRAIL,
	FIRST_PERSON
};

VIEW_MODE cameraMode = VIEW_MODE::FIRST_PERSON;

/**
 * 
 */
class HGTReader
{
public:
	HGTReader(const char* filename)
		: mFile(filename, std::ios::binary)
		, mMaxHeight(0.0f)
		, mMinHeight(50000.0f) {
		mHeightMap = new float*[1201];
		for (int i = 0; i < 1201; i++)
			mHeightMap[i] = new float[1201];

		for (int x = 0; x < 1201; x++)
		{
			for (int y = 0; y < 1201; y++)
				{
					float elevation = ((int) mFile.get() * 256) + mFile.get();
					mHeightMap[x][y] = elevation;

					if (elevation > mMaxHeight)
						mMaxHeight = elevation;
					if (elevation < mMinHeight)
						mMinHeight = elevation;
				}
		}

		int i = 0;
	}

	~HGTReader() {
		mFile.close();
	}


public:
	float** mHeightMap;
	float mMaxHeight, mMinHeight;
	std::ifstream mFile;
};

class TerrainApplication : public Application
{
public:
	TerrainApplication()
		: Application((Renderer::Renderer*)(new Renderer::D3D11Renderer(Renderer::RendererSettings(), &mEventManager, &mTransformSystem))) 
		, mFileReader((filename + ".hgt").c_str())
		, mFirstPersonPosition(0.0f, 4394.12f, 0.0f)
		, mFirstPersonDirection(0.0f, 0.0f, 0.0f)
		, cameraTheta(0.0f)
		, cameraPhi(0.0f)
		, mFirstPersonCursorOn(false) {
	}

	~TerrainApplication() {

	}

protected:
	float cameraTheta; // Yaw
	float cameraPhi; // Pitch
	float cameraZoom;
	Vector3 mFirstPersonPosition;
	Vector3 mFirstPersonDirection;
	HGTReader mFileReader;
	float cursorX;
	float cursorY;
	float cursorDiffX;
	float cursorDiffY;
	bool mFirstPersonCursorOn;

	Renderer::VertexBufferHandle* chunkVertexBuffers;
	Renderer::MeshComponent* chunkMeshes;
	Scene::TransformComponent* chunkPositions;
	Renderer::TextureHandle* chunkTextures;
	std::stringstream*	textureFiles;
	Renderer::MeshComponent skydomeMesh;

	void OnInitialize() {
		chunkVertexBuffers = new Renderer::VertexBufferHandle[numChunks * numChunks]();
		chunkMeshes = new Renderer::MeshComponent[numChunks * numChunks]();
		chunkPositions = new Scene::TransformComponent[numChunks * numChunks]();
		chunkTextures = new Renderer::TextureHandle[numChunks * numChunks]();
		textureFiles = new std::stringstream[numChunks * numChunks]();

		//
		// Generate Skydome
		//
		mRenderer->CreateGradientSkydome(Math::Vector3(0.0f, 0.0f, 0.0f), Math::Vector3(0.0f, 0.0f, 0.0f));


		//
		// Generate Texture Map
		//
		int textureMap[16][16];
		int x = 0;
		int y = 0;
		int index = 0;
		for (int i = 0; i < (256/4); i++)
		{
			textureMap[x][y] = index + 1;
			textureMap[1+x][y] = index;
			textureMap[x][1+y] = index + 3;
			textureMap[1+x][1+y] = index + 2;
			x += 2;

			if (x >= 16) {
				x = 0;
				y += 2;
			}

			index += 4;
		}

		for (int x = 0; x < numChunks; x++)
		{
			for (int y = 0; y < numChunks; y++)
			{
				int index = y + (x * numChunks);

				textureFiles[index] << "I:/terrain/" << filename << "/" << textureMap[x + chunkXOffset][y + chunkYOffset] << ".tif" << '\0';

				chunkVertexBuffers[index] = generateTerrain(x + chunkXOffset, y + chunkYOffset);
				chunkTextures[index] = mRenderer->LoadTexture(textureFiles[index].str());

				chunkMeshes[index].TextureId = chunkTextures[index];
				chunkMeshes[index].VertexBuffer = chunkVertexBuffers[index];

				chunkPositions[index].position += Math::Vector3((y * chunkXScale * (size - 1)), 0.0f, (x * chunkYScale * (size - 1)));
				chunkPositions[index].position += Math::Vector3(-(chunkXScale * (float)(size - 1) * (float)numChunks) / 2.0f, 0.0f, -(chunkYScale * (float)(size - 1) * (float)numChunks) / 2.0f);

				mRenderer->AttachMeshComponent(index, chunkMeshes[index]);
				mTransformSystem.AttachComponent(index, chunkPositions[index]);
			}
		}
		
		cameraTheta = 0.0f;
		cameraZoom = 1.0f;

		Renderer::Camera& camera = mRenderer->GetCamera();
		camera.y = mFileReader.mMaxHeight + 3000.0f;
		camera.lookAtY = mFileReader.mMinHeight + 1700.0f;

		PollMousePosition();
	}

	void PollMousePosition() {
		float x = 0;
		float y = 0;
		
		POINT cursorPosition;
		GetCursorPos(&cursorPosition);

		x = cursorPosition.x;
		y = cursorPosition.y;

		cursorDiffX = x - cursorX;
		cursorDiffY = y - cursorY;

		cursorX = x;
		cursorY = y;
	}

	void OnUpdate() {
		Renderer::Camera& camera = mRenderer->GetCamera();
		
		switch (cameraMode)
		{
		case VIEW_MODE::DEFAULT:
			camera.lookAtX = 0;
			camera.lookAtZ = 0;
			camera.x = cos(cameraTheta) * ((float)size * (float)numChunks / 2.0f * chunkXScale) * cameraZoom;
			camera.z = sin(cameraTheta) * ((float)size * (float)numChunks / 2.0f * chunkXScale) * cameraZoom;
			break;
		case VIEW_MODE::PEAK:
			camera.x = peakViewX;
			camera.z = peakViewZ;
			camera.lookAtX = camera.x + (cos(cameraTheta) * 500.0f);
			camera.lookAtZ = camera.z + (sin(cameraTheta) * 500.0f);
			camera.lookAtY = 4394.12f; //mFileReader.mHeightMap[(int)camera.x][(int)camera.z] + 5.0f;
			camera.y = 4394.12f; //mFileReader.mHeightMap[(int)camera.x][(int)camera.z] + 5.0f;
			break;
		case VIEW_MODE::FIRST_PERSON:
			mFirstPersonDirection = Vector3(cos(cameraTheta), sin(cameraPhi), sin(cameraTheta));
			camera.x = mFirstPersonPosition.mX;
			camera.z = mFirstPersonPosition.mZ;
			camera.y = mFirstPersonPosition.mY;
			camera.lookAtX = camera.x + (mFirstPersonDirection.mX * 500.0f);
			camera.lookAtZ = camera.z + (mFirstPersonDirection.mZ * 500.0f);
			camera.lookAtY = camera.y + (mFirstPersonDirection.mY * 500.0f);
			break;
		}
		
		camera.farClip = 1000000.0f;

		U16 leftKeyState = GetAsyncKeyState(VK_LEFT);
		U16 rightKeyState = GetAsyncKeyState(VK_RIGHT);
		U16 upKeyState = GetAsyncKeyState(VK_UP);
		U16 downKeyState = GetAsyncKeyState(VK_DOWN);
		U16 spaceKeyState = GetAsyncKeyState(VK_SPACE);
		U16 leftShiftKeyState = GetAsyncKeyState(0xA0);

		U16 wKeyState = GetAsyncKeyState(0x57);
		U16 aKeyState = GetAsyncKeyState(0x41);
		U16 sKeyState = GetAsyncKeyState(0x53);
		U16 dKeyState = GetAsyncKeyState(0x44);
		U16 qKeyState = GetAsyncKeyState(0x51);
		U16 eKeyState = GetAsyncKeyState(0x45);

		// 
		// Default View Controls
		// 
		if (leftKeyState)
			cameraTheta += mGameClock.getDeltaTime()/4.0f;
		if (rightKeyState)
			cameraTheta -= mGameClock.getDeltaTime() / 4.0f;
		if (upKeyState)
			cameraZoom -= mGameClock.getDeltaTime() / 4.0f;
		if (downKeyState)
			cameraZoom += mGameClock.getDeltaTime() / 4.0f;


		//
		// First Person View Controls
		//
		if (wKeyState)
		{
			mFirstPersonPosition += mFirstPersonDirection * mGameClock.getDeltaTime() * 2500.0f;
		}
		if (aKeyState)
		{
			mFirstPersonPosition += Vector3(cos((3 * PI) / 2 + cameraTheta), 0.0f, sin((3 * PI) / 2 + cameraTheta)) * -1.0f * mGameClock.getDeltaTime() * 2500.0f;
		}
		if (sKeyState)
		{
			mFirstPersonPosition += mFirstPersonDirection * (-1.0f) * mGameClock.getDeltaTime() * 2500.0f;
		}
		if (dKeyState)
		{
			mFirstPersonPosition += Vector3(cos((3 * PI) / 2 + cameraTheta), 0.0f, sin((3 * PI) / 2 + cameraTheta)) * mGameClock.getDeltaTime() * 2500.0f;
		}
		if (spaceKeyState)
			mQuit = true;

		if (cameraZoom <= 0.0f)
			cameraZoom = 0.001f;

		if (!mFirstPersonCursorOn)
		{
			PollMousePosition();

			cameraTheta -= cursorDiffX * mGameClock.getDeltaTime() / 4.0f;
			cameraPhi -= cursorDiffY * mGameClock.getDeltaTime() / 4.0f;

			SetCursorPos(2560.0f / 2.0f, 1440.0f / 2.0f);
			cursorX = 2560.0f / 2.0f;
			cursorY = 1440.0f / 2.0f;
		}
	}

	void OnShutdown() {

		for (int x = 0; x < numChunks / 2.0f; x++)
		{
			for (int y = 0; y < numChunks / 2.0f; y++)
			{
				int index = (x * size) + y;

				mTransformSystem.DetachComponent(index);
				mRenderer->DetachMeshComponent(index);
				mRenderer->ReleaseTexture(index);
			}
		}

		mRenderer->DetachMeshComponent(1000);
		mTransformSystem.DetachComponent(1000);

		delete textureFiles;
		delete chunkTextures;
		delete chunkPositions;
		delete chunkMeshes;
		delete chunkVertexBuffers;
	}

	Renderer::VertexBufferHandle generateTerrain(U32 chunkX, U32 chunkY) {
		Renderer::VertexType* vertices = new Renderer::VertexType[size * size];
		unsigned int* indicies = new unsigned int[6 * ((size-1) * (size-1))];

		int yOffset = chunkX * 75.0f;
		int xOffset = chunkY * 75.0f;

		// Generate Vertices
		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++)
			{
				float height = mFileReader.mHeightMap[x + xOffset][y + yOffset];
				float color = (mFileReader.mHeightMap[x + xOffset][y + yOffset] - mFileReader.mMinHeight) / (mFileReader.mMaxHeight - mFileReader.mMinHeight);
				vertices[(x * size) + y].SetPosition((x * chunkXScale) - ((chunkXScale * size) / 2.0f), (mFileReader.mHeightMap[x + xOffset][y + yOffset]), (y * chunkYScale) - ((chunkYScale * size) / 2.0f));
				vertices[(x * size) + y].SetColor(color, color, color, 1.0f);
				vertices[(x * size) + y].SetUV((float)(y) / (float)size, (float)(x) / (float)size );
			}
		}

		// Generate Indicies
		int i = 0;
		for (int z = 0; z < (size - 1); z++)
		{
			for (int d = 0; d < (size - 1); d++)
			{
				int index = (z * size) + d;
				indicies[i] = index + size;
				indicies[i + 1] = index;
				indicies[i + 2] = index + 1;
				indicies[i + 3] = index + size;
				indicies[i + 4] = index + 1;
				indicies[i + 5] = index + size + 1;

				i += 6;
			}
		}

		return mRenderer->CreateVertexBuffer(vertices, size * size, indicies, 6 * ((size - 1) * (size - 1)));
	}

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	TerrainApplication application;
	application.Run();
	return 0;
}

#endif