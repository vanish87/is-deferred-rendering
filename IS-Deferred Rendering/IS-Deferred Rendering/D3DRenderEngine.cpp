#include "D3DRenderEngine.h"

namespace MocapGE
{
	D3DRenderEngine::D3DRenderEngine(void)
	{
		HRESULT result;
		result = ::CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&gi_factory_);
		if(FAILED(result))
		{
			PRINT("CreateDXGIFactory Failed");
		}

		IDXGIAdapter* adapter;
		result = gi_factory_->EnumAdapters(0, &adapter);
		if(FAILED(result))
		{
			PRINT("EnumAdapters Failed");
		}
	}


	D3DRenderEngine::~D3DRenderEngine(void)
	{
	}

	void D3DRenderEngine::InitRenderWindow( std::string const & name, RenderSetting const & render_setting )
	{
		RenderEngine::InitRenderWindow(name, render_setting);


		throw std::exception("The method or operation is not implemented.");
	}

	void D3DRenderEngine::Render()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void D3DRenderEngine::SwapBuffers()
	{
		throw std::exception("The method or operation is not implemented.");
	}

}

