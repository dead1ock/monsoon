/**
* Copyright (c) 2014 Sonora Games
*
*/


#include <Platform/Platform.h>

#ifdef MONSOON_OS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <App/Application.h>
#include <D3D11Renderer.h>

#include <fstream>

using namespace Monsoon;

class HGTReader
{
public:
	HGTReader(const char* filename)
		: mFile(filename, std::ios::binary) {
		mHeightMap = new float*[1201];
		for (int i = 0; i < 1201; i++)
			mHeightMap[i] = new float[1201];

		for (int x = 0; x < 1201; x++)
		{
			for (int y = 0; y < 1201; y++)
				{
					float elevation = ((int) mFile.get() * 256) + mFile.get();
					if (elevation < 0)
						int holdup = 1;
						mHeightMap[x][y] = elevation;
				}
		}

		int i = 0;
	}

	~HGTReader() {

	}


public:
	float** mHeightMap;
	std::ifstream mFile;
};

class TerrainApplication : public Application
{
public:
	TerrainApplication()
		: Application((Renderer::Renderer*)(new Renderer::D3D11Renderer(Renderer::RendererSettings(), &mSpatialSystem))) 
		, mFileReader("N45W122.hgt") {
	}

	~TerrainApplication() {

	}

protected:
	float cameraTheta;
	HGTReader mFileReader;

	void OnInitialize() {
		Renderer::VertexBufferHandle planeVB = generateTerrain();

		Renderer::MeshComponent plane;
		Scene::SpatialComponent planeOnePosition;
		plane.VertexBuffer = planeVB;
		mRenderer->AttachMeshComponent(0, plane);
		mSpatialSystem.AttachComponent(0, planeOnePosition);
		
		cameraTheta = 0.0f;
	}

	void OnUpdate() {
		Renderer::Camera& camera = mRenderer->GetCamera();
		
		camera.lookAtX = (1201.0f * 43.0f) / 2;
		camera.lookAtZ = (1201.0f * 35.0f) / 2;
		camera.x = camera.lookAtX + cos(cameraTheta) * (1201.0f * 43.0f / 1.35f);
		camera.z = camera.lookAtZ + sin(cameraTheta) * (1201.0f * 35.0f / 1.35f);
		camera.farClip = 100000.0f;

		camera.y = 5000;
		camera.lookAtY = 1000;

		cameraTheta += mGameClock.getDeltaTime()/8.0f;
	}

	void OnShutdown() {

		mSpatialSystem.DetachComponent(0);
		mSpatialSystem.DetachComponent(1);

		mRenderer->DetachMeshComponent(0);
		mRenderer->DetachMeshComponent(1);

		mRenderer->ReleaseTexture(0);
	}

	Renderer::VertexBufferHandle generateTerrain() {
		const U32 size = 1201;
		Renderer::VertexType* vertices = new Renderer::VertexType[size * size];
		unsigned int* indicies = new unsigned int[6 * ((size-1) * (size-1))];

		float maxElevation = 0;
		for (int f = 0; f < size; f++)
		{
			for (int x = 0; x < size; x++)
			{
				if (mFileReader.mHeightMap[f][x] > maxElevation);
					maxElevation = mFileReader.mHeightMap[f][x];
			}
		}

		// Generate Vertices
		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++)
			{
				vertices[(x * size) + y].SetPosition(x*43, mFileReader.mHeightMap[x][y] - maxElevation, y*35);
				vertices[(x * size) + y].SetColor(mFileReader.mHeightMap[x][y] / (maxElevation + 3000.0f), mFileReader.mHeightMap[x][y] / (maxElevation + 2500.0f), mFileReader.mHeightMap[x][y] / (maxElevation + 2500.0f), 1.0f);
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