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

		this->InitObjects();

	}

	void App::Run()
	{
		MSG msg = {0};
		DWORD next_game_tick;
		int loops;
		next_game_tick = GetTickCount();

		while( WM_QUIT != msg.message )
		{
			if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{
				loops = 0;
				while( GetTickCount() > next_game_tick && loops < MAX_FRAMESKIP) 
				{
					this->Update();//do user's update

					Context::Instance().GetSceneManager().Update();

					next_game_tick += SKIP_TICKS;
					loops++;
				}
				Context::Instance().GetSceneManager().Flush();

			}
		}
	}

	Window* App::InitializeWindow( std::string app_name, RenderSetting render_setting )
	{
		return new Window(app_name,render_setting);
	}

}
