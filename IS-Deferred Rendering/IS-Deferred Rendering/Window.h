#ifndef WINDOW_H_
#define WINDOW_H_

#pragma once

#include <windows.h>
#include <string>

#include "PreDec.h"
#include "Configure.h"

namespace MocapGE
{
	class MOCAPGE_API Window
	{
	public:
		Window(void);
		Window(std::string app_name, RenderSetting render_setting);
		~Window(void);

		HWND GetWnd()
		{
			return wnd_;
		}

	private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		LRESULT CALLBACK MsgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
		HWND wnd_;

		std::string win_name_;
	};
}


#endif

