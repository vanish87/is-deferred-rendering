#include "RenderElement.h"

namespace MocapGE
{
	RenderElement::RenderElement(void)
	{
	}


	RenderElement::~RenderElement(void)
	{
	}

	ShaderObject* RenderElement::GetShaderObject()
	{
		return shader_object_;
	}

	void RenderElement::SetShaderObject( ShaderObject* shader_object )
	{
		this->shader_object_ = shader_object;
	}

	void RenderElement::LoadShaderFile( std::string file_name )
	{
		//load basic shader file for render element
		
	}


}