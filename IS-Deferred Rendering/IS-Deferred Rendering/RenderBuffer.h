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
		RenderBuffer(BufferUsage usage, AccessType access_type)
			:usage_(usage), access_type_(access_type)
		{
		
		};
		~RenderBuffer(void);
		BufferUsage GetUsage();
	protected:
		BufferUsage usage_;
		AccessType access_type_;
	};
}

#endif

