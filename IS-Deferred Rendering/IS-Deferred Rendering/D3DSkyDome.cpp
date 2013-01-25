#include "D3DSkyDome.h"

#include "D3DRenderEngine.h"
#include "D3DTexture.h"

namespace MocapGE
{

	D3DSkyDome::D3DSkyDome(void)
	{
	}


	D3DSkyDome::~D3DSkyDome(void)
	{
	}

	Texture* D3DSkyDome::LoadTexture( std::string file_name )
	{

		//only for load d3d Texture
		//if I have a original texture file loader, remove it, do Texture loading on Model Class
		D3DRenderEngine* d3d_re = static_cast<D3DRenderEngine*>(&Context::Instance().GetRenderFactory().GetRenderEngine());	
		ID3D11Resource* texture;
		std::wstring widestr = std::wstring(file_name.begin(), file_name.end());
		HRESULT result = DirectX::CreateDDSTextureFromFile(d3d_re->D3DDevice(), widestr.c_str(), &texture, NULL);
		if(FAILED(result))
			PRINT("Cannot Load Texture File");
		ID3D11Texture2D* texture_2d= static_cast<ID3D11Texture2D*>(texture);
		D3D11_TEXTURE2D_DESC desc;
		texture_2d->GetDesc(&desc);
		//TODO: use unified routine to Create Texture
		D3DTexture2D* d3d_tex = new D3DTexture2D(desc,texture_2d);

		return d3d_tex;
	}

}
