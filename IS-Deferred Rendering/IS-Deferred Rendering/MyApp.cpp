#include "MyApp.h"
#include "Context.h"
#include "Timer.h"

#include "StartMenu.h"
#include "LoadingState.h"
#include "Picking.h"

#include "D3DSkyDome.h"

using namespace MocapGE;
MyApp::MyApp(void) : App("The CentBebop Design")
{

}


MyApp::~MyApp(void)
{
}

void MyApp::InitObjects()
{

	theta = 0;
	ship_model = new D3DModel();
	ship_model->LoadFile("..\\Media\\spacecraft.dae");
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
	
	point_light = new PointLight();
	point_light->SetPos(float3(50 ,0, 0));	
	point_light->AddToScene();

	point_light_1 = new PointLight();
	point_light_1->SetPos(float3(0, 15, -15));
	point_light_1->AddToScene();


	timer_ = new Timer();
	timer_->Retart();
	
	start_menu_ = new StartMenu();
	Context::Instance().GetStateManager().ChangeState(start_menu_,MocapGE::SOP_PUSH);

	picking_ = new Picking();
	mouse_down_ = false;
	ship_pos = float3(0,0,0);

	speed_ = 1;

	//Example of changing Camera View
	float3 pos = float3(0,1,5);
	Camera* camera = GetCamera();
	camera->SetView(pos ,float3(0,0,0),float3(0,1,0));


	D3DSkyDome* sky = new D3DSkyDome("..\\Media\\sky.dds");
	sky->LoadShaderFile("..\\FxFiles\\SkyDome.fxo");
	SceneObject* sky_scene = new SceneObject(sky);
	sky_scene->AddToScene();

}

void MyApp::ReleaseObjects()
{
	delete ship_model;
	delete cannon_1;
	delete cannon_2;
}

void MyApp::Update()
{

	//camera->SetProjection(3.14f/4, 1280/800, 1, 100000);

	//Example of changing model position
	float4x4 model_matrix;
	float4x4 scale_matrix, xrotation_matrix, yrotation_matrix,zrotation_matrix,translate_matrix;
	Math::Identity(model_matrix);

	theta+= 0.001;
	float r = 0.6;
	float3 cannon_pos;
	
	//for all part
	//if(0)
	if(theta > 3.14/2)
	{
		Math::YRotation(zrotation_matrix, Math::PI/4*theta);
		Math::Translate(translate_matrix, Math::Cos(theta) ,0 , 0 );	
		//model_matrix= zrotation_matrix*  model_matrix;
		cannon_pos = float3(0, 0, 0);
	}
	else
	{
		cannon_pos = float3(0, 5*Math::Cos(theta), 0);
	}

	//for ship
	//Math::XRotation(xrotation_matrix, theta);
	Math::YRotation(yrotation_matrix, Math::PI/2 );	
	//ship_model->SetModelMatrix(yrotation_matrix* model_matrix);

	//for cannons
	Math::YRotation(yrotation_matrix, Math::PI+ Math::PI/6*Math::Sin(theta));
	Math::Scale(scale_matrix, 0.3);
	Math::Translate(translate_matrix, -3 + cannon_pos.x(), -0.5+ cannon_pos.y(), 3);//right
	cannon_1->SetModelMatrix(yrotation_matrix* translate_matrix*scale_matrix* model_matrix);
	Math::YRotation(yrotation_matrix, Math::PI+ Math::PI/6*Math::Cos(theta));
	Math::Translate(translate_matrix, 3 + cannon_pos.x(), -0.5 + cannon_pos.y(), 3);//left
	cannon_2->SetModelMatrix(yrotation_matrix* translate_matrix*scale_matrix* model_matrix);

	//point_light_1->SetPos(float3(55*Math::Cos(theta),55*Math::Sin(0),55*Math::Sin(theta)));
	//std::cout<<"light zpos"<< 5*Math::Sin(theta)<< std::endl;

	//PRINT(timer_->Time());
}

void MyApp::OnKeyDown( WPARAM key_para )
{
	switch (key_para) 
	{ 
		case 'P'://ascii P 
			
			ship_->SetVisiable(false);
			Context::Instance().GetStateManager().ChangeState(start_menu_, SOP_POP);
			delete start_menu_;
			start_menu_ = 0;
			loading_ = new LoadingState();
			Context::Instance().GetStateManager().ChangeState(loading_, SOP_PUSH);
			break;
		case 'A':
			{
				std::cout<<"A key down"<<std::endl;
				Camera* camera = GetCamera();
				float3 pos = camera->GetPos();
				float3 at = camera->GetLookAt();
				float3 dir =  at - pos;
				float3 up = camera->GetUp();
				float3 left = Math::Cross(dir,up);
				left = Math::Normalize(left);
				left = left * speed_;
				camera->SetView(pos + left, at, up);
				float4x4 m = ship_model->GetModelMatrix();
				Math::Translate(m, 1,0,0);
				//ship_model->SetModelMatrix(m);
				break;
			}
		case 'D':
			{
				std::cout<<"D key down"<<std::endl;
				Camera* camera = GetCamera();
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
		default:
			break;
	
	}
}

void MyApp::OnMouseDown( WPARAM mouse_para, int x, int y )
{
	//std::cout<<"mouse move "<<x<<" "<<y<<std::endl;
	float2 screen_pos(x, y);
	float3 picked_pos;
	Viewport* viewport = Context::Instance().GetRenderFactory().GetRenderEngine().CurrentFrameBuffer()->GetViewport();
	mouse_down_= true;
	picked = picking_->GetIntersection(ship_model, viewport, screen_pos, picked_pos);
}

void MyApp::OnMouseMove( WPARAM mouse_para, int x, int y )
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
			Camera* camera = GetCamera();
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
			
			//ship_pos = ship_pos + float3(delta.x()/320, delta.y()/200, 0) ;
			std::cout<<ship_pos.x()<<" "<<ship_pos.y()<<std::endl;
			Math::Translate(model_matrix, ship_pos.x(), ship_pos.y(), ship_pos.z());
			ship_model->SetModelMatrix(model_matrix);
		}
	
	}
	pre_pos = float2(x,y);
}

void MyApp::OnMouseUp( WPARAM mouse_para, int x, int y )
{
	mouse_down_= false;
	picked = false;
}

int main()
{
	Context::Instance().LoadConfig("E:\\12Fall\\Independent Study\\IS-Deferred Rendering\\IS-Deferred Rendering\\Config.xml");
	ContextConfig cfg = Context::Instance().GetConfigure();
	Context::Instance().Configure(cfg);

	MyApp app;
	app.Initialize();
	app.Run();

	return 0;
}

 