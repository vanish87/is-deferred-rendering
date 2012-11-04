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


}