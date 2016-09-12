/**
* Copyright (c) 2014-2016 Dallin Wellington
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
#include "D3D11GradientSkydomeMaterial.h"
#include "D3D11BarrelDistortionPostFx.h"
#include "Util/PackedPool.h"

const float RENDERSCALE = 1.0f;

namespace Monsoon {
	namespace Renderer{

		struct D3D11Texture : Texture
		{
			ID3D11ShaderResourceView* Resource;
		};

		class DRIVER D3D11Renderer : Renderer
		{
		public:
			D3D11Renderer(RendererSettings& settings, Event::EventManager* eventManager, Scene::TransformSystem* transformSystem);
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

			AtlasSheetHandle CreateAtlasSheet(AtlasSheet sheet);
			AtlasSheetHandle CreateGridAtlasSheet(float slicesWidth, float slicesHeight, float srcWidth, float srcHeight);
			void ReleaseAtlasSheet(AtlasSheetHandle sheet);

			Camera& GetCamera() { return defaultCamera; }

			// Creates a "Gradient" skydome and attaches it to the camera.
			// A gradient skydome takes two colors and interpolates between them from the apex (top) and bottom
			// of the skydome.
			void CreateGradientSkydome(Math::Vector3 apexColor, Math::Vector3 lowColor);

		private:
			enum EYE
			{
				BOTH,
				LEFT,
				RIGHT
			};

			void Render(EYE eye = BOTH);

			// Renders post processing fullscreen quad.
			void RenderPostProcessingQuad(EYE eye);

			D3D11Window mWindow;
			D3D mD3d;

			std::vector<D3D11VertexBuffer> mVertexBuffers;
			std::vector<VertexBufferHandle> mFreeIndexList;

			std::vector<D3D11Texture> mTextures;
			std::vector<TextureHandle> mTextureFreeList;

			std::vector<AtlasSheet> mAtlasSheets;
			std::vector<AtlasSheetHandle> mAtlasSheetFreeList;

			VertexBufferHandle mSkydomeVB;
			Math::Vector3 mSkydomeApexColor, mSkydomeBottomColor;

			Camera defaultCamera;

			Util::PackedPool<Entity, MeshComponent> mMeshComponents;
			Util::PackedPool<Entity, SpriteComponent> mSpriteComponents;

			// Materials
			D3D11ColorMaterial mColorMaterial;
			D3D11TextureMaterial mTextureMaterial;
			D3D11SpriteMaterial mSpriteMaterial;
			D3D11GradientSkydomeMaterial mGradientSkydomeMaterial;
			D3D11BarrelDistortionPostFx mBarrelDistortionPostFx;

			// Fixed Vertex Buffers
			VertexBufferHandle mSpritePlane;
			VertexBufferHandle mScreenAlignedQuadVB;	// Used rendering post processing effects.

			Scene::TransformSystem* mTransformSystem;
			Event::EventManager* mEventManager;
			RendererSettings mSettings;

			// Loads a mesh from a text file and returns a vertex buffer handle.
			VertexBufferHandle LoadMeshTxt(std::string filename);
		};
	}
}

#endif