#ifndef D3DTEXTURE_H_
#define D3DTEXTURE_H_

#pragma once

#include "PreDec.h"
#include "Texture.h"
#include "D3DPreDec.h"

namespace MocapGE
{
	class D3DTexture2D :	public Texture
	{
	public:
		D3DTexture2D(void);
		~D3DTexture2D(void);

		D3DTexture2D(uint32_t width, uint32_t height, uint32_t access_flag, uint32_t array_size, 
			Format format, uint32_t mip_level, uint32_t sample_count, uint32_t sample_quality, uint32_t usage, uint32_t bind_flag,
			InitData const * init_data);

		ID3D11Texture2D* const & D3DTexture() const
		{
			return d3dTexture2D_;
		}

		ID3D11RenderTargetView* GetRenderTargetView( int array_size, int mip_level );
		size_t GetWidth();
		size_t GetHeight();
		Format GetFormat();
		void SetD3DTexture( ID3D11Texture2D* texture );
	private:
		D3D11_TEXTURE2D_DESC desc_;
		ID3D11Texture2D* d3dTexture2D_;

		ID3D11RenderTargetView* d3d_rt_view_;
		ID3D11DepthStencilView* d3d_ds_view_;

		size_t	width_;
		size_t	height_;
	};

}

#endif

