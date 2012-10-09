#include "RenderEngine.h"

namespace MocapGE
{
	RenderEngine::RenderEngine(void)
	{
	}


	RenderEngine::~RenderEngine(void)
	{
	}

	void RenderEngine::InitRenderWindow( std::string const & name, RenderSetting const & render_setting )
	{
		name_ = name;
		render_setting_ = render_setting;
	}

}
