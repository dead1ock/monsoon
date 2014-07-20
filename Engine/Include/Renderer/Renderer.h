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

			virtual void CreateVertexBuffer(ColorVertex* vertices, int vertexCount, unsigned long* indicies, int indexCount)=0;
		};
	}
}
#endif // _MONSOON_RENDERER_RENDERER_H_