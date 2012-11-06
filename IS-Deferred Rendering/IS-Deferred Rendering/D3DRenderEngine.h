#ifndef D3DRENDERENGINE_H_
#define D3DRENDERENGINE_H_

#pragma once

#include "Context.h"
#include "App.h"
#include "Window.h"
#include "RenderEngine.h"
#include "D3DPreDec.h"
#include "TypeAndFormat.h"
#include "D3DRenderLayout.h"
#include "D3DFrameBuffer.h"
#include "D3DShaderobject.h"
#include "D3DRenderBuffer.h"

#pragma comment(lib, "d3d11.lib") //may fix in the future

namespace MocapGE
{
	class D3DRenderEngine :	public RenderEngine
	{
	public:
		D3DRenderEngine(void);
		~D3DRenderEngine(void);

		virtual void InitRenderWindow( std::string const & name, RenderSetting const & render_setting );

		virtual void Render(RenderLayout* render_layout, ShaderObject* shader_object);

		virtual void SwapBuffers();

		virtual void OnResize();

		ID3D11Device* const D3DDevice() const{return d3d_device_;};
		ID3D11DeviceContext* const & D3DDeviceImmContext() const{return d3d_imm_context_;};

		DXGI_FORMAT MapFormat(Format format);

		virtual void BindFrameBuffer( FrameBuffer* const & fb );

		virtual void RenderFrameBegin();

		virtual void RenderFrameEnd();

	private:
		ID3D11Device*		d3d_device_;
		ID3D11DeviceContext* d3d_imm_context_;
		D3D_FEATURE_LEVEL d3d_feature_level_;
		IDXGISwapChain* d3d_swap_chain;



		


	};
}


#endif

