#include "D3DFrameBuffer.h"

namespace MocapGE
{
	D3DFreamBuffer::D3DFreamBuffer(void)
		: depth_stencil_buffer_(new D3DTexture2D),
		  depth_stencil_view_(new D3DDepthStencilRenderView)
	{
		render_view_ = new D3DRenderTargetView();
	}

	D3DFreamBuffer::D3DFreamBuffer( RenderSetting& render_setting )
		: depth_stencil_buffer_(new D3DTexture2D),
		  depth_stencil_view_(new D3DDepthStencilRenderView)		
	{
		viewport_ = new Viewport(0 , 0,render_setting.width,render_setting.height);
		render_view_ = new D3DRenderTargetView();
	}


	D3DFreamBuffer::~D3DFreamBuffer(void)
	{
	}

	void D3DFreamBuffer::OnBind()
	{
		D3DRenderEngine* render_engine = static_cast<D3DRenderEngine*>(&Context::Instance().GetRenderFactory().GetRenderEngine());
		ID3D11RenderTargetView* rtv = static_cast<D3DRenderTargetView*>(this->render_view_)->D3DRTV();
		ID3D11DepthStencilView* dsv = this->depth_stencil_view_->D3DDSV();
		render_engine->D3DDeviceImmContext()->OMSetRenderTargets(1, &rtv , dsv);

		D3D11_VIEWPORT d3d_viewport;
		d3d_viewport.TopLeftX = viewport_->Top();
		d3d_viewport.TopLeftY = viewport_->Left();
		d3d_viewport.Width    = static_cast<float>(viewport_->Width());
		d3d_viewport.Height   = static_cast<float>(viewport_->Height());
		d3d_viewport.MinDepth = 0.0f;//get info from frustum
		d3d_viewport.MaxDepth = 1.0f;

		render_engine->D3DDeviceImmContext()->RSSetViewports(1, &d3d_viewport);
	}

}
