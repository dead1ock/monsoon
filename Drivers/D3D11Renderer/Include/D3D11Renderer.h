/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_DX11_RENDERER_
#define _MONSOON_DX11_RENDERER_

#include <vector>

#include <Renderer/Renderer.h>

#include "D3D.h"
#include "D3D11Window.h"
#include "D3D11ColorMaterial.h"
#include "Util/PackedArray.h"


namespace Monsoon {
	namespace Renderer{

		class DRIVER D3D11Renderer : Renderer
		{
		public:
			D3D11Renderer(RendererSettings& settings);
			~D3D11Renderer();

			bool Initialize();
			void Shutdown();
			bool Update();

			VertexBufferHandle CreateVertexBuffer(ColorVertex* vertices, int vertexCount, unsigned int* indicies, int indexCount);
			void DestroyVertexBuffer(VertexBufferHandle vbHandle);

			void AttachMeshComponent(Entity entity, MeshComponent& component);
			void DetachMeshComponent(Entity entity);

			MeshComponent& GetMeshComponent(Entity entity);

			VertexBufferHandle CreatePlane(float width, float height);
			VertexBufferHandle CreateCube(float length);
			VertexBufferHandle CreatePyramid(float base, float height);

			Camera& GetCamera() { return defaultCamera; }

		private:
			D3D11Window mWindow;
			D3D mD3d;
			std::vector<D3D11VertexBuffer> mVertexBuffers;
			std::vector<int> mFreeIndexList;
			Camera defaultCamera;

			Util::PackedArray<Entity, MeshComponent> mMeshComponents;
			D3D11ColorMaterial mColorMaterial;
		};
	}
}

#endif