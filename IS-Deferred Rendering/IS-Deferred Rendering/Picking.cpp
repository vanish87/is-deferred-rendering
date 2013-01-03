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
		VertexType* vertice = static_cast<VertexType*>(rb->Map(AT_CPU_WRITE));
		size_t vsize = rl->GetIndexCount();

		for(size_t j =0; j < vsize; j++)
		{			
			vertice[i].position = Math::Transform(vertice[i].position, mw_matrix);
		}
		AABBox* aabb = new AABBox(vertice, vsize);
		std::cout<<"aabb_max " <<aabb->Max().x() << " " << aabb->Max().y() << " " << aabb->Max().z()<<std::endl;
		std::cout<<"aabb_min " <<aabb->Min().x() << " " << aabb->Min().y() << " " << aabb->Min().z()<<std::endl;
// 		PRINT(" ");
// 		for(size_t j =0; j < vsize; j++)
// 		{			
// 			std::cout<< vertice[j].position.x()<< " " << vertice[j].position.y() << " " << vertice[j].position.z() <<std::endl;
// 		}

		if(Math::IntersectRayAABB(ray, aabb))
		{
			for(size_t j =0; j < vsize/3; j++)
	 		{
				if(Math::IntersectRayTriangle(ray, vertice[j].position, vertice[j+1].position, vertice[j+2].position))
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
