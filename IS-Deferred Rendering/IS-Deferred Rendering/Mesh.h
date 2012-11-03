#ifndef MESH_H_
#define MESH_H_

#pragma once

#include "PreDec.h"
#include "RenderElement.h"

namespace MocapGE
{
	class Mesh :public RenderElement
	{
	public:
		Mesh(void);
		~Mesh(void);

		Mesh(std::string name, RenderLayout* render_layout, float4x4 model_matrix)
			:RenderElement(model_matrix), name_(name), render_layout_(render_layout)
		{};

		virtual void AddToScene();

		virtual RenderLayout* GetRenderLayout();

		virtual void SetRenderParameters();

		virtual void Render();

		virtual void EndRender();

	protected:

		std::string name_;
		RenderLayout* render_layout_;
	};

}

#endif
