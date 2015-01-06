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
#include "D3D11TextureMaterial.h"
#include "D3D11SpriteMaterial.h"
#include "Util/PackedPool.h"


namespace Monsoon {
	namespace Renderer{

		class DRIVER D3D11Renderer : Renderer
		{
		public:
			D3D11Renderer(RendererSettings& settings, Scene::SpatialSystem* spatialSystem);
			~D3D11Renderer();

			bool Initialize();
			void Shutdown();
			bool Update();

			VertexBufferHandle CreateVertexBuffer(VertexType vertices[], int vertexCount, unsigned int indicies[], int indexCount);
			void DestroyVertexBuffer(VertexBufferHandle vbHandle);

			// 3D Rendering
			void AttachMeshComponent(Entity entity, MeshComponent& component);
			void DetachMeshComponent(Entity entity);
			MeshComponent& GetMeshComponent(Entity entity);

			// 2D Rendering
			void AttachSpriteComponent(Entity entity, SpriteComponent& component);
			void DetachSpriteComponent(Entity entity);
			SpriteComponent& GetSpriteComponent(Entity entity);

			VertexBufferHandle CreatePlane(float width, float height);
			VertexBufferHandle CreateCube(float length);
			VertexBufferHandle CreatePyramid(float base, float height);
			VertexBufferHandle CreateCylinder(U32 sections);

			U32 LoadTexture(std::string filename);
			void ReleaseTexture(U32 textureId);

			SpriteSheetHandle CreateSpriteSheet(SpriteSheet sheet);
			void ReleaseSpriteSheet(SpriteSheetHandle sheet);

			Camera& GetCamera() { return defaultCamera; }

		private:
			D3D11Window mWindow;
			D3D mD3d;
			std::vector<D3D11VertexBuffer> mVertexBuffers;
			std::vector<VertexBufferHandle> mFreeIndexList;

			std::vector<ID3D11ShaderResourceView*> mTextures;
			std::vector<TextureHandle> mTextureFreeList;

			std::vector<SpriteSheet> mSpriteSheets;
			std::vector<SpriteSheetHandle> mSpriteSheetFreeList;

			Camera defaultCamera;

			Util::PackedPool<Entity, MeshComponent> mMeshComponents;
			Util::PackedPool<Entity, SpriteComponent> mSpriteComponents;

			// Materials
			D3D11ColorMaterial mColorMaterial;
			D3D11TextureMaterial mTextureMaterial;
			D3D11SpriteMaterial mSpriteMaterial;

			VertexBufferHandle mSpritePlane;

			Scene::SpatialSystem* mSpatialSystem;
		};
	}
}

#endif