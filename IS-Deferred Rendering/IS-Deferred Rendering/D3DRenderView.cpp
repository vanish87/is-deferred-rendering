#include "D3DRenderView.h"

namespace MocapGE
{

	D3DRenderTargetView::D3DRenderTargetView(void)
		:render_target_view_(nullptr)
	{
	}

	D3DRenderTargetView::D3DRenderTargetView( Texture& texture, int array_size, int mip_level )
	{
		D3DTexture2D* d3d_t2d = static_cast<D3DTexture2D*>(&texture);
		render_target_view_ = d3d_t2d->GetRenderTargetView( array_size, mip_level );
		width_ = d3d_t2d->GetWidth();
		height_ = d3d_t2d->GetHeight();

		format_ = d3d_t2d->GetFormat();
		dimension_ = TextureType::TEXTURE2D;
	}


	D3DRenderTargetView::~D3DRenderTargetView(void)
	{
	}

	void D3DRenderTargetView::SetD3DRTV( ID3D11RenderTargetView* render_target_view )
	{
		this->render_target_view_ = render_target_view;
	}

	D3D11DepthStencilRenderView::D3D11DepthStencilRenderView( void )
		:depth_stencil_view_(nullptr)
	{

	}

	void D3D11DepthStencilRenderView::SetD3DDSV( ID3D11DepthStencilView* depth_stencil_view )
	{
		this->depth_stencil_view_=depth_stencil_view;
	}

}
