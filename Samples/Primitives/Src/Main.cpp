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

using namespace Monsoon;

class PrimitivesApplication : public Application
{
public:
	PrimitivesApplication()
		: Application((Renderer::Renderer*)(new Renderer::D3D11Renderer(Renderer::RendererSettings()))) {

	}

	~PrimitivesApplication() {

	}

protected:

	void OnInitialize() {

		unsigned int* indices = new unsigned int[3];
		Renderer::ColorVertex* vertices = new Renderer::ColorVertex[3];

		vertices[0].SetPosition(-1.0f, -1.0f, 0.0f);
		vertices[0].SetColor(1.0f, 0.0f, 1.0f, 1.0f);

		vertices[1].SetPosition(0.0f, 1.0f, 0.0f);
		vertices[1].SetColor(1.0f, 0.0f, 1.0f, 1.0f);

		vertices[2].SetPosition(1.0f, -1.0f, 0.0f);
		vertices[2].SetColor(1.0f, 0.0f, 1.0f, 1.0f);

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;

		int triangle = mRenderer->CreateVertexBuffer(vertices, 3, indices, 3);

		delete indices;
		delete vertices;

		unsigned int* cube_indices = new unsigned int[36]
		{
				// front face
				0, 1, 2,
				0, 2, 3,
				// back face
				4, 6, 5,
				4, 7, 6,
				// left face
				4, 5, 1,
				4, 1, 0,
				// right face
				3, 2, 6,
				3, 6, 7,
				// top face
				1, 5, 6,
				1, 6, 2,
				// bottom face
				4, 0, 3,
				4, 3, 7
		};

		vertices = new Renderer::ColorVertex[8];

		vertices[0].SetPosition(-1.0f, -1.0f, -1.0f);
		vertices[0].SetColor(1.0f, 1.0f, 0.0f, 1.0f);

		vertices[1].SetPosition(-1, 1, -1);
		vertices[1].SetColor(1.0f, 0.5f, 1.0f, 1.0f);

		vertices[2].SetPosition(1, 1, -1);
		vertices[2].SetColor(1.0f, 0.5f, 0.5f, 1.0f);

		vertices[3].SetPosition(1, -1, -1);
		vertices[3].SetColor(1.0f, 0.5f, 0.0f, 1.0f);

		vertices[4].SetPosition(-1, -1, 1);
		vertices[4].SetColor(1.0f, 1.0f, 0.0f, 1.0f);

		vertices[5].SetPosition(-1, 1, 1);
		vertices[5].SetColor(1.0f, 0.5f, 1.0f, 1.0f);

		vertices[6].SetPosition(1, 1, 1);
		vertices[6].SetColor(1.0f, 0.5f, 0.5f, 1.0f);

		vertices[7].SetPosition(1, -1, 1);
		vertices[7].SetColor(1.0f, 0.5f, 0.0f, 1.0f);

		int cube = mRenderer->CreateVertexBuffer(vertices, 8, cube_indices, 36);

		Renderer::MeshComponent triangle_one;
		triangle_one.VertexBuffer = triangle;
		triangle_one.x = 1;
		mRenderer->AttachMeshComponent(0, triangle_one);

		Renderer::MeshComponent triangle_two;
		triangle_two.VertexBuffer = triangle;
		triangle_two.y = 2;
		mRenderer->AttachMeshComponent(1, triangle_two);

		Renderer::MeshComponent triangle_three;
		triangle_three.VertexBuffer = triangle;
		triangle_three.x = -1;
		mRenderer->AttachMeshComponent(2, triangle_three);

		Renderer::MeshComponent cube_one;
		cube_one.VertexBuffer = cube;
		cube_one.x = -5.0f;
		mRenderer->AttachMeshComponent(3, cube_one);

		Renderer::MeshComponent plane;
		plane.VertexBuffer = mRenderer->CreatePlane(10.0f, 7.0f);
		plane.z = 1.0f;
		mRenderer->AttachMeshComponent(4, plane);
	}

	void OnUpdate() {

	}

	void OnShutdown() {
		mRenderer->DetachMeshComponent(4);
		mRenderer->DetachMeshComponent(3);
		mRenderer->DetachMeshComponent(2);
		mRenderer->DetachMeshComponent(1);
		mRenderer->DetachMeshComponent(0);
	}

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	PrimitivesApplication application;
	application.Run();
	return 0;
}

#endif