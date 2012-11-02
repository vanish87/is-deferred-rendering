#ifndef RENDERBUFFER_H_
#define RENDERBUFFER_H_

#pragma once

#include "PreDec.h"
#include "TypeAndFormat.h"

namespace MocapGE
{
	class RenderBuffer
	{
	public:
		RenderBuffer(void);
		~RenderBuffer(void);

	protected:
		BufferUsage usage_;
	};
}

#endif

