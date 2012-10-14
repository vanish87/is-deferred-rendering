#include "MyApp.h"
#include "Context.h"
#include "Vector.h"
#include "Matrix.h"

using namespace MocapGE;
MyApp::MyApp(void) : App("test")
{
}


MyApp::~MyApp(void)
{
}

void MyApp::InitObjects()
{

}

void MyApp::ReleaseObjects()
{

}

int main()
{
	float2 aaa(2.0,3.0);
	float2 c;
	aaa[0];
	int2 a(1,2);
	a = a *2.5;
	float2 g(3.8,4);

	//g=a*2.5;
	//aaa = b+g;
	int2 sss;
	float4 d;

	d.w()=0;

	c = aaa*2;

	aaa.x() = 2;
	float4x4 test,test1,test2;

	int3 cord;

	test = test1 * 2;

	
	//c = b + test;
	//int2 f= b;
	//b.x()+=100;
	float4 e(1,2,3,4);

	d = e / 2;

	Context::Instance().LoadConfig("E:\\12Fall\\Independent Study\\IS-Deferred Rendering\\IS-Deferred Rendering\\Config.xml");
	ContextConfig cfg = Context::Instance().GetConfigure();
	Context::Instance().Configure(cfg);

	MyApp app;
	app.Initialize();
	app.Run();

	return 0;
}