/**
* Copyright (c) 2014 Sonora Games
*
*/


#include <Platform/Platform.h>

#ifdef MONSOON_OS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <sstream>

#include <App/Application.h>
#include <D3D11Renderer.h>

#include <fstream>

using namespace Monsoon;

/*
 *
 */
const U32 size = 76;
const U32 numChunks = 16; // The number of chunks to render as a square. (Must be greater than 2).
const float chunkXScale = 65.0f;
const float chunkYScale = 50.0f;
const int chunkXOffset = 0;
const int chunkYOffset = 0;

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
		: Application((Renderer::Renderer*)(new Renderer::D3D11Renderer(Renderer::RendererSettings(), &mSpatialSystem))) 
		, mFileReader("N39W107.hgt") {
	}

	~TerrainApplication() {

	}

protected:
	float cameraTheta;
	HGTReader mFileReader;

	Renderer::VertexBufferHandle* chunkVertexBuffers;
	Renderer::MeshComponent* chunkMeshes;
	Scene::SpatialComponent* chunkPositions;
	Renderer::TextureHandle* chunkTextures;
	std::stringstream*	textureFiles;

	void OnInitialize() {
		chunkVertexBuffers = new Renderer::VertexBufferHandle[numChunks * numChunks]();
		chunkMeshes = new Renderer::MeshComponent[numChunks * numChunks]();
		chunkPositions = new Scene::SpatialComponent[numChunks * numChunks]();
		chunkTextures = new Renderer::TextureHandle[numChunks * numChunks]();
		textureFiles = new std::stringstream[numChunks * numChunks]();

		for (int x = 0; x < numChunks; x++)
		{
			for (int y = 0; y < numChunks; y++)
			{
				int index = y + (x * numChunks);

				textureFiles[index] << "Textures/N39W107/" << x + chunkXOffset << "_" << y + chunkYOffset << ".dds" << '\0';

				chunkVertexBuffers[index] = generateTerrain(x + chunkXOffset, y + chunkYOffset);
				chunkTextures[index] = mRenderer->LoadTexture(textureFiles[index].str());

				chunkMeshes[index].TextureId = chunkTextures[index];
				chunkMeshes[index].VertexBuffer = chunkVertexBuffers[index];

				chunkPositions[index].position += Math::Vector3((y * chunkXScale * (size - 1)), 0.0f, (x * chunkYScale * (size - 1)));
				chunkPositions[index].position += Math::Vector3(-(chunkXScale * (float)size * (float)numChunks / 2.0f), 0.0f, -(chunkYScale * (float)size * (float)numChunks / 2.0f));

				mRenderer->AttachMeshComponent(index, chunkMeshes[index]);
				mSpatialSystem.AttachComponent(index, chunkPositions[index]);
			}
		}
		
		cameraTheta = 0.0f;
	}

	void OnUpdate() {
		Renderer::Camera& camera = mRenderer->GetCamera();
		
		camera.lookAtX = 0;
		camera.lookAtZ = 0;
		camera.x = cos(cameraTheta) * ((float)size * (numChunks / 2) * chunkYScale * 1.35f);
		camera.z = sin(cameraTheta) * ((float)size * (numChunks / 2) * chunkYScale * 1.35f);
		camera.farClip = 1000000.0f;

		camera.y = mFileReader.mMaxHeight + 5000.0f;
		camera.lookAtY = mFileReader.mMinHeight;

		cameraTheta += mGameClock.getDeltaTime()/8.0f;
	}

	void OnShutdown() {

		for (int x = 0; x < numChunks / 2.0f; x++)
		{
			for (int y = 0; y < numChunks / 2.0f; y++)
			{
				int index = (x * size) + y;

				mSpatialSystem.DetachComponent(index);
				mRenderer->DetachMeshComponent(index);
				mRenderer->ReleaseTexture(index);
			}
		}

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