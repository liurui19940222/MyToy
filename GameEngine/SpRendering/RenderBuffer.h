#ifndef _RENDER_BUFFER_H_
#define _RENDER_BUFFER_H_

namespace spgameengine
{

	class IRenderBuffer
	{
	public:

		virtual void BindBuffer() = 0;

		virtual void ReleaseBuffer() = 0;
	};
}
#endif