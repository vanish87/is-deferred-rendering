#ifndef RENDERLAYOUT_H_
#define RENDERLAYOUT_H_

#pragma once
#include <vector>

#include "PreDec.h"
#include "TypeAndFormat.h"
#include "RenderBuffer.h"

namespace MocapGE
{
	class RenderLayout
	{
	public:
		RenderLayout(void);
		~RenderLayout(void);

		virtual RenderLayout* GetLayout() = 0;
		virtual void AddBuffer(RenderBuffer* render_buffer);
		void SetPrimitive( PrimitiveType primitive_type );
		void SetInputLayout( std::vector<VertexUsage> input_layout );


	protected:
		struct LayoutInstance
		{
			RenderBuffer* buffer;
			VertexBufferUsage usage;
		};
		std::vector<LayoutInstance*> data_;
		PrimitiveType type_;
		std::vector<VertexUsage> input_layout_;
	};
}

#endif

