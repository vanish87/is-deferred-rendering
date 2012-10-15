#include "D3DTexture.h"
#include "Context.h"
#include "D3DRenderEngine.h"
#include "RenderView.h"

namespace MocapGE
{
	D3DTexture2D::D3DTexture2D(void)
	{
	}


	D3DTexture2D::~D3DTexture2D(void)
	{
	}

	ID3D11RenderTargetView* D3DTexture2D::GetRenderTargetView( int array_size, int mip_level )
	{
		if(d3d_rt_view_ == nullptr)
		{
			//check it first
			D3DRenderEngine* d3d_re = static_cast<D3DRenderEngine*>(&Context::Instance().GetRenderFactory().GetRenderEngine());
			D3D11_RENDER_TARGET_VIEW_DESC desc;
			assert(array_size == 1);
			if(array_size == 1)
			{
				desc.Format = d3d_re->MapFormat(this->format_);
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice = mip_level;
			}
			d3d_re->D3DDevice()->CreateRenderTargetView(this->D3DTexture(), &desc, &d3d_rt_view_);
		}
		return d3d_rt_view_;
	}

	size_t D3DTexture2D::GetWidth()
	{
		return width_;
	}

	size_t D3DTexture2D::GetHeight()
	{
		return height_;
	}

	Format D3DTexture2D::GetFormat()
	{
		return format_;
	}

	void D3DTexture2D::SetD3DTexture( ID3D11Texture2D* depth_stencil_buffer )
	{
		this->d3dTexture2D_ = depth_stencil_buffer;
	}

}
