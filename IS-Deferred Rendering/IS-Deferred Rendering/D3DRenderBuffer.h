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

	private:
		ID3D11Buffer* d3d_buffer_;
		ID3D11ShaderResourceView* d3d_shader_resource_view_;
	};

}

#endif
