/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_VERTEX_BUFFER_H_
#define _MONSOON_VERTEX_BUFFER_H_

namespace Monsoon {
	namespace Renderer {

		struct VertexType
		{
			float x, y, z;
			float r, b, g, a;
			float u, v;

			VertexType()
			{
				x = 0.0f;
				y = 0.0f;
				z = 0.0f;
				r = 0.0f;
				b = 0.0f;
				g = 0.0f;
				a = 1.0f;
				u = 0.0f;
				v = 0.0f;
			}

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

			void SetUV(float _u, float _v)
			{
				u = _u;
				v = _v;
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