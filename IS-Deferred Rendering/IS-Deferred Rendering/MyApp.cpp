#include "MyApp.h"
#include "Context.h"

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
	ship_ = new D3DModel();
	ship_->LoadFile("..\\Media\\spacecraft.dae");
	ship_->LoadShaderFile("..\\FxFiles\\DeferredLighting.fxo");
	//scene->LoadShaderFile("..\\FxFiles\\color.fxo");
	ship_->AddToScene();

	cannon_1 = new D3DModel();
	cannon_1->LoadFile("..\\Media\\gun.dae");
	cannon_1->LoadShaderFile("..\\FxFiles\\DeferredLighting.fxo");
	//cannon_1->AddToScene();

	cannon_2 = new D3DModel();
	cannon_2->LoadFile("..\\Media\\gun.dae");
	cannon_2->LoadShaderFile("..\\FxFiles\\DeferredLighting.fxo");
	//cannon_2->AddToScene();
	
	point_light = new PointLight();
	point_light->SetPos(float3(0 ,1000, 0));	
	point_light->AddToScene();

	point_light_1 = new PointLight();
	point_light_1->SetPos(float3(50, 50, 50));
	point_light_1->AddToScene();
}

void MyApp::ReleaseObjects()
{
	delete ship_;
	delete cannon_1;
	delete cannon_2;
}

void MyApp::Update()
{
	//Example for changing Camera View
	float3 pos = float3(0,1,3.5);
	Camera* camera = GetCamera();
	camera->SetView(pos ,float3(0,0,0),float3(0,1,0));
	//frame_buffer->GetFrameCamera()->SetProjection(3.14f/4, 1280/800, 1, 1000);

	//Example for changing model position
	float4x4 model_matrix;
	float4x4 scale_matrix, xrotation_matrix, yrotation_matrix,zrotation_matrix,translate_matrix;
	Math::Identity(model_matrix);

	theta+= 0.001;
	float r = 0.6;
	float3 cannon_pos;
	
	//for all part
	if(theta > 3.14/2)
	{
		Math::ZRotation(zrotation_matrix, Math::PI/4*Math::Sin(theta));
		Math::Translate(translate_matrix, r * Math::PI/6*Math::Cos(theta) ,r*Math::PI/6*Math::Sin(theta* 2) , 0 );	
		model_matrix= zrotation_matrix* translate_matrix * model_matrix;
		cannon_pos = float3(0, 0, 0);
	}
	else
	{
		cannon_pos = float3(0, 5*Math::Cos(theta), 0);
	}

	//for ship
	//Math::XRotation(xrotation_matrix, Math::PI/4*Math::Sin(theta));
	Math::YRotation(yrotation_matrix, Math::PI/2 );	
	ship_->SetModelMatrix(yrotation_matrix* model_matrix);

	//for cannons
	Math::YRotation(yrotation_matrix, Math::PI+ Math::PI/6*Math::Sin(theta));
	Math::Scale(scale_matrix, 0.3);
	Math::Translate(translate_matrix, -3 + cannon_pos.x(), -0.5+ cannon_pos.y(), 3);//right
	cannon_1->SetModelMatrix(yrotation_matrix* translate_matrix*scale_matrix* model_matrix);
	Math::YRotation(yrotation_matrix, Math::PI+ Math::PI/6*Math::Cos(theta));
	Math::Translate(translate_matrix, 3 + cannon_pos.x(), -0.5 + cannon_pos.y(), 3);//left
	cannon_2->SetModelMatrix(yrotation_matrix* translate_matrix*scale_matrix* model_matrix);

	point_light_1->SetPos(float3(55*Math::Cos(theta),55*Math::Sin(theta),55*Math::Sin(0)));



	//std::cout<<"light zpos"<< 5*Math::Sin(theta)<< std::endl;
}

void MyApp::OnKeyDown( WPARAM key_para )
{
	std::cout<<"key down"<<std::endl;
}

void MyApp::OnMouseMove( WPARAM mouse_para, int x, int y )
{
	std::cout<<"mouse move "<<x<<" "<<y<<std::endl;
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

