#ifndef MESH_H_
#define MESH_H_

#pragma once

#include "PreDec.h"
#include "RenderElement.h"
#include "TypeAndFormat.h"

namespace MocapGE
{
	class Mesh :public RenderElement
	{
	public:
		Mesh(void);
		~Mesh(void);

		Mesh(std::string name, RenderLayout* render_layout, float4x4 model_matrix, VertexType* vb, uint32_t* ib)
			:RenderElement(model_matrix), name_(name), render_layout_(render_layout)
			,vb_(vb), ib_(ib)
		{};

		//virtual void AddToScene();

		virtual RenderLayout* GetRenderLayout();

		virtual void SetRenderParameters();

		virtual void Render(int pass_index);

		virtual void EndRender();

		VertexType* GetVertex(){return vb_;};
		uint32_t* GetIndex(){return ib_;};


	protected:

		std::string name_;
		RenderLayout* render_layout_;

		//stores vertex in cpu for obj management
		VertexType* vb_;
		uint32_t* ib_;
	};

}

#endif
