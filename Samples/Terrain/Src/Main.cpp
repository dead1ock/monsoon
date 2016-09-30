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

#include "HgtReader.h"
#include "GCS.h"

#include <fstream>

using namespace Monsoon;
using namespace Monsoon::Math;

const U32 numChunks = 4; // The number of chunks to render as a square. (Must be greater than 2).
const int chunkXOffset = 8;
const int chunkYOffset = 12;

const float peakViewX = -1897.81921;
const float peakViewZ = -3074.84082;


const float SurfaceLengthMi = 2 * PI *  3959.0f;
const float WorldScale = SurfaceLengthMi / (360.0f);

float FirstPersonCameraSpeed = 4.0f;

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
const GcsLocation startLocation = GcsLocation(GcsCoord(39, 0, 0), GcsCoord(107, 0, 0));

class TerrainApplication : public Application
{
public:
	TerrainApplication()
		: Application((Renderer::Renderer*)(new Renderer::D3D11Renderer(Renderer::RendererSettings(), &mEventManager, &mTransformSystem))) 
		, mFileReader()
		, mFirstPersonPosition(0.0f, 0.0f, 0.0f)
		, mFirstPersonDirection(0.0f, 0.0f, 0.0f)
		, cameraTheta(0.0f)
		, cameraPhi(0.0f)
		, mFirstPersonCursorOn(false) {

		std::stringstream filename;
		filename << "I:/terrain/" << "N" << abs(startLocation.latitude.degrees) << "W" << abs(startLocation.longitude.degrees) << "-2.hgt";
		mFileReader.Load(filename.str().c_str());
		chunkSize = ceil(sqrt((mFileReader.GetResolution() * mFileReader.GetResolution()) / 256));
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
	U32 chunkSize = 0; // The number of points on a single side of a chunk. The number of total points is chunkSize ^ 2. Set dynamically after loading the HGT file.

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

		Vector2 cartesian = GcsToCartesian(startLocation.latitude, startLocation.longitude);
		mFirstPersonPosition = Vector3(cartesian.mX, 4394.12f/80.0f, cartesian.mY);

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

				textureFiles[index] << "I:/terrain/" << "N" << startLocation.latitude.degrees << "W" << startLocation.longitude.degrees << "/" << textureMap[x + chunkXOffset][y + chunkYOffset] << ".tif" << '\0';

				chunkVertexBuffers[index] = generateTerrain(x + chunkXOffset, y + chunkYOffset);
				chunkTextures[index] = mRenderer->LoadTexture(textureFiles[index].str());

				chunkMeshes[index].TextureId = chunkTextures[index];
				chunkMeshes[index].VertexBuffer = chunkVertexBuffers[index];

				chunkPositions[index].position += Vector3((((float)y/(float)chunkSize) * WorldScale * (chunkSize - 1)), 0.0f, (((float)x/(float)chunkSize) * WorldScale * (chunkSize - 1)));
				Vector2 cartesian = GcsToCartesian(startLocation.latitude, startLocation.longitude);
				chunkPositions[index].position += Vector3(cartesian.mX, 0.0f, cartesian.mY);

				mRenderer->AttachMeshComponent(index, chunkMeshes[index]);
				mTransformSystem.AttachComponent(index, chunkPositions[index]);
			}
		}
		
		cameraTheta = 0.0f;
		cameraZoom = 1.0f;

		Renderer::Camera& camera = mRenderer->GetCamera();
		camera.y = mFileReader.mMaxHeight + 3000.0f;

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
			camera.x = cos(cameraTheta) * ((float)chunkSize * (float)numChunks / 2.0f * WorldScale) * cameraZoom;
			camera.z = sin(cameraTheta) * ((float)chunkSize * (float)numChunks / 2.0f * WorldScale) * cameraZoom;
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
			camera.lookAtX = camera.x + (mFirstPersonDirection.mX);
			camera.lookAtZ = camera.z + (mFirstPersonDirection.mZ);
			camera.lookAtY = camera.y + (mFirstPersonDirection.mY);
			break;
		}
		
		camera.farClip = 1000000.0f;

		U16 leftKeyState = GetAsyncKeyState(VK_LEFT);
		U16 rightKeyState = GetAsyncKeyState(VK_RIGHT);
		U16 upKeyState = GetAsyncKeyState(VK_UP);
		U16 downKeyState = GetAsyncKeyState(VK_DOWN);
		U16 spaceKeyState = GetAsyncKeyState(VK_SPACE);
		U16 leftShiftKeyState = GetAsyncKeyState(0xA0);

		U16 rightMouseState = GetAsyncKeyState(VK_RBUTTON);
		U16 leftMouseState = GetAsyncKeyState(VK_LBUTTON);

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
			mFirstPersonPosition += mFirstPersonDirection * mGameClock.getDeltaTime() * FirstPersonCameraSpeed;
		}
		if (aKeyState)
		{
			mFirstPersonPosition += Vector3(cos((3 * PI) / 2 + cameraTheta), 0.0f, sin((3 * PI) / 2 + cameraTheta)) * -1.0f * mGameClock.getDeltaTime() * FirstPersonCameraSpeed;
		}
		if (sKeyState)
		{
			mFirstPersonPosition += mFirstPersonDirection * (-1.0f) * mGameClock.getDeltaTime() * FirstPersonCameraSpeed;
		}
		if (dKeyState)
		{
			mFirstPersonPosition += Vector3(cos((3 * PI) / 2 + cameraTheta), 0.0f, sin((3 * PI) / 2 + cameraTheta)) * mGameClock.getDeltaTime() * FirstPersonCameraSpeed;
		}
		if (spaceKeyState)
			mQuit = true;

		//
		// Debug Actions
		//
		if (upKeyState)
		{
			std::stringstream locationText;
			GcsLocation location = CartesianToGcs(mFirstPersonPosition);
			locationText << "=======================================================" << std::endl;
			locationText << "Latitude: " << location.latitude.degrees << " " << location.latitude.minutes << " " << location.latitude.seconds << std::endl;
			locationText << "Longittude: " << location.longitude.degrees << " " << location.longitude.minutes << " " << location.longitude.seconds << std::endl;
			locationText << "Direction: " << std::endl;
			locationText << "=======================================================" << std:: endl;
			mLog.Debug(Platform::VERBOSITY::ALL, locationText.str().c_str());
		}

		if (leftMouseState)
			FirstPersonCameraSpeed += mGameClock.getDeltaTime() * 100.0f;
		if (rightMouseState)
			FirstPersonCameraSpeed -= mGameClock.getDeltaTime() * 100.0f;

		if (cameraZoom <= 0.0f)
			cameraZoom = 0.001f;

		if (!mFirstPersonCursorOn)
		{
			ShowCursor(false);
			PollMousePosition();

			cameraTheta -= cursorDiffX * mGameClock.getDeltaTime() / 10.0f;
			cameraPhi -= cursorDiffY * mGameClock.getDeltaTime() / 10.0f;

			if (cameraPhi > PI / 2)
				cameraPhi = PI / 2;
			if (cameraPhi < -PI/2)
				cameraPhi = -PI / 2;

			SetCursorPos(1280.0f / 2.0f, 800.0f / 2.0f);
			cursorX = 1280.0f / 2.0f;
			cursorY = 800.0f / 2.0f;
		}
	}

	void OnShutdown() {
		for (int x = 0; x < numChunks / 2.0f; x++)
		{
			for (int y = 0; y < numChunks / 2.0f; y++)
			{
				int index = (x * chunkSize) + y;

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
		Renderer::VertexType* vertices = new Renderer::VertexType[chunkSize * chunkSize];
		unsigned int* indicies = new unsigned int[6 * ((chunkSize - 1) * (chunkSize - 1))];

		int yOffset = chunkX * (chunkSize - 1);
		int xOffset = chunkY * (chunkSize - 1);

		// Generate Vertices
		for (int x = 0; x < chunkSize; x++)
		{
			for (int y = 0; y < chunkSize; y++)
			{
				float height = mFileReader.mHeightMap[x + xOffset][y + yOffset];
				float color = (mFileReader.mHeightMap[x + xOffset][y + yOffset] - mFileReader.mMinHeight) / (mFileReader.mMaxHeight - mFileReader.mMinHeight);
				vertices[(x * chunkSize) + y].SetPosition(((float)x/(float)chunkSize) * WorldScale, mFileReader.mHeightMap[x + xOffset][y + yOffset]/80.0f, ((float)y/(float)chunkSize) * WorldScale); // This is where i'll convert to simulation world coords for scaling.
				vertices[(x * chunkSize) + y].SetColor(color, color, color, 1.0f);
				vertices[(x * chunkSize) + y].SetUV((float)(y) / (float)chunkSize, (float)(x) / (float)chunkSize);
			}
		}

		// Generate Indicies
		int i = 0;
		for (int z = 0; z < (chunkSize - 1); z++)
		{
			for (int d = 0; d < (chunkSize - 1); d++)
			{
				int index = (z * chunkSize) + d;
				indicies[i] = index + chunkSize;
				indicies[i + 1] = index;
				indicies[i + 2] = index + 1;
				indicies[i + 3] = index + chunkSize;
				indicies[i + 4] = index + 1;
				indicies[i + 5] = index + chunkSize + 1;

				i += 6;
			}
		}

		return mRenderer->CreateVertexBuffer(vertices, chunkSize * chunkSize, indicies, 6 * ((chunkSize - 1) * (chunkSize - 1)));
	}

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	TerrainApplication application;
	application.Run();
	return 0;
}

#endif