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

		virtual void AddToScene();

		virtual RenderLayout* GetRenderLayout();

		virtual void SetRenderParameters();

		virtual void Render();

		virtual void EndRender();

	protected:

		RenderLayout* render_layout_;
	};

}

#endif
