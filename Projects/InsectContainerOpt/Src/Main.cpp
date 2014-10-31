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

class InsectContainerOpt : public Application
{
public:
	InsectContainerOpt()
		: Application((Renderer::Renderer*)(new Renderer::D3D11Renderer(Renderer::RendererSettings(), &mSpatialSystem))) {

	}

	~InsectContainerOpt() {

	}

protected:

	void OnInitialize() {

		mBoxVertexBuffer = mRenderer->CreateCylinder(32);

		mRenderer->GetCamera().lookAtX = 25.0f;
		mRenderer->GetCamera().lookAtY = 50.0f;
		mRenderer->GetCamera().lookAtZ = 15.0f;

		U32 texture = mRenderer->LoadTexture("all_metal_streaked_b.dds");

		Entity rightPlane = mEntityManager.CreateEntity();
		Renderer::MeshComponent rightPlaneMesh;
		rightPlaneMesh.TextureId = texture;
		rightPlaneMesh.VertexBuffer = mRenderer->CreatePlane(50, 100);
		
		Scene::SpatialComponent rightPlanePosition;
		rightPlanePosition.y += 50.0f;
		rightPlanePosition.x += 25.0f;
		rightPlanePosition.z -= 0.5f;
		rightPlanePosition.yaw -= (D3DX_PI);
		mSpatialSystem.AttachComponent(rightPlane, rightPlanePosition);
		mRenderer->AttachMeshComponent(rightPlane, rightPlaneMesh);

		Entity leftPlane = mEntityManager.CreateEntity();
		Renderer::MeshComponent leftPlaneMesh;
		leftPlaneMesh.TextureId = texture;
		leftPlaneMesh.VertexBuffer = mRenderer->CreatePlane(50, 100);

		Scene::SpatialComponent leftPlanePosition;
		leftPlanePosition.y += 50.0f;
		leftPlanePosition.x += 25.0f;
		leftPlanePosition.z += 30.5f;
		mSpatialSystem.AttachComponent(leftPlane, leftPlanePosition);
		mRenderer->AttachMeshComponent(leftPlane, leftPlaneMesh);


		Entity backPlane = mEntityManager.CreateEntity();
		Renderer::MeshComponent backPlaneMesh;
		backPlaneMesh.TextureId = texture;
		backPlaneMesh.VertexBuffer = mRenderer->CreatePlane(30, 100);
		mRenderer->AttachMeshComponent(backPlane, backPlaneMesh);

		Scene::SpatialComponent backPlanePosition;
		backPlanePosition.y += 50.0f;
		backPlanePosition.z += 15.0f;
		backPlanePosition.x -= 0.5f;
		backPlanePosition.yaw -= (D3DX_PI / 2.0f);
		mSpatialSystem.AttachComponent(backPlane, backPlanePosition);


		Entity frontPlane = mEntityManager.CreateEntity();
		Renderer::MeshComponent frontPlaneMesh;
		frontPlaneMesh.TextureId = texture;
		frontPlaneMesh.VertexBuffer = mRenderer->CreatePlane(30, 100);
		mRenderer->AttachMeshComponent(frontPlane, frontPlaneMesh);

		Scene::SpatialComponent frontPlanePosition;
		frontPlanePosition.y += 50.0f;
		frontPlanePosition.z += 15.0f;
		frontPlanePosition.x += 50.5f;
		frontPlanePosition.yaw += (D3DX_PI / 2.0f);
		mSpatialSystem.AttachComponent(frontPlane, frontPlanePosition);


		numContainers = 0;
		boxes = nullptr;
		containerTexture = mRenderer->LoadTexture("all_concrete_c_fallback.dds");
	}

	void OnUpdate() {

		Renderer::Camera& camera = mRenderer->GetCamera();

		camera.x = cos(cameraTheta) * 250.0f;
		camera.y = 250.0f;
		camera.z = sin(cameraTheta) * 250.0f;

		cameraTheta += mGameClock.getDeltaTime();

		// Destroy Boxes
		for (int x = 0; x < numContainers; x++)
		{
			mRenderer->DetachMeshComponent(boxes[x]);
			mEntityManager.DestroyEntity(boxes[x]);
		}

		if (boxes != nullptr)
			delete boxes;

		float L = 50, W = 30, H = 100;
		float h = 1000 / (D3DX_PI * (radius * radius));
		int stacks = floor(H / h), rows = floor(L / (2 * radius)), cols = floor(W / (2 * radius));
		numContainers = stacks * rows * cols;

		boxes = new Entity[numContainers];

		// Generate Boxes
		int cStack = 1, cRow = 1, cCol = 1;
		for (int x = 0; x < numContainers; x++)
		{
			boxes[x] = mEntityManager.CreateEntity();

			Renderer::MeshComponent boxMesh;
			boxMesh.VertexBuffer = mBoxVertexBuffer;
			//boxMesh.TextureId = containerTexture;
			mRenderer->AttachMeshComponent(boxes[x], boxMesh);

			Scene::SpatialComponent boxPosition;
			boxPosition.x += ((cRow )  * (radius * 2.0f)) - radius;
			boxPosition.y += ((cStack ) * h) - (h / 2.0f);
			boxPosition.z += ((cCol ) * (radius * 2.0f)) - radius;
			boxPosition.scaleX = radius * 2;
			boxPosition.scaleY = h;
			boxPosition.scaleZ = radius * 2;
			mSpatialSystem.AttachComponent(boxes[x], boxPosition);

			if (cCol == cols)
			{
				cCol = 1;
				if (cRow == rows)
				{
					cRow = 1;
					cStack++;

					if (cStack > stacks)
						assert(true && "This ain't suppost to happen!");
				}
				else
					cRow++;
			}
			else
				cCol++;
		}

		if (radius < 20.0f)
			radius += 1.0f * mGameClock.getDeltaTime();
		else
			radius = 1.0f;

	}

	void OnShutdown() {

	}

private:
	Renderer::VertexBufferHandle mBoxVertexBuffer;

	float radius = 1.0f;
	Entity box;
	Entity* boxes;
	int numContainers;
	float cameraTheta = 0.0;
	int containerTexture;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	InsectContainerOpt application;
	application.Run();
	return 0;
}

#endif