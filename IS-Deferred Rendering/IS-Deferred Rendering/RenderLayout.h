#ifndef RENDERLAYOUT_H_
#define RENDERLAYOUT_H_

#pragma once

#include "PreDec.h"
#include "TypeAndFormat.h"

namespace MocapGE
{
	class RenderLayout
	{
	public:
		RenderLayout(void);
		~RenderLayout(void);

		virtual RenderLayout* GetLayout() = 0;

	protected:

		RenderBuffer* data_;
		PrimitiveType type_;
		VertexUsage usage_;
	};
}

#endif

