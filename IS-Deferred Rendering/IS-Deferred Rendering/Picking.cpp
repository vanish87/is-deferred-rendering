#include "Picking.h"
#include "Context.h"
#include "Ray.h"
#include "AABBox.h"

using namespace MocapGE;
Picking::Picking(void)
{
}


Picking::~Picking(void)
{
}

bool Picking::GetIntersection( D3DModel* model, Viewport* viewport, float2 screen_point, float3& intersected_point )
{

	Camera* camera = viewport->GetCamera();
	float4x4 proj_matrix = camera->GetProjMatrix();
	float4x4 view_matrix = camera->GetViewMatirx();

	float4x4 world_matrix = model->GetModelMatrix();
	float4x4 invs_view_matrix = Math::Inverse(view_matrix);

	float4x4 invs_trans_view_matrix = Math::InverTranspose(view_matrix);

	float px = ((( 2.0f * screen_point.x()) / viewport->Width())  - 1.0f) / proj_matrix(0, 0);
	float py = (((-2.0f * screen_point.y()) / viewport->Height()) + 1.0f) / proj_matrix(1, 1);

	float3 pos = float3(0.0f, 0.0f, 0.0f);
	float3 dir = float3(px, py, 1.0f);

	pos = Math::Transform(pos, invs_view_matrix);
	dir = Math::Transform(dir, invs_trans_view_matrix);

	Math::Normalize(dir);

	Ray* ray = new Ray(pos, dir);
	std::cout<<"scr_pos " <<screen_point.x() << " " << screen_point.y()<<std::endl;
	std::cout<<"ray_pos " <<pos.x() << " " << pos.y() << " " << pos.z()<<std::endl;
	std::cout<<"ray_dir " <<dir.x() << " " << dir.y() << " " <<  dir.z()<<std::endl;

	std::vector<Mesh*> meshes_ = model->GetMesh();
	for(size_t i =0; i < meshes_.size() ; i++)
	{
		float4x4 model_matrix = meshes_[i]->GetModelMatrix();
		float4x4 mw_matrix = Math::Multiply(model_matrix, world_matrix);
		RenderLayout* rl = meshes_[i]->GetRenderLayout();
		RenderBuffer* rb = rl->GetBuffer(VBU_VERTEX);
		VertexType* vertice_gpu = static_cast<VertexType*>(rb->Map(AT_CPU_WRITE));
		
		size_t vsize = rl->GetIndexCount();

		for(size_t j =0; j < vsize; j++)
		{			
			vertice_cpu.push_back(new VertexType());
			vertice_cpu[j]->position = Math::Transform(vertice_gpu[j].position, mw_matrix);
		}
		AABBox* aabb = new AABBox(vertice_cpu, vsize);
		std::cout<<"aabb_max " <<aabb->Max().x() << " " << aabb->Max().y() << " " << aabb->Max().z()<<std::endl;
		std::cout<<"aabb_min " <<aabb->Min().x() << " " << aabb->Min().y() << " " << aabb->Min().z()<<std::endl;

		/*VertexType* vb = new VertexType[6];
		int* ib = new int[6];
		vb[0].position = aabb->Min();
		vb[1].position = float3(aabb->Min().x(), aabb->Min().y(), aabb->Max().z());
		vb[2].position = aabb->Max();
		vb[3].position = aabb->Min();
		vb[4].position = aabb->Max();
		vb[5].position = float3(aabb->Max().x(), aabb->Max().y(), aabb->Min().z());

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
		RenderBuffer* index_buffer = Context::Instance().GetRenderFactory().MakeRenderBuffer(init_data, AT_GPU_READ, BU_INDEX, 6, sizeof(int));
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
		float4x4 model_matrix_;
		Math::Identity(model_matrix_);
		Mesh* aabb_mesh = new Mesh("full screen quad", render_layout, model_matrix_);
		aabb_mesh->SetShaderObject(model->GetShaderObject());

		SceneObject* aabb_obj = new SceneObject(aabb_mesh);
		aabb_obj->AddToScene();
*/

//  		PRINT(" ");
// 		for(size_t j =0; j < vsize; j++)
//  		{			
//  			std::cout<< vertice[j].position.x()<< " " << vertice[j].position.y() << " " << vertice[j].position.z() <<std::endl;
//  		}

		if(Math::IntersectRayAABB(ray, aabb))
		{
			
			for(size_t j =0; j < vsize/3; j++)
	 		{
				if(Math::IntersectRayTriangle(ray, vertice_cpu[j]->position, vertice_cpu[j+1]->position, vertice_cpu[j+2]->position))
				{

					PRINT("clicked");
					
				}
			}
		}
		
		rb->UnMap();
	}
	// 	pos = pos * view_matrix;
	// 	Ray* ray = new Ray();
	// 	ray->SetOrigin();
	// 	ray->SetOrigin();


	//Context::Instance().GetRenderFactory().GetRenderEngine()
	return false;
}
