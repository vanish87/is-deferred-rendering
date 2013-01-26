#include "GamingState.h"
#include "D3DSkyDome.h"

using namespace MocapGE;
GamingState::GamingState(void)
{
	ship_model = new D3DModel();
	ship_model->LoadFile("..\\Media\\heart1.dae");
	ship_model->LoadShaderFile("..\\FxFiles\\DeferredLighting.fxo");
	ship_ = new SceneObject(ship_model);
	ship_->AddToScene();

	cannon_1 = new D3DModel();
	//cannon_1->LoadFile("..\\Media\\gun.dae");
	//cannon_1->LoadShaderFile("..\\FxFiles\\DeferredLighting.fxo");
	//cannon_1->AddToScene();

	cannon_2 = new D3DModel();
	//cannon_2->LoadFile("..\\Media\\gun.dae");
	//cannon_2->LoadShaderFile("..\\FxFiles\\DeferredLighting.fxo");
	//cannon_2->AddToScene();

	sky_= new D3DSkyDome("..\\Media\\universe.dds");
	sky_->LoadShaderFile("..\\FxFiles\\SkyDome.fxo");
	SceneObject* sky_scene = new SceneObject(sky_);
	sky_scene->AddToScene();


	speed_ = 1;
	mouse_down_ = false;
	ship_pos = float3(0,0,0);


	picking_ = new Picking();
}


GamingState::~GamingState(void)
{
	delete ship_model;
	delete cannon_1;
	delete cannon_2;
}

void GamingState::Update()
{

}

void GamingState::OnKeyDown( WPARAM key_para )
{
	switch (key_para) 
	{ 
	case 'A':
		{
			std::cout<<"A key down"<<std::endl;
			Camera* camera = Context::Instance().AppInstance().GetCamera();
			float3 pos = camera->GetPos();
			float3 at = camera->GetLookAt();
			float3 dir =  at - pos;
			float3 up = camera->GetUp();
			float3 left = Math::Cross(dir,up);
			left = Math::Normalize(left);
			left = left * speed_;
			camera->SetView(pos + left, at, up);
			//float4x4 m = ship_model->GetModelMatrix();
			//Math::Translate(m, 1,0,0);
			//ship_model->SetModelMatrix(m);
			break;
		}
	case 'D':
		{
			std::cout<<"D key down"<<std::endl;
			Camera* camera = Context::Instance().AppInstance().GetCamera();
			float3 pos = camera->GetPos();
			float3 at = camera->GetLookAt();
			float3 dir =  at - pos;
			float3 up = camera->GetUp();
			float3 left = Math::Cross(dir,up);
			left = Math::Normalize(left);
			left = left * speed_;

			float4x4 m = ship_model->GetModelMatrix();
			Math::Translate(m, -1,0,0);
			//ship_model->SetModelMatrix(m);
			camera->SetView(pos - left, at, up);
			break;
		}
	case 'S':
		break;
	case 'W':
		break;
	}
}

void GamingState::OnMouseDown( WPARAM mouse_para, int x, int y )
{
	//std::cout<<"mouse move "<<x<<" "<<y<<std::endl;
	float2 screen_pos(x, y);
	float3 picked_pos;
	Viewport* viewport = Context::Instance().GetRenderFactory().GetRenderEngine().CurrentFrameBuffer()->GetViewport();
	mouse_down_= true;
	picked = picking_->GetIntersection(ship_model, viewport, screen_pos, picked_pos);
}

void GamingState::OnMouseUp( WPARAM mouse_para, int x, int y )
{

	mouse_down_= false;
	picked = false;
}

void GamingState::OnMouseMove( WPARAM mouse_para, int x, int y )
{
	if(mouse_down_)
	{
		float2 screen_pos(x, y);
		//std::cout<<screen_pos.x()<<" "<<screen_pos.y()<<std::endl;
		//std::cout<<pre_pos.x()<<" "<<pre_pos.y()<<std::endl;
		float3 picked_pos;
		Viewport* viewport = Context::Instance().GetRenderFactory().GetRenderEngine().CurrentFrameBuffer()->GetViewport();
		if(picked)
		{
			float4x4 model_matrix = ship_model->GetModelMatrix();
			float2 delta = screen_pos - pre_pos;
			std::cout<<delta.x()<<"d "<<delta.y()<<std::endl;
			Camera* camera = Context::Instance().AppInstance().GetCamera();
			float3 pos = camera->GetPos();
			float3 at = camera->GetLookAt();
			float3 dir =  at - pos;
			float3 up = camera->GetUp();
			float3 left = Math::Cross(dir,up);
			left = Math::Normalize(left);
			float3 right = float3(-left.x(), -left.y(), -left.z());
			float3 real_up = Math::Cross(dir, right);
			real_up = Math::Normalize(real_up);
			float3 down = float3(-real_up.x(), -real_up.y(), -real_up.z());
			if(delta.x() > 0)
				ship_pos = ship_pos + right / 50;
			else
				if(delta.x() < 0)
					ship_pos = ship_pos + left / 50;
			if(delta.y() > 0 )
				ship_pos = ship_pos + down/ 50;
			else
				if(delta.y() < 0 )
					ship_pos = ship_pos + real_up/ 50;

			ship_pos = ship_pos + float3(delta.x()/320, delta.y()/200, 0) ;
			std::cout<<ship_pos.x()<<" "<<ship_pos.y()<<std::endl;
			Math::Translate(model_matrix, ship_pos.x(), ship_pos.y(), ship_pos.z());
			ship_model->SetModelMatrix(model_matrix);
		}

	}
	pre_pos = float2(x,y);
}
