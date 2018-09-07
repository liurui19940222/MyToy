#ifndef _RENDER_BUFFER_H_
#define _RENDER_BUFFER_H_

#include"SpCommon\defs.h"

BEGIN_NAMESPACE_ENGINE

class IRenderBuffer
{
public:

	virtual void BindBuffer() = 0;

};

END_NAMESPACE_ENGINE

#endif