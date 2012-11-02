#ifndef RENDERELEMENT_H_
#define RENDERELEMENT_H_

#pragma once
#include "PreDec.h"

namespace MocapGE
{
	class RenderElement
	{
	public:
		RenderElement(void);
		~RenderElement(void);

		virtual void AddToScene() = 0;
		virtual RenderLayout* GetRenderLayout() = 0;

		//virtual ShaderObject* GetShaderObject() = 0;

		virtual void Render() = 0;

	};

}

#endif

