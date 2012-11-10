#ifndef D3DRENDRBUFFER_H_
#define D3DRENDRBUFFER_H_

#pragma once
#include "RenderBuffer.h"

#include "D3DPreDec.h"

namespace MocapGE
{
	class D3DRenderBuffer : public RenderBuffer
	{
	public:
		D3DRenderBuffer(void);
		~D3DRenderBuffer(void);

		D3DRenderBuffer(void* init_data, BufferUsage buffer_usage, AccessType access_type);

		ID3D11Buffer* & D3DBuffer(){return d3d_buffer_;};
		ID3D11ShaderResourceView* & D3DShaderResourceView(){return d3d_shader_resource_view_;};

		virtual void UnMap();


	private:
		ID3D11Buffer* d3d_buffer_;
		ID3D11ShaderResourceView* d3d_shader_resource_view_;

	private:
		virtual void DoMap( AccessType access_type );
	};


}

#endif
