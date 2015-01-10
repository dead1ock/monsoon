/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_RENDERER_H_
#define _MONSOON_RENDERER_H_

#include "Platform/Export.h"
#include "Platform/Types.h"

#include "RendererSettings.h"
#include "VertexBuffer.h"

#include "Scene/SpatialSystem.h"

namespace Monsoon {
	namespace Renderer {

		typedef U32 VertexBufferHandle;
		typedef U32 MaterialHandle;
		typedef U32 AtlasSheetHandle;
		typedef U32 TextureHandle;

		//
		// 3D Rendering
		//
		struct MeshComponent
		{
			MeshComponent() {
				VertexBuffer = -1;
				TextureId = -1;
			}

			VertexBufferHandle VertexBuffer;
			TextureHandle TextureId;
		};

		//
		// 2D Rendering
		//
		struct AtlasSprite
		{
			AtlasSprite()
			{
				uOffset = 0.0f;
				vOffset = 0.0f;
				spriteWidth = 0.0f;
				spriteHeight = 0.0f;
			}
			float uOffset;
			float vOffset;
			int spriteWidth;
			int spriteHeight;
		};

		struct AtlasSheet
		{
			std::vector<AtlasSprite> SrcRects;
		};

		struct SpriteComponent
		{
			// Specifies whether to render this sprite from a 
			// single image source, or a slice from an atlas.
			enum SpriteMode
			{
				SINGLE,
				ATLAS
			};

			SpriteComponent() {
				Texture = -1;
				AtlasSheet = -1;
				Mode = SINGLE;
				AtlasIndex = 0;
				ZOrder = 0;
			}

			SpriteMode Mode;
			AtlasSheetHandle AtlasSheet;
			TextureHandle Texture;
			int AtlasIndex;
			int ZOrder;
		};

		//
		//
		//

		struct Texture
		{
			std::string Filename;
			int Width;
			int Height;
		};

		struct Camera
		{
			enum Projection
			{
				PERSPECTIVE,
				ORTHOGRAPHIC
			};

			Camera() {
				x = 0.0f;
				y = 0.0f;
				z = 0.0f;
				
				yaw = 0.0f;
				pitch = 0.0f;
				roll = 0.0f;

				mode = PERSPECTIVE;
				
				orthoWidth = 45.0f;
				orthoHeight = 30.0f;

				nearClip = 0.1f;
				farClip = 1000.0f;

				lookAtX = 0.0f;
				lookAtY = 0.0f;
				lookAtZ = 0.0f;
			}

			float x, y, z;
			float yaw, pitch, roll;
			Projection mode;
			float orthoWidth, orthoHeight;
			float nearClip, farClip;
			float lookAtX, lookAtY, lookAtZ;
		};

		/**
		* Responsible for the setup and teardown of a Canvas (RenderWindow) and GPU resources (vertex buffers, shaders, meshes, textures, ect..).
		*/
		class DRIVER Renderer
		{
		public:
			Renderer(RendererSettings& settings, Scene::SpatialSystem* spatialSystem) { };
			virtual ~Renderer() { };

			virtual bool Initialize() = 0;
			virtual void Shutdown() = 0;
			virtual bool Update() = 0;

			virtual VertexBufferHandle CreateVertexBuffer(VertexType vertices[], int vertexCount, unsigned int indicies[], int indexCount) = 0;
			virtual void DestroyVertexBuffer(VertexBufferHandle vbHandle) = 0;

			// 3D Rendering
			virtual void AttachMeshComponent(Entity entity, MeshComponent& component) = 0;
			virtual void DetachMeshComponent(Entity entity) = 0;
			virtual MeshComponent& GetMeshComponent(Entity entity) = 0;

			// 2D Rendering
			virtual void AttachSpriteComponent(Entity entity, SpriteComponent& component) = 0;
			virtual void DetachSpriteComponent(Entity entity) = 0;
			virtual SpriteComponent& GetSpriteComponent(Entity entity) = 0;

			virtual AtlasSheetHandle CreateAtlasSheet(AtlasSheet sheet) = 0;
			virtual void ReleaseAtlasSheet(AtlasSheetHandle sheet) = 0;

			// Primitives
			virtual VertexBufferHandle CreatePlane(float width, float height) = 0;
			virtual VertexBufferHandle CreateCube(float length) = 0;
			virtual VertexBufferHandle CreatePyramid(float base, float height) = 0;
			virtual VertexBufferHandle CreateCylinder(U32 sections) = 0;

			virtual TextureHandle LoadTexture(std::string filename) = 0;
			virtual void ReleaseTexture(TextureHandle textureHandle) = 0;
			//virtual const Texture& GetTexture(TextureHandle textureHandle) = 0;

			// Camera
			virtual Camera& GetCamera() = 0;
		};
	}
}
#endif // _MONSOON_RENDERER_RENDERER_H_