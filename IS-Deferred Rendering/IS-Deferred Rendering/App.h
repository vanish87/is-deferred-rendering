#ifndef APP_H_
#define APP_H_

#pragma once

#include <string>

#include "PreDec.h"

namespace MocapGE
{
	class MOCAPGE_API App
	{
	public:
		App(void);
		~App(void);
		App(std::string app_name);
		void Initialize();
		void Run();

		Window* InitializeWindow( std::string app_name, RenderSetting render_setting );
		Window& GetWindow()
		{
			return *win_;
		}

	private:
		std::string app_name_;
		Window* win_;

		static const int MAXIMUM_FRAME_RATE = 60;
		static const int UPDATE_INTERVAL = 1000 / MAXIMUM_FRAME_RATE;
		static const int MINIMUM_FRAME_RATE = 24;
		static const int MAX_CYCLES_PER_FRAME = MAXIMUM_FRAME_RATE / MINIMUM_FRAME_RATE ;


	protected:
		virtual void InitObjects()
		{
		}
		virtual void Update()
		{
		}
		virtual void ReleaseObjects()
		{
		}

	};
}
#endif

