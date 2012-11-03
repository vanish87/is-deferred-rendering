#include "Model.h"

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
		throw std::exception("The method or operation is not implemented.");
	}

	RenderLayout* Model::GetRenderLayout()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void Model::Render()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void Model::SetRenderParameters()
	{
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