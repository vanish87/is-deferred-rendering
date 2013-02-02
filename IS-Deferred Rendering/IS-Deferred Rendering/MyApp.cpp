#include "MyApp.h"
#include "Context.h"
#include "Timer.h"

#include "StartMenu.h"
#include "GamingState.h"

#include "Picking.h"

#include "D3DSkyDome.h"
#include "ResembleState.h"

using namespace MocapGE;
MyApp::MyApp(void) : App("The CentBebop Design")
	,start_menu_(nullptr), gaming_(nullptr), resembling_(nullptr)
{

}


MyApp::~MyApp(void)
{
}

void MyApp::InitObjects()
{
	//set up lights
	point_light = new PointLight();
	point_light->SetPos(float3(50 ,0, 0));	
	point_light->AddToScene();

	point_light_1 = new PointLight();
	point_light_1->SetPos(float3(0, 15, -15));
	point_light_1->AddToScene();

	timer_ = new Timer();
	timer_->Retart();
	
	//set current state to gaming
	gaming_ = new GamingState();
	Context::Instance().GetStateManager().ChangeState(gaming_, SOP_PUSH);
}

void MyApp::ReleaseObjects()
{
	delete point_light;
	delete point_light_1;
	delete timer_;
	//delete start_menu_;
	delete gaming_;
	delete resembling_;
}

void MyApp::Update()
{
}

void MyApp::OnKeyDown( WPARAM key_para )
{
	switch (key_para) 
	{ 
		//entering assembling mode
		case 'R':
			{
				if(Context::Instance().GetStateManager().CurrentState() == gaming_)
				{
					std::cout<<"Assemble"<<std::endl;
					PartList parts = gaming_->GetParts();
					if(resembling_ == nullptr)
						resembling_ = new ResembleState(gaming_->GetShip(), parts);
					resembling_->Init(parts);					
					Context::Instance().GetStateManager().ChangeState(resembling_, SOP_PUSH);
					break;
				}
				
				if(Context::Instance().GetStateManager().CurrentState() == resembling_)
				{
					std::cout<<"Gaming"<<std::endl;
					Context::Instance().GetStateManager().ChangeState(resembling_, SOP_POP);
					break;
				}
				break;
			}
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

 