#include "Model.h"
#include "Context.h"

namespace MocapGE
{
	Model::Model(void)
	{
	}


	Model::~Model(void)
	{
	}

	void Model::AddToScene()
	{
		Context::Instance().GetSceneManager().AddSceneObject(new SceneObject(this));
		for(size_t i = 0;i < meshes_.size(); i++)
		{
			meshes_[i]->AddToScene();
		}
	}

	RenderLayout* Model::GetRenderLayout()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void Model::Render()
	{
		//do nothing
		throw std::exception("The method or operation is not implemented.");
	}

	void Model::SetRenderParameters()
	{
		//only do the general setup
		throw std::exception("The method or operation is not implemented.");
	}

	void Model::EndRender()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void Model::LoadFile( std::string file_name )
	{

	}

	void Model::AddMesh( Mesh* mesh )
	{
		meshes_.push_back(mesh);
	}



}