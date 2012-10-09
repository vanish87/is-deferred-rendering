#ifndef CONFIGURE_H_
#define CONFIGURE_H_

#pragma once

#include <string>

namespace MocapGE
{
	struct RenderSetting
	{
		bool	full_screen;
		int		left;
		int		top;
		int		width;
		int		height;

		bool	hdr;
		bool	gamma;

		bool deferred_rendering;
	};
	struct ContextConfig
	{
		std::string render_factory_name;

		RenderSetting render_setting;
	};
}

#endif