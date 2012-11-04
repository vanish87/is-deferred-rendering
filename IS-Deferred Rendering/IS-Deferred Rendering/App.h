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

		static const int TICKS_PER_SECOND = 50;
		static const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
		static const int MAX_FRAMESKIP = 2;

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

