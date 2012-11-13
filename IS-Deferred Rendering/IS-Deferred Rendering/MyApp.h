#pragma once
#include "App.h"
#include "D3DModel.h"

class MyApp :
	public MocapGE:: App
{
public:
	MyApp(void);
	~MyApp(void);
	void InitObjects();
	void ReleaseObjects();
	void Update();

private:

	MocapGE::D3DModel* scene,*scene_1;
	MocapGE::PointLight* point_light;
	MocapGE::PointLight* point_light_1;
	float theta;
};

