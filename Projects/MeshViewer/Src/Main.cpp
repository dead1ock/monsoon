/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#include <Math/AABB.h>
#include <Platform/Platform.h>

#ifdef MONSOON_OS_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <App/Application.h>
#include <D3D11Renderer.h>

#include "iff.hpp"
#include "byte_buffer.hpp"

using namespace Monsoon;

class MeshViewer : public Application
{
public:
	MeshViewer()
		: Application((Renderer::Renderer*)(new Renderer::D3D11Renderer(Renderer::RendererSettings(), &mSpatialSystem))) {

	}

	~MeshViewer() {

	}

protected:

	void OnInitialize() {

		// Load Resources
		mMeshFile = mResourceManager.Load("a_wing_model.msh");
		mTextureFile = mRenderer->LoadTexture("a_wing.dds");

		//
		// LOAD SWG MESH
		//
		auto resource = mResourceManager.GetResourceData(mMeshFile);
		auto iffData = swgutils::tre::parse_iff(swgutils::byte_buffer((const unsigned char*)resource.data, resource.size));
		auto node = iffData->form("MESH");

		auto shaderFilename = node->children[0]->form("SPS ")->children[0]->children[1]->record("NAME");
		auto info = node->children[0]->form("SPS ")->children[0]->children[1]->children[2]->form("VTXA")->children[0]->record("INFO");
		info->data.read<uint32_t>();
		auto numVerts = info->data.read<uint32_t>();
		auto vertexData = node->children[0]->form("SPS ")->children[0]->children[1]->children[2]->form("VTXA")->children[0]->record("DATA");
		auto indicesData = node->children[0]->form("SPS ")->children[0]->children[1]->children[2]->record("INDX");
		auto numInd = indicesData->data.read<uint32_t>();

		Renderer::VertexType* vertices = new Renderer::VertexType[numVerts];
		int BVP = vertexData->data.size() / numVerts;

		for (int x = 0; x < numVerts; x++)
		{
			switch (BVP)
			{
			case 48:
				vertices[x].x = vertexData->data.read<float>();
				vertices[x].y = vertexData->data.read<float>();
				vertices[x].z = vertexData->data.read<float>();
				vertexData->data.read<float>();
				vertexData->data.read<float>();
				vertexData->data.read<float>();
				vertices[x].u = vertexData->data.read<float>();
				vertices[x].v = vertexData->data.read<float>();
				vertexData->data.read<float>();
				vertexData->data.read<float>();
				vertexData->data.read<float>();
				vertexData->data.read<float>();
				break;
			case 32:
				vertices[x].x = vertexData->data.read<float>();
				vertices[x].y = vertexData->data.read<float>();
				vertices[x].z = vertexData->data.read<float>();
				vertexData->data.read<float>();
				vertexData->data.read<float>();
				vertexData->data.read<float>();
				vertices[x].u = vertexData->data.read<float>();
				vertices[x].v = vertexData->data.read<float>();
				break;
			default:
				assert(false);
			}
		}

		int BPI = (indicesData->data.size() - 4) / numInd;
		U32* indicies = new U32[numInd];

		for (int x = 0; x < numInd; x++)
		{
			switch (BPI)
			{
			case 2:
				indicies[x] = indicesData->data.read<uint16_t>();
				break;
			case 4:
				indicies[x] = indicesData->data.read<uint32_t>();
				break;
			}
			
		}
		

		//
		// LOAD SWG MESH
		//

		mMeshVertexBuffer = mRenderer->CreateVertexBuffer(vertices, numVerts, indicies, numInd);
		mMeshComponent.VertexBuffer = mMeshVertexBuffer;
		mMeshComponent.TextureId = mTextureFile;

		mRenderer->AttachMeshComponent(0, mMeshComponent);
		mSpatialSystem.AttachComponent(0, Scene::SpatialComponent());
	}

	void OnUpdate() {
		// Get key states.
		U16 upKeyState = GetAsyncKeyState(VK_UP);
		U16 leftKeyState = GetAsyncKeyState(VK_LEFT);
		U16 rightKeyState = GetAsyncKeyState(VK_RIGHT);
		U16 downKeyState = GetAsyncKeyState(VK_DOWN);

		// Get input.
		if (upKeyState)
			cameraRadius -= 10.0f * mGameClock.getDeltaTime();
		else if (downKeyState)
			cameraRadius += 10.0f * mGameClock.getDeltaTime();

		if (leftKeyState)
			cameraTheta -= 2.0f * mGameClock.getDeltaTime();
		else if (rightKeyState)
			cameraTheta += 2.0f * mGameClock.getDeltaTime();

		// Update Camera
		Renderer::Camera& camera = mRenderer->GetCamera();
		camera.x = cos(cameraTheta) * cameraRadius;
		camera.y = 5.0f;
		camera.z = sin(cameraTheta) * cameraRadius;
	}

	void OnShutdown() {
		mSpatialSystem.DetachComponent(0);
		mRenderer->ReleaseTexture(mTextureFile);
		mRenderer->DetachMeshComponent(0);
		mResourceManager.Unload(mMeshFile);
	}

private:
	Renderer::VertexBufferHandle mMeshVertexBuffer;
	Renderer::MeshComponent mMeshComponent;
	Resource::ResourceId mMeshFile;
	U32 mTextureFile;

	float cameraRadius = 10.0f;
	float cameraTheta = 0.0;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	MeshViewer application;
	application.Run();
	return 0;
}

#endif