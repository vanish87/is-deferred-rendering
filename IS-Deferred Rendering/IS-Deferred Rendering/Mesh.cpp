#include "Mesh.h"
#include "Context.h"

namespace MocapGE
{
	Mesh::Mesh(void)
	{
	}


	Mesh::~Mesh(void)
	{
	}

	void Mesh::AddToScene()
	{
		Context::Instance().GetSceneManager().AddSceneObject(new SceneObject(this));
	}

	RenderLayout* Mesh::GetRenderLayout()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void Mesh::SetRenderParameters()
	{
		shader_object_->SetMatrixVariable("g_model_matrix", model_matrix_);
	}

	void Mesh::Render()
	{
		RenderEngine& render_engine = Context::Instance().GetRenderFactory().GetRenderEngine();
		render_engine.Render(render_layout_, shader_object_);
	}

	void Mesh::EndRender()
	{
		//throw std::exception("The method or operation is not implemented.");
	}


}
