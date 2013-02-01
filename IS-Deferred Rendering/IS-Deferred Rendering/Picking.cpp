#include "Picking.h"
#include "Context.h"
#include "Ray.h"
#include "AABBox.h"

#include <limits>

using namespace MocapGE;
Picking::Picking(void)
	:model_(nullptr)
{
}

Picking::~Picking(void)
{
}

bool Picking::GetIntersection( D3DModel* model, Viewport* viewport, float2 screen_point, float3& intersected_point )
{
	if(model_ == nullptr)
	{
		model_ = model;
		std::vector<Mesh*> meshes_ = model_->GetMesh();
		for(size_t i =0; i < meshes_.size() ; i++)
		{
			float4x4 model_matrix = meshes_[i]->GetModelMatrix();
			RenderLayout* rl = meshes_[i]->GetRenderLayout();
			//RenderBuffer* rb = rl->GetBuffer(VBU_VERTEX);
			VertexType* vertice_gpu = meshes_[i]->GetVertex();

			std::vector<VertexType*> new_mesh;
			vertice_cpu_.push_back(new_mesh);
			size_t vsize = rl->GetIndexCount();

			for(size_t j =0; j < vsize; j++)
			{			
				vertice_cpu_[i].push_back(new VertexType());
				vertice_cpu_[i][j]->position = Math::Transform(vertice_gpu[j].position, model_matrix);
				//std::cout<< vertice_gpu[j].position.x()<< " " << vertice_gpu[j].position.y() << " " << vertice_gpu[j].position.z() <<std::endl;
			}
			AABBox* aabb = new AABBox(vertice_cpu_[i], vsize);
			aabbs_.push_back(aabb);
			//std::cout<<"aabbs"<<aabbs_.size()<<std::endl;
			//rb->UnMap();
		}
	}
	//make all points in camera space
	Camera* camera = viewport->GetCamera();
	float4x4 proj_matrix = camera->GetProjMatrix();
	float4x4 view_matrix = camera->GetViewMatirx();

	float4x4 world_matrix = model->GetModelMatrix();

	float px = ((( 2.0f * screen_point.x()) / viewport->Width())  - 1.0f) / proj_matrix(0, 0);
	float py = (((-2.0f * screen_point.y()) / viewport->Height()) + 1.0f) / proj_matrix(1, 1);

	float4 pos = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 dir = float4(px, py, 1.0f, 0.0f);

	//pos = Math::Transform(pos, invs_view_matrix);
	//dir = Math::Transform(dir, invs_trans_view_matrix);
	//dir.x() = dir.x() / dir.w();
	//dir.y() = dir.y() / dir.w();
	//dir.z() = dir.z() / dir.w();
	
	float3 dir_ = Math::Normalize(float3(dir.x(), dir.y(), dir.z()));

	Ray* ray = new Ray(float3(pos.x(), pos.y(), pos.z()), float3(dir_.x(), dir_.y(), dir_.z()));
/*
	std::cout<<"scr_pos " <<screen_point.x() << " " << screen_point.y()<<std::endl;
	std::cout<<"ray_pos " <<pos.x() << " " << pos.y() << " " << pos.z()<<std::endl;
	std::cout<<"ray_dir " <<dir_.x() << " " << dir_.y() << " " <<  dir_.z()<<std::endl;*/

	for(int i =0; i < aabbs_.size(); i++)
	{
		
		float4x4 wv_matrix =  world_matrix * view_matrix;
		std::vector<VertexType*> mesh_vertice;
		for(int j = 0; j< vertice_cpu_[i].size(); j++)
		{
			mesh_vertice.push_back(new VertexType());
			mesh_vertice[j]->position = Math::Transform(vertice_cpu_[i][j]->position, wv_matrix);
		}
		AABBox* aabb = new AABBox(mesh_vertice, mesh_vertice.size());
/*
		std::cout<<"aabb_max " <<aabb->Max().x() << " " << aabb->Max().y() << " " << aabb->Max().z()<<std::endl;
		std::cout<<"aabb_min " <<aabb->Min().x() << " " << aabb->Min().y() << " " << aabb->Min().z()<<std::endl;

*/

		/*VertexType* vb = new VertexType[2];
		float3 n = float3(0,1,0);
		float2 t = float2(0,0);
		int* ib = new int[2];
		vb[0].position = Math::Transform( ray->Origin() , invs_view_matrix);
		vb[0].normal = n;
		vb[0].uv = t;
		vb[1].position = Math::Transform( ray->Direction()  * 2, invs_view_matrix);
		vb[1].normal = n;
		vb[1].uv = t;
		/ *vb[2].position =  ray->Origin() + float3(0.2, 0.2, 0.2);
		vb[2].normal = n;
		vb[2].uv = t;
/ *
		vb[0].position = aabb->Min();
		vb[0].normal = n;
		vb[0].uv = t;
		vb[1].position = float3(aabb->Min().x(), aabb->Min().y(), aabb->Max().z());
		vb[1].normal = n;
		vb[1].uv = t;
		vb[2].position = aabb->Max();
		vb[2].normal = n;
		vb[2].uv = t;* /
		vb[3].position = aabb->Min();
		vb[3].normal = n;
		vb[3].uv = t;
		vb[4].position = aabb->Max();
		vb[4].normal = n;
		vb[4].uv = t;
		vb[5].position = float3(aabb->Max().x(), aabb->Max().y(), aabb->Min().z());
		vb[5].normal = n;
		vb[5].uv = t;* /

		ib[0] = 0;
		ib[1] = 1;
		/ *ib[2] = 2;
		ib[3] = 3;
		ib[4] = 4;
		ib[5] = 5;* /

		//call MakeRenderLayout
		RenderLayout* render_layout = Context::Instance().GetRenderFactory().MakeRenderLayout();
		//call MakeRenderBuffer(Vertex)
		InitData init_data;
		init_data.data = vb;
		init_data.row_pitch = 0;
		init_data.slice_pitch = 0;
		RenderBuffer* vertex_buffer = Context::Instance().GetRenderFactory().MakeRenderBuffer(init_data, AT_GPU_READ, BU_VERTEX, 2 ,sizeof(VertexType));
		//delete[] vb;
		//call MakeRenderBuffer(Index)
		init_data.data = ib;
		init_data.row_pitch = 0;
		init_data.slice_pitch = 0;
		RenderBuffer* index_buffer = Context::Instance().GetRenderFactory().MakeRenderBuffer(init_data, AT_GPU_READ, BU_INDEX, 2, sizeof(int));
		//delete[] ib;

		//add VertexBuffer to renderlayout;
		render_layout->AddBuffer(vertex_buffer, sizeof(VertexType));
		//add IndexBuffer to renderlayout;
		render_layout->AddBuffer(index_buffer, 2);
		//set Primitivetype of renderlayout;
		render_layout->SetPrimitive(PT_LINELIST);
		//set Input layout Semi
		std::vector<VertexUsage> inputlayout;
		inputlayout.push_back(VU_POSITION);
		inputlayout.push_back(VU_NORMAL);
		inputlayout.push_back(VU_TEXCOORD);
		render_layout->SetInputLayout(inputlayout);
		float4x4 model_matrix_;
		Math::Identity(model_matrix_);
		Mesh* aabb_mesh = new Mesh("full screen quad", render_layout, model_matrix_);
		aabb_mesh->SetShaderObject(model->GetShaderObject());

		SceneObject* aabb_obj = new SceneObject(aabb_mesh);
		aabb_obj->AddToScene();*/


//  		PRINT(" ");
// 		for(size_t j =0; j < vsize; j++)
//  		{			
//  			std::cout<< vertice[j].position.x()<< " " << vertice[j].position.y() << " " << vertice[j].position.z() <<std::endl;
//  		}
		float t_min = std::numeric_limits<float>::max();
		float t = 0;
		if(Math::IntersectRayAABB(ray, aabb))
		{
			//PRINT("AABB clicked");
			for(size_t j =0; j < mesh_vertice.size() -3; j++)
	 		{

				if(Math::IntersectRayTriangle(ray, mesh_vertice[j]->position, mesh_vertice[j+1]->position, mesh_vertice[j+2]->position, t))
				{
					if(t < t_min)
						t_min = t;
					//PRINT("clicked");
				}
			}
			if(t_min < std::numeric_limits<float>::max())
			{
				PRINT("clicked");
				PRINT(t_min);
				intersected_point = ray->Origin()+ ray->Direction() * t_min;
				float4x4 inv_view_mat = Math::Inverse(view_matrix);
				intersected_point = Math::Transform(intersected_point, inv_view_mat);
				//std::cout<<intersected_point.x()<<" "<<intersected_point.y()<<" "<<intersected_point.z()<<std::endl;
				return true;
			}
		}
		return false;
		
	}
	// 	pos = pos * view_matrix;
	// 	Ray* ray = new Ray();
	// 	ray->SetOrigin();
	// 	ray->SetOrigin();


	//Context::Instance().GetRenderFactory().GetRenderEngine()
	return false;
}
