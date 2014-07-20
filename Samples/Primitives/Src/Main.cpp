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

		unsigned long* indices = new unsigned long[3];
		Renderer::ColorVertex* vertices = new Renderer::ColorVertex[3];

		vertices[0].SetPosition(-4.0f, -1.0f, 0.0f);
		vertices[0].SetColor(1.0f, 0.0f, 0.0f, 1.0f);

		vertices[1].SetPosition(-3.0f, 1.0f, 0.0f);
		vertices[1].SetColor(1.0f, 0.0f, 0.0f, 1.0f);

		vertices[2].SetPosition(-2.0f, -1.0f, 0.0f);
		vertices[2].SetColor(1.0f, 0.0f, 0.0f, 1.0f);

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;

		mRenderer->CreateVertexBuffer(vertices, 3, indices, 3);

		delete indices;
		delete vertices;

		indices = new unsigned long[6];
		vertices = new Renderer::ColorVertex[4];

		vertices[0].SetPosition(2.0f, -1.0f, 0.0f);
		vertices[0].SetColor(1.0f, 1.0f, 0.0f, 1.0f);

		vertices[1].SetPosition(2.0f, 1.0f, 0.0f);
		vertices[1].SetColor(1.0f, 0.5f, 1.0f, 1.0f);

		vertices[2].SetPosition(4.0f, 1.0f, 0.0f);
		vertices[2].SetColor(1.0f, 0.5f, 0.5f, 1.0f);

		vertices[3].SetPosition(4.0f, -1.0f, 0.0f);
		vertices[3].SetColor(1.0f, 0.5f, 0.0f, 1.0f);

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 0;
		indices[4] = 2;
		indices[5] = 3;

		mRenderer->CreateVertexBuffer(vertices, 4, indices, 6);
	}

	void OnShutdown() {
		mRenderer->DestroyVertexBuffer(1);
		mRenderer->DestroyVertexBuffer(0);
	}

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	PrimitivesApplication application;
	application.Run();
	return 0;
}

#endif