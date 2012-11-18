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

	virtual void OnKeyDown( WPARAM key_para );

	virtual void OnMouseMove( WPARAM mouse_para, int x, int y );

private:

	MocapGE::D3DModel* ship_,*cannon_1, *cannon_2;
	MocapGE::PointLight* point_light;
	MocapGE::PointLight* point_light_1;
	float theta;
};

