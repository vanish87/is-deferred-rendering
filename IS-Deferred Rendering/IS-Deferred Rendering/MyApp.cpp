#include "MyApp.h"
#include "Context.h"

using namespace MocapGE;
MyApp::MyApp(void) : App("test")
{

}


MyApp::~MyApp(void)
{
}

void MyApp::InitObjects()
{
	theta = 0;
	scene = new D3DModel();
	scene->LoadFile("..\\Media\\pumpkin.dae");
	scene->LoadShaderFile("..\\FxFiles\\DeferredRendering.fxo");
	scene->AddToScene();

	//scene_1 = new D3DModel();
	//scene_1->LoadFile("..\\Media\\pumpkin.dae");
	//scene_1->LoadShaderFile("..\\FxFiles\\color.fxo");
	//scene_1->AddToScene();
	
	point_light = new PointLight();
	point_light->SetPos(float3(0 ,1000, 0));	
	point_light->AddToScene();

	point_light_1 = new PointLight();
	point_light_1->SetPos(float3(50, 50, 50));
	point_light_1->AddToScene();
}

void MyApp::ReleaseObjects()
{
	delete scene;
}

void MyApp::Update()
{
	//Example for changing Camera View
	float3 pos = float3(0,1,3.5);
	FrameBuffer* frame_buffer;
	frame_buffer= Context::Instance().GetRenderFactory().GetRenderEngine().CurrentFrameBuffer();
	frame_buffer->GetFrameCamera()->SetView(pos ,float3(0,0,0),float3(0,1,0));
	//frame_buffer->GetFrameCamera()->SetProjection(3.14f/4, 1280/800, 1, 1000);

	//Example for changing model position
	float4x4 model_matrix = scene->GetModelMatrix();
	Math::Identity(model_matrix);
	float4x4 scale_matrix, xrotation_matrix, yrotation_matrix,translate_matrix;
	Math::Identity(scale_matrix);
	Math::Identity(xrotation_matrix);
	Math::Identity(yrotation_matrix);
	Math::Identity(translate_matrix);
	scale_matrix(0,0) = 0.1;
	scale_matrix(1,1) = 0.1;
	scale_matrix(2,2) = 0.1;
	theta+= 0.001;
	xrotation_matrix[1][1] = Math::Cos(theta);
	xrotation_matrix[1][2] = -Math::Sin(theta);
	xrotation_matrix[2][1] = Math::Sin(theta);
	xrotation_matrix[2][2] = Math::Cos(theta);

	yrotation_matrix[0][0] = Math::Cos(theta);
	yrotation_matrix[0][2] = Math::Sin(theta);
	yrotation_matrix[2][0] = -Math::Sin(theta);
	yrotation_matrix[2][2] = Math::Cos(theta);
	//model_matrix = yrotation_matrix ;
	translate_matrix[3][0] = Math::Cos(theta);
	translate_matrix[3][1] = 0;
	translate_matrix[3][2] = 0;
	//scene->SetModelMatrix(yrotation_matrix*model_matrix);
	point_light_1->SetPos(float3(55*Math::Cos(theta),0,55*Math::Sin(theta)));
	//std::cout<<"light zpos"<< 5*Math::Sin(theta)<< std::endl;
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

