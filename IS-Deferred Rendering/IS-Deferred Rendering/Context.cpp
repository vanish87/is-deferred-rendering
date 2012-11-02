#include "Context.h"
#include "D3DRenderFactory.h"

namespace MocapGE
{
	Context::Context(void)
	{
	}


	Context::~Context(void)
	{
	}

	Context& Context::Instance()
	{
		static Context context;
		return context;
	}

	void Context::LoadConfig( std::string const & cfg_file )
	{
		XMLParserPtr xml_parser_ptr_=boost::shared_ptr<XMLParser>(new XMLParser);
		xml_parser_ptr_->LoadFile(cfg_file);

		XMLNodePtr configure=xml_parser_ptr_->GetRoot("configure");
		XMLNodePtr context=configure->GetChild("context");
		std::string render_factory_name=context->GetChild("render_factory")->GetAttri("name")->GetValue();

		XMLNodePtr frame=configure->GetChild("render")->GetChild("frame");

		int width = atoi(frame->GetAttri("width")->GetValue().c_str());
		int height = atoi(frame->GetAttri("height")->GetValue().c_str());
		short full_screen = atoi(frame->GetAttri("fullscreen")->GetValue().c_str());

		XMLNodePtr hdr_ptr = configure->GetChild("render")->GetChild("msaa4x");
		short msaa4x = atoi(hdr_ptr->GetAttri("value")->GetValue().c_str());

		XMLNodePtr gamma_ptr = configure->GetChild("render")->GetChild("gamma");
		short gamma = atoi(gamma_ptr->GetAttri("value")->GetValue().c_str());

		XMLNodePtr deferred_rendering_ptr = configure->GetChild("render")->GetChild("deferred_rendering");
		short deferred_rendering = atoi(deferred_rendering_ptr->GetAttri("value")->GetValue().c_str());

		context_config_.render_factory_name=render_factory_name;

		context_config_.render_setting.width=width;
		context_config_.render_setting.height=height;
		context_config_.render_setting.left=0;
		context_config_.render_setting.top=0;
		context_config_.render_setting.full_screen=full_screen;
		context_config_.render_setting.msaa4x=msaa4x;
		context_config_.render_setting.gamma=gamma;
		context_config_.render_setting.deferred_rendering=deferred_rendering;
	}

	ContextConfig const & Context::GetConfigure() const
	{
		return context_config_;
	}

	void Context::Configure( ContextConfig const & cfg )
	{
		if(!context_config_.render_factory_name.compare(cfg.render_factory_name) || render_factory_==nullptr)
		{
			//this->LoadRenderFactory(cfg.render_factory_name);
			render_factory_= new D3DRenderFactory();
		}

		scene_manager_ = new SceneManager();

		context_config_ = cfg;
	}

	RenderFactory& Context::GetRenderFactory()
	{
		if(render_factory_ == nullptr)std::cout<< "no Render Factory"<<std::endl;
		return *render_factory_;
	}

	SceneManager& Context::GetSceneManager()
	{
		if(scene_manager_ == nullptr)std::cout<< "no Scene Manager"<<std::endl;
		return *scene_manager_;
	}

}

