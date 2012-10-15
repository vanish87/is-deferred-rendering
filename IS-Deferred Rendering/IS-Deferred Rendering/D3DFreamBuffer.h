#ifndef D3DFRAMEBUFFER_H_
#define D3DFRAMEBUFFER_H_

#pragma once
#include "FrameBuffer.h"
#include "D3DPreDec.h"
#include "D3DRenderView.h"
#include "D3DTexture.h"
#include "D3DRenderEngine.h"
#include "Context.h"

namespace MocapGE
{

	class D3DFreamBuffer : public FrameBuffer
	{
	public:
		D3DFreamBuffer(void);
		D3DFreamBuffer(RenderSetting& render_setting);
		~D3DFreamBuffer(void);

		D3DRenderTargetView* const &D3DRTView() const {return render_target_view_;};
		D3D11DepthStencilRenderView* const &D3DDSView() const {return depth_stencil_view_;};
		D3DTexture2D* const &D3DDSBuffer() const {return depth_stencil_buffer_;};

		virtual void OnBind();

	private:
		D3DRenderTargetView* render_target_view_;
		D3D11DepthStencilRenderView* depth_stencil_view_;
		D3DTexture2D* depth_stencil_buffer_;
	};
}


#endif
