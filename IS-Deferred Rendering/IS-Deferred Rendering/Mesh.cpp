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
		throw std::exception("The method or operation is not implemented.");
	}

	void Mesh::Render()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void Mesh::EndRender()
	{
		throw std::exception("The method or operation is not implemented.");
	}

}
