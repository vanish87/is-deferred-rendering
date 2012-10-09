#include "App.h"
#include "Context.h"
#include "Window.h"

namespace MocapGE
{
	App::App(void)
	{
	}

	App::App( std::string app_name )
		:app_name_(app_name)
	{
		Context::Instance().SetAppInstance(*this);

		ContextConfig config = Context::Instance().GetConfigure();
		win_ = this->InitializeWindow(app_name_, config.render_setting);

		Context::Instance().Configure(config);
	}


	App::~App(void)
	{
	}

	void App::Initialize()
	{
		ContextConfig config = Context::Instance().GetConfigure();
		Context::Instance().GetRenderFactory().GetRenderEngine().InitRenderWindow(app_name_ , config.render_setting);

		//init deferred rendering here


	}

	void App::Run()
	{

	}

	Window* App::InitializeWindow( std::string app_name, RenderSetting render_setting )
	{
		return new Window(app_name,render_setting);
	}

}
