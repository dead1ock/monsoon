/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_VERTEX_BUFFER_H_
#define _MONSOON_VERTEX_BUFFER_H_

namespace monsoon {
	namespace renderer {

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