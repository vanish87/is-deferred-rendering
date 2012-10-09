#ifndef D3DRENDERENGINE_H_
#define D3DRENDERENGINE_H_

#pragma once

#include "RenderEngine.h"
#include "D3DPreDec.h"

namespace MocapGE
{
	class D3DRenderEngine :	public RenderEngine
	{
	public:
		D3DRenderEngine(void);
		~D3DRenderEngine(void);

		virtual void InitRenderWindow( std::string const & name, RenderSetting const & render_setting );

		virtual void Render();

		virtual void SwapBuffers();

	private:
		IDXGIFactory1*	gi_factory_;
		ID3D11Device*		d3d_device_;
		ID3D11DeviceContext* d3d_imm_ctx_;
		D3D_FEATURE_LEVEL d3d_feature_level_;



	};
}


#endif

