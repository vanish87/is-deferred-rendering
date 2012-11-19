#include "DeferredRendering.h"

namespace MocapGE
{
	DeferredRendering::DeferredRendering( RenderSetting& render_setting )
	{
		gbuffer_ = Context::Instance().GetRenderFactory().MakeFrameBuffer(render_setting);
		lighting_buffer_ = Context::Instance().GetRenderFactory().MakeFrameBuffer(render_setting);
		//make a full screen qua for lighting pass
		VertexType* vb = new VertexType[6];
		uint32_t* ib = new uint32_t[6];
		vb[0].position = float3(-1, +1, 1);
		vb[1].position = float3(+1, +1, 1);
		vb[2].position = float3(-1, -1, 1);
		vb[3].position = float3(+1, -1, 1);
		vb[4].position = float3(-1, -1, 1);
		vb[5].position = float3(+1, +1, 1);

		ib[0] = 0;
		ib[1] = 1;
		ib[2] = 2;
		ib[3] = 3;
		ib[4] = 4;
		ib[5] = 5;

		//call MakeRenderLayout
		RenderLayout* render_layout = Context::Instance().GetRenderFactory().MakeRenderLayout();
		//call MakeRenderBuffer(Vertex)
		InitData init_data;
		init_data.data = vb;
		init_data.row_pitch = 0;
		init_data.slice_pitch = 0;
		RenderBuffer* vertex_buffer = Context::Instance().GetRenderFactory().MakeRenderBuffer(init_data, AT_GPU_READ, BU_VERTEX, 6 ,sizeof(VertexType));
		//delete[] vb;
		//call MakeRenderBuffer(Index)
		init_data.data = ib;
		init_data.row_pitch = 0;
		init_data.slice_pitch = 0;
		RenderBuffer* index_buffer = Context::Instance().GetRenderFactory().MakeRenderBuffer(init_data, AT_GPU_READ, BU_INDEX, 6, sizeof(uint32_t));
		//delete[] ib;

		//add VertexBuffer to renderlayout;
		render_layout->AddBuffer(vertex_buffer, sizeof(VertexType));
		//add IndexBuffer to renderlayout;
		render_layout->AddBuffer(index_buffer, 6);
		//set Primitivetype of renderlayout;
		render_layout->SetPrimitive(PT_TRIANGLELIST);
		//set Input layout Semi
		std::vector<VertexUsage> inputlayout;
		inputlayout.push_back(VU_POSITION);
		render_layout->SetInputLayout(inputlayout);
		float4x4 model_matrix;
		Math::Identity(model_matrix);
		fullscreen_mesh_ = new MocapGE::Mesh("full screen quad", render_layout, model_matrix);

	}


	DeferredRendering::~DeferredRendering(void)
	{
	}

	void DeferredRendering::AddGBuffer( RenderView* render_target_view )
	{
		gbuffer_->AddRenderView(render_target_view);
	}

	void DeferredRendering::AddGBuffer( RenderBuffer* shader_resource_view )
	{
		gbuffer_srv_.push_back(shader_resource_view);
	}

	Mesh* DeferredRendering::GetFullscreenQuad()
	{
		return fullscreen_mesh_;
	}

	void DeferredRendering::AddLightingBuffer( RenderView* render_view )
	{
		lighting_buffer_->AddRenderView(render_view);
	}

	void DeferredRendering::AddLightingBuffer( RenderBuffer* shader_resource)
	{
		lighting_srv_ = shader_resource;
	}


}
