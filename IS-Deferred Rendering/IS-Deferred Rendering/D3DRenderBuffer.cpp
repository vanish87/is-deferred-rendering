#include "D3DRenderBuffer.h"

namespace MocapGE
{
	D3DRenderBuffer::D3DRenderBuffer(void)
	{
	}

	D3DRenderBuffer::D3DRenderBuffer( void* init_data, BufferUsage buffer_usage, AccessType access_type )
		: RenderBuffer(buffer_usage, access_type)
	{
		d3d_buffer_ = (ID3D11Buffer*)init_data;
		if(access_type_ == AT_GPU_READ || access_type_ == AT_GPU_WRITE)
			;//create shader resource view
	}


	D3DRenderBuffer::~D3DRenderBuffer(void)
	{
	}
}
