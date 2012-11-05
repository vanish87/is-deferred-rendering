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
	scene = new D3DModel();
	scene->LoadFile("heart.dae");
	scene->LoadShaderFile("..\\FxFiles\\color.fxo");
	scene->AddToScene();
}

void MyApp::ReleaseObjects()
{
	delete scene;
}

void MyApp::Update()
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

