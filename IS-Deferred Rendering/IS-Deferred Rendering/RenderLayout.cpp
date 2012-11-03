#include "RenderLayout.h"

namespace MocapGE
{

	RenderLayout::RenderLayout(void)
	{
	}


	RenderLayout::~RenderLayout(void)
	{
	}

	void RenderLayout::AddBuffer( RenderBuffer* render_buffer )
	{
		LayoutInstance li;
		li.buffer = render_buffer;
		BufferUsage bu = render_buffer->GetUsage();
		switch (bu)
		{
		case BU_VERTEX:
			li.usage = VBU_VERTEX;
			break;
		case BU_INDEX:
			li.usage = VBU_INDEX;
			break;
			default:
				NULL;
				break;
		}
		data_.push_back(&li);
	}

	void RenderLayout::SetPrimitive( PrimitiveType primitive_type )
	{
		type_ = primitive_type;
	}

	void RenderLayout::SetInputLayout( std::vector<VertexUsage> input_layout )
	{
		input_layout_ = input_layout;
	}

}
