#include "D3DRenderFactory.h"
#include "D3DRenderEngine.h"

namespace MocapGE
{
	D3DRenderFactory::D3DRenderFactory(void)
		
	{
		render_engine_ = nullptr;
	}


	D3DRenderFactory::~D3DRenderFactory(void)
	{
	}

	RenderEngine* D3DRenderFactory::CreateRenderEngine()
	{
		return new D3DRenderEngine();
	}
	
	FrameBuffer* D3DRenderFactory::MakeFrameBuffer( RenderSetting& render_setting )
	{
		return new D3DFreamBuffer(render_setting);
	}

}