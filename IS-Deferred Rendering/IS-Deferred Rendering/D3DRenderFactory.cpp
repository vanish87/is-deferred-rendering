#include "D3DRenderFactory.h"

namespace MocapGE
{
	D3DRenderFactory::D3DRenderFactory(void)
		
	{
		render_engine_ = nullptr;
	}


	D3DRenderFactory::~D3DRenderFactory(void)
	{
	}

	RenderEngine* D3DRenderFactory::CreateRenderEngine()
	{
		return new D3DRenderEngine();
	}
	
	FrameBuffer* D3DRenderFactory::MakeFrameBuffer( RenderSetting& render_setting )
	{
		return new D3DFreamBuffer(render_setting);
	}

	RenderLayout* D3DRenderFactory::MakeRenderLayout()
	{
		return new D3DRenderLayout();
	}

	RenderBuffer* D3DRenderFactory::MakeRenderBuffer( InitData init_data, BufferUsage usage, uint32_t byte_width)
	{
		D3DRenderEngine* d3d_re = static_cast<D3DRenderEngine*>(&Context::Instance().GetRenderFactory().GetRenderEngine());
		switch (usage)
		{
		case BU_VERTEX:
			D3D11_BUFFER_DESC vertex_buffer_desc;
			vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
			vertex_buffer_desc.ByteWidth = byte_width;
			vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertex_buffer_desc.CPUAccessFlags = 0;
			vertex_buffer_desc.MiscFlags = 0;
			vertex_buffer_desc.StructureByteStride = 0;
			D3D11_SUBRESOURCE_DATA vertex_data;
			vertex_data.pSysMem = init_data.data;
			vertex_data.SysMemPitch = init_data.row_pitch;
			vertex_data.SysMemSlicePitch = init_data.slice_pitch;

			ID3D11Buffer* vertex_buffer;
			HRESULT result = d3d_re->D3DDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &vertex_buffer);
			if(FAILED(result))
			{
				PRINT("Cannot create Vertex Buffer");
			}
			delete[] init_data.data;

			D3DRenderBuffer* d3d_render_buffer = new D3DRenderBuffer(vertex_buffer, BU_VERTEX, AT_GPU_WRITE);
			return d3d_render_buffer;
			break;
		case BU_INDEX:
			D3D11_BUFFER_DESC index_buffer_desc;
			index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
			index_buffer_desc.ByteWidth = byte_width;
			index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			index_buffer_desc.CPUAccessFlags = 0;
			index_buffer_desc.MiscFlags = 0;
			index_buffer_desc.StructureByteStride = 0;
			D3D11_SUBRESOURCE_DATA index_data;
			index_data.pSysMem = init_data.data;
			index_data.SysMemPitch = init_data.row_pitch;
			index_data.SysMemSlicePitch = init_data.slice_pitch;

			ID3D11Buffer* index_buffer;
			HRESULT result = d3d_re->D3DDevice()->CreateBuffer(&index_buffer_desc, &index_data, &index_buffer);
			if(FAILED(result))
			{
				PRINT("Cannot create Index Buffer");
			}
			delete[] init_data.data;

			D3DRenderBuffer* d3d_render_buffer = new D3DRenderBuffer(index_buffer, BU_INDEX, AT_GPU_WRITE);
			return d3d_render_buffer;

			break;
		case BU_SHADER_RES:
			break;
		case BU_SHADER_CONST:
			break;
		default:
			break;
		}
		delete[] init_data.data;
	}

}