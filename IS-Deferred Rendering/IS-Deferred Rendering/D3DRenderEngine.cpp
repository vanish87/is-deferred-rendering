#include "D3DRenderEngine.h"
#include "Context.h"
#include "App.h"
#include "Window.h"
#include "D3DFreamBuffer.h"

namespace MocapGE
{
	D3DRenderEngine::D3DRenderEngine(void)
	{		
		cur_frame_buffer_ = nullptr;
	}


	D3DRenderEngine::~D3DRenderEngine(void)
	{
	}

	void D3DRenderEngine::InitRenderWindow( std::string const & name, RenderSetting const & render_setting )
	{
		RenderEngine::InitRenderWindow(name, render_setting);

		HRESULT result;
		UINT create_device_flags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
		create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
		result = D3D11CreateDevice(
				0,                 // default adapter
				md3dDriverType,
				0,                 // no software device
				create_device_flags, 
				0, 0,              // default feature level array
				D3D11_SDK_VERSION,
				&d3d_device_,
				&d3d_feature_level_,
				&d3d_imm_ctx_);

		if( FAILED(result) )
		{
			PRINT("D3D11CreateDevice Failed.");
		}

		if( d3d_feature_level_ != D3D_FEATURE_LEVEL_11_0 )
		{
			PRINT("Direct3D Feature Level 11 unsupported.");
		}
		UINT msaa_quality = 0;
		result = d3d_device_->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaa_quality);
		if(FAILED(result) )
		{
			PRINT("msaa_quality Failed.");
		}

		DXGI_SWAP_CHAIN_DESC swap_chain_desc;
		swap_chain_desc.BufferDesc.Width  = render_setting_.width;
		swap_chain_desc.BufferDesc.Height = render_setting_.height;
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
		swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// Use 4X MSAA? 
		if( render_setting_.msaa4x )
		{
			swap_chain_desc.SampleDesc.Count   = 4;
			swap_chain_desc.SampleDesc.Quality = msaa_quality-1;
		}
		// No MSAA
		else
		{
			swap_chain_desc.SampleDesc.Count   = 1;
			swap_chain_desc.SampleDesc.Quality = 0;
		}

		swap_chain_desc.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.BufferCount  = 1;
		swap_chain_desc.OutputWindow = Context::Instance().AppInstance().GetWindow().GetWnd();
		swap_chain_desc.Windowed     = true;
		swap_chain_desc.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
		swap_chain_desc.Flags        = 0;

