#include "MyApp.h"
#include "Context.h"
#include "Timer.h"

#include "StartMenu.h"
#include "GamingState.h"

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
	point_light = new PointLight();
	point_light->SetPos(float3(50 ,0, 0));	
	point_light->AddToScene();

	point_light_1 = new PointLight();
	point_light_1->SetPos(float3(0, 15, -15));
	point_light_1->AddToScene();

	timer_ = new Timer();
	timer_->Retart();
	
	//start_menu_ = new StartMenu();
	//Context::Instance().GetStateManager().ChangeState(start_menu_,MocapGE::SOP_PUSH);
	gaming_ = new GamingState();
	Context::Instance().GetStateManager().ChangeState(gaming_, SOP_PUSH);



	//Example of changing Camera View
	float3 pos = float3(0,1,5);
	Camera* camera = GetCamera();
	camera->SetView(pos ,float3(0,0,0),float3(0,1,0));
}

void MyApp::ReleaseObjects()
{
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
	//cannon_1->SetModelMatrix(yrotation_matrix* translate_matrix*scale_matrix* model_matrix);
	Math::YRotation(yrotation_matrix, Math::PI+ Math::PI/6*Math::Cos(theta));
	Math::Translate(translate_matrix, 3 + cannon_pos.x(), -0.5 + cannon_pos.y(), 3);//left
	//cannon_2->SetModelMatrix(yrotation_matrix* translate_matrix*scale_matrix* model_matrix);

	//point_light_1->SetPos(float3(55*Math::Cos(theta),55*Math::Sin(0),55*Math::Sin(theta)));
	//std::cout<<"light zpos"<< 5*Math::Sin(theta)<< std::endl;

	//PRINT(timer_->Time());
}

void MyApp::OnKeyDown( WPARAM key_para )
{
	switch (key_para) 
	{ 
		case VK_RETURN://ascii P 
			Context::Instance().GetStateManager().ChangeState(start_menu_, SOP_POP);
			delete start_menu_;
			start_menu_ = 0;
			gaming_ = new GamingState();
			Context::Instance().GetStateManager().ChangeState(gaming_, SOP_PUSH);
			break;
		default:
			break;
	
	}
}

void MyApp::OnMouseDown( WPARAM mouse_para, int x, int y )
{
}

void MyApp::OnMouseMove( WPARAM mouse_para, int x, int y )
{

}

void MyApp::OnMouseUp( WPARAM mouse_para, int x, int y )
{
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

 