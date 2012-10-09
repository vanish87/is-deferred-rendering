#include "D3DRenderFactory.h"
#include "D3DRenderEngine.h"

namespace MocapGE
{
	D3DRenderFactory::D3DRenderFactory(void)
	{
	}


	D3DRenderFactory::~D3DRenderFactory(void)
	{
	}

	RenderEngine* D3DRenderFactory::CreateRenderEngine()
	{
		return new D3DRenderEngine();
	}

}