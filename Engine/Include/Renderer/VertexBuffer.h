/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_VERTEX_BUFFER_H_
#define _MONSOON_VERTEX_BUFFER_H_

namespace Monsoon {
	namespace Renderer {

		struct ColorVertex
		{
			float x, y, z;
			float r, b, g, a;

			void SetPosition(float _x, float _y, float _z)
			{
				x = _x;
				y = _y;
				z = _z;
			}

			void SetColor(float _r, float _b, float _g, float _a)
			{
				r = _r;
				b = _b;
				g = _g;
				a = _a;
			}
		};

		/*
		 * VertexBuffer Interface.
		 */
		class VertexBuffer
		{
		public:
			VertexBuffer() { };
			virtual ~VertexBuffer() { };
		};

	}
}

#endif //_MONSOON_VERTEX_BUFFER_H_