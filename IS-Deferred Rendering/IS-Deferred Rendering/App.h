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

	private:
		std::string app_name_;
		Window* win_;

	protected:
		virtual void InitObjects()
		{
		}virtual void ReleaseObjects()
		{
		}
	};
}
#endif

