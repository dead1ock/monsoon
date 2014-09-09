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

		struct MeshComponent
		{
			MeshComponent() {
				VertexBuffer = -1;
				Material = -1;
			}
			VertexBufferHandle VertexBuffer;
			MaterialHandle Material;
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
			}

			float x, y, z;
			float yaw, pitch, roll;
			Projection mode;
			float orthoWidth, orthoHeight;
			float nearClip, farClip;
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

			virtual VertexBufferHandle CreateVertexBuffer(ColorVertex* vertices, int vertexCount, unsigned int* indicies, int indexCount) = 0;
			virtual void DestroyVertexBuffer(VertexBufferHandle vbHandle) = 0;

			virtual void AttachMeshComponent(Entity entity, MeshComponent& component)=0;
			virtual void DetachMeshComponent(Entity entity)=0;
			virtual MeshComponent& GetMeshComponent(Entity entity) = 0;

			// Primitives
			virtual VertexBufferHandle CreatePlane(float width, float height) = 0;
			virtual VertexBufferHandle CreateCube(float length) = 0;
			virtual VertexBufferHandle CreatePyramid(float base, float height) = 0;

			// Camera
			virtual Camera& GetCamera() = 0;
		};
	}
}
#endif // _MONSOON_RENDERER_RENDERER_H_