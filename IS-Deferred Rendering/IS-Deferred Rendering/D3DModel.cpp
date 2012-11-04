#include "D3DModel.h"

namespace MocapGE
{
	D3DModel::D3DModel(void)
	{
	}


	D3DModel::~D3DModel(void)
	{
	}

	void D3DModel::LoadFile( std::string file_name )
	{
		Model::LoadFile(file_name);
		//do Load texture here
	}

	void D3DModel::SetRenderParameters()
	{

	}

}
