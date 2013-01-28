#include "GamingState.h"
#include "D3DSkyDome.h"

using namespace MocapGE;
GamingState::GamingState(void)
{
	ship_model = new D3DModel();
	ship_model->LoadFile("..\\Media\\spacecraft_new.dae");
	ship_model->LoadShaderFile("..\\FxFiles\\DeferredLighting.fxo");
	scene_ship_ = new SceneObject(ship_model);
	scene_ship_->AddToScene();

	ship_ = new Ship(ship_model);

	float3 ship_dir = ship_->GetDir();
	float3 ship_pos = ship_->GetPos();
	float3 ship_up = ship_->GetUp();

	Camera* camera = Context::Instance().AppInstance().GetCamera();

	float3 cam_pos = ship_pos - ship_dir * 3 + ship_up;
	float3 cam_at = ship_pos + ship_dir * 2;
	camera->SetView(cam_pos, cam_at, ship_up);

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


	//speed_ = 1;
	//mouse_down_ = false;
	spacekey_down_ = false;
	wkey_down_ = false;
	skey_down_ = false;
	ship_pos = float3(0,0,0);


	//picking_ = new Picking();

	D3DModel* ground = new D3DModel();
	ground->LoadFile("..\\Media\\ground.dae");
	ground->LoadShaderFile("..\\FxFiles\\DeferredLighting.fxo");
	float4x4 scale_mat;
	Math::Scale(scale_mat, 100);
	ground->SetModelMatrix(scale_mat);
	SceneObject* scene_ground = new SceneObject(ground);
	scene_ground->AddToScene();
}


GamingState::~GamingState(void)
{
	delete ship_model;
	delete cannon_1;
	delete cannon_2;
}

void GamingState::Update()
{
	if(!spacekey_down_)
		ship_->Deccelerating();
 	if(!skey_down_)
 		ship_->Stabling(false);
 	if(!wkey_down_)
 		ship_->Stabling(true);

	ship_->Update();
	float3 ship_dir = ship_->GetDir();
	float3 ship_pos = ship_->GetPos();
	float3 ship_up = ship_->GetUp();

	//std::cout<<"dir= "<<ship_dir.x()<<" "<<ship_dir.y()<<" "<<ship_dir.z()<<" "<<std::endl;
	float3 cam_tag_pos = ship_pos - ship_dir * 3 + ship_up;
	Camera* camera = Context::Instance().AppInstance().GetCamera();
	float3 cam_pos = camera->GetPos();
	float3 cam_up = camera->GetUp();

	float3 dis = cam_tag_pos - cam_pos;
	//if(Math::Dot(dis, dis) > 0.0001)
	//{
	//	dis = Math::Normalize(dis);
	//	camera->SetView(cam_pos + dis* 0.005, ship_pos + ship_dir* 2, cam_up);
	//}
	//else
	{
		camera->SetView(cam_tag_pos, ship_pos + ship_dir* 2, ship_up);
	}
}

void GamingState::OnKeyDown( WPARAM key_para )
{
	switch (key_para) 
	{ 
	case 'A':
		{
			//std::cout<<"A key down"<<std::endl;
			ship_->TurnLeft();
// 			Camera* camera = Context::Instance().AppInstance().GetCamera();
// 			float3 pos = camera->GetPos();
// 			float3 at = camera->GetLookAt();
// 			float3 dir =  at - pos;
// 			float3 up = camera->GetUp();
// 			float3 left = Math::Cross(dir,up);
// 			left = Math::Normalize(left);
// 			left = left * speed_;
// 			camera->SetView(pos + left, at, up);
			//float4x4 m = ship_model->GetModelMatrix();
			//Math::Translate(m, 1,0,0);
			//ship_model->SetModelMatrix(m);
			break;
		}
	case 'D':
		{
			//std::cout<<"D key down"<<std::endl;
			ship_->TurnRight();
// 			Camera* camera = Context::Instance().AppInstance().GetCamera();
// 			float3 pos = camera->GetPos();
// 			float3 at = camera->GetLookAt();
// 			float3 dir =  at - pos;
// 			float3 up = camera->GetUp();
// 			float3 left = Math::Cross(dir,up);
// 			left = Math::Normalize(left);
// 			left = left * speed_;
// 
// 			float4x4 m = ship_model->GetModelMatrix();
// 			Math::Translate(m, -1,0,0);
// 			//ship_model->SetModelMatrix(m);
// 			camera->SetView(pos - left, at, up);
			break;
		}
	case 'S':
		{
			std::cout<<"s key down"<<std::endl;
			skey_down_ = true;
			ship_->HeadUp();
			break;
		}
	case 'W':
		{
			spacekey_down_ = true;
			ship_->HeadDown();
			break;
		}

	case VK_SPACE:
		{
			spacekey_down_ = true;
			ship_->Accelerating();
			break;
		}

	case 'R':
		{
			break;
		}
	}
}

void GamingState::OnKeyUp(WPARAM key_para)
{
	switch (key_para) 
	{ 
	case 'S':
		{
			std::cout<<"s key up"<<std::endl;
			skey_down_ = false;
			break;
		}
	case 'W':
		{
			spacekey_down_ = false;
			break;
		}
	case VK_SPACE:
		{
			spacekey_down_ = false;
			break;
		}
	}
}

void GamingState::OnMouseDown( WPARAM mouse_para, int x, int y )
{
	//std::cout<<"mouse move "<<x<<" "<<y<<std::endl;
	
}

void GamingState::OnMouseUp( WPARAM mouse_para, int x, int y )
{

}

void GamingState::OnMouseMove( WPARAM mouse_para, int x, int y )
{
	
}