		IDXGIDevice* dxgi_device = 0;
		result = d3d_device_->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgi_device);
		if( FAILED(result) )
		{
			PRINT("dxgiDevice Failed.");
		}

		IDXGIAdapter* dxgi_adapter = 0;
		result = dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgi_adapter);
		if( FAILED(result) )
		{
			PRINT("dxgiAdapter Failed.");
		}

		IDXGIFactory* dxgiFactory = 0;
		result = dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
		if( FAILED(result) )
		{
			PRINT("dxgiFactory Failed.");
		}

		result = dxgiFactory->CreateSwapChain(d3d_device_, &swap_chain_desc, &d3d_swap_chain);
		if( FAILED(result) )
		{
			PRINT("d3d_swap_chain Failed.");
		}

		dxgi_device->Release();
		dxgi_adapter->Release();
		dxgiFactory->Release();


		this->OnResize();

	}

	void D3DRenderEngine::Render()
	{
		float color[4] = {0.0f,0.0f,1.0f,1.0f};
		D3DFreamBuffer* d3d_frame_buffer;
		d3d_frame_buffer= static_cast<D3DFreamBuffer*>(cur_frame_buffer_);
		d3d_imm_ctx_->ClearRenderTargetView(d3d_frame_buffer->D3DRTView()->D3DRTV(), color);
		d3d_imm_ctx_->ClearDepthStencilView(d3d_frame_buffer->D3DDSView()->D3DDSV(), D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

		d3d_swap_chain->Present(0, 0);
	}

	void D3DRenderEngine::SwapBuffers()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3DRenderEngine::OnResize()
	{
		HRESULT result;
		D3DFreamBuffer* d3d_frame_buffer;
		if(cur_frame_buffer_ == nullptr)
		{
			cur_frame_buffer_ = Context::Instance().GetRenderFactory().MakeFrameBuffer(render_setting_);
		    d3d_frame_buffer= static_cast<D3DFreamBuffer*>(cur_frame_buffer_);
		}
		else
		{
			//check it
			d3d_frame_buffer= static_cast<D3DFreamBuffer*>(cur_frame_buffer_);
			d3d_frame_buffer->D3DRTView()->D3DRTV()->Release();
			d3d_frame_buffer->D3DDSView()->D3DDSV()->Release();
		}


		result = d3d_swap_chain->ResizeBuffers(1, render_setting_.width, render_setting_.height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
		if(FAILED(result))
			PRINT("ResizeBuffer Failed!");

		ID3D11Texture2D* back_buffer;
		ID3D11RenderTargetView* render_target_view = d3d_frame_buffer->D3DRTView()->D3DRTV();
		result = d3d_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));
		if(FAILED(result))
			PRINT("GetBuffer Failed!");
		result = d3d_device_->CreateRenderTargetView(back_buffer, 0, &render_target_view);
		if(FAILED(result))
			PRINT("CreateRenderTargetView Failed!");
		back_buffer->Release();

		D3D11_TEXTURE2D_DESC depth_stencil_desc;
	
		depth_stencil_desc.Width     = render_setting_.width;
		depth_stencil_desc.Height    = render_setting_.height;
		depth_stencil_desc.MipLevels = 1;
		depth_stencil_desc.ArraySize = 1;
		depth_stencil_desc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;



		if( render_setting_.msaa4x )
		{
			UINT msaa_quality = 0;
			result = d3d_device_->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaa_quality);
			if(FAILED(result) )
			{
				PRINT("msaa_quality Failed.");
			}
			depth_stencil_desc.SampleDesc.Count   = 4;
			depth_stencil_desc.SampleDesc.Quality = msaa_quality-1;
		}
		// No MSAA
		else
		{
			depth_stencil_desc.SampleDesc.Count   = 1;
			depth_stencil_desc.SampleDesc.Quality = 0;
		}

		depth_stencil_desc.Usage          = D3D11_USAGE_DEFAULT;
		depth_stencil_desc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
		depth_stencil_desc.CPUAccessFlags = 0; 
		depth_stencil_desc.MiscFlags      = 0;

		ID3D11Texture2D* depth_stencil_buffer = d3d_frame_buffer->D3DDSBuffer()->D3DTexture();
		ID3D11DepthStencilView*	depth_stencil_view = d3d_frame_buffer->D3DDSView()->D3DDSV();

		result = d3d_device_->CreateTexture2D(&depth_stencil_desc, 0, &depth_stencil_buffer);
		if(FAILED(result))
			PRINT("depth_stencil create Failed!");

		result = d3d_device_->CreateDepthStencilView(depth_stencil_buffer, 0, &depth_stencil_view);
		if(FAILED(result))
			PRINT("depth_stencil_view create Failed!");


		d3d_frame_buffer->D3DRTView()->SetD3DRTV(render_target_view);
		d3d_frame_buffer->D3DDSBuffer()->SetD3DTexture(depth_stencil_buffer);
		d3d_frame_buffer->D3DDSView()->SetD3DDSV(depth_stencil_view);

		this->BindFrameBuffer(d3d_frame_buffer);


	}

	DXGI_FORMAT D3DRenderEngine::MapFormat( Format format )
	{
		switch (format)
		{
		case MocapGE::A8_U:
			return DXGI_FORMAT_A8_UNORM;
			break;
		case MocapGE::R8_U:
			return DXGI_FORMAT_R8_UNORM;
			break;
		case MocapGE::R8G8B8A8_U:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		default:
			return DXGI_FORMAT_R32G32B32A32_UINT;
			break;
		}
	}

	void D3DRenderEngine::BindFrameBuffer( FrameBuffer* const & fb )
	{
		fb->OnBind();
	}

}

