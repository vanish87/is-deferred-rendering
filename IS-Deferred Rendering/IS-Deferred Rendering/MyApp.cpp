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
	point_light_ = new PointLight();
	point_light_->SetPos(float3(50 ,0, 0));	
	//point_light_->AddToScene();

	spot_light_ = new SpotLight();
	spot_light_->SetPos(float3(50, 10, 0));
	spot_light_->SetDir(float3(0,0,0)-spot_light_->GetPos());
	spot_light_->SetInnerAngle(Math::PI / 24);
	spot_light_->SetOuterAngle(Math::PI / 14);
	spot_light_->AddToScene();

	timer_ = new Timer();
	timer_->Retart();
	
	//set current state to gaming
	gaming_ = new GamingState();
	Context::Instance().GetStateManager().ChangeState(gaming_, SOP_PUSH);
}

void MyApp::ReleaseObjects()
{
	delete point_light_;
	delete spot_light_;
	delete timer_;
	//delete start_menu_; 
	delete gaming_;
	delete resembling_;
}

void MyApp::Update()
{
	//std::cout<<timer_->Time()<<"\r";

	//std::cout<<spot_light_->GetPos().x()<<"\r";
	//spot_light_->SetDir(float3(-0.5,-1,Math::Cos(timer_->Time()/1000.0f) * Math::PI / 6));
}

void MyApp::OnKeyDown( WPARAM key_para )
{

	float3 pos = spot_light_->GetPos();
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

		case 'O':
			pos.y()--;
			spot_light_->SetPos(pos);
			spot_light_->SetDir(float3(0,0,0)-spot_light_->GetPos());
			break;
		case 'P':
			pos.y()++;
			spot_light_->SetPos(pos);
			spot_light_->SetDir(float3(0,0,0)-spot_light_->GetPos());
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
	 _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	Context::Instance().LoadConfig("E:\\12Fall\\Independent Study\\IS-Deferred Rendering\\IS-Deferred Rendering\\Config.xml");
	ContextConfig cfg = Context::Instance().GetConfigure();
	Context::Instance().Configure(cfg);

	MyApp app;
	app.Initialize();
	app.Run();

	_CrtDumpMemoryLeaks();
	return 0;
}

 