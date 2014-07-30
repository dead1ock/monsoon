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

namespace Monsoon {
	namespace Renderer {

		typedef U32 VertexBufferHandle;
		typedef U32 MaterialHandle;

		struct MeshComponent
		{
			MeshComponent() {
				x = 0;
				y = 0;
				z = 0;
				yaw = 0;
				pitch = 0;
				roll = 0;
				VertexBuffer = -1;
				Material = -1;
			}
			VertexBufferHandle VertexBuffer;
			MaterialHandle Material;
			float x, y, z;
			float yaw, pitch, roll;
		};

		/**
		* Responsible for the setup and teardown of a Canvas (RenderWindow) and GPU resources (vertex buffers, shaders, meshes, textures, ect..).
		*/
		class DRIVER Renderer
		{
		public:
			Renderer(RendererSettings& settings) { };
			virtual ~Renderer() { };

			virtual bool Initialize() = 0;
			virtual void Shutdown() = 0;
			virtual bool Update() = 0;

			virtual int CreateVertexBuffer(ColorVertex* vertices, int vertexCount, unsigned int* indicies, int indexCount)=0;
			virtual void DestroyVertexBuffer(int vbHandle)=0;

			virtual void AttachMeshComponent(Entity entity, MeshComponent& component)=0;
			virtual void DetachMeshComponent(Entity entity)=0;
		};
	}
}
#endif // _MONSOON_RENDERER_RENDERER_H_