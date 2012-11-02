#ifndef CONTEXT_H_
#define CONTEXT_H_

#pragma once

#include <string>

#include "PreDec.h"
#include "Configure.h"
#include "XMLParser.h"
#include "RenderFactory.h"
#include "SceneManager.h"

namespace MocapGE
{
	class MOCAPGE_API Context
	{
	public:
		Context(void);
		~Context(void);

		static Context& Instance();

		void LoadConfig(std::string const & cfg_file);

		void Configure(ContextConfig const & cfg);
		ContextConfig const & GetConfigure() const;

		RenderFactory& GetRenderFactory();
		SceneManager& GetSceneManager();

		void SetAppInstance(App &app)
		{
			app_ = &app;
		}

		App& AppInstance()
		{
			return *app_;
		}
	private:
		ContextConfig context_config_;

		App*		app_;

		RenderFactory*	render_factory_;
		SceneManager* scene_manager_;
	};
}

#endif

