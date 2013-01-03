#pragma once
#include "App.h"
#include "D3DModel.h"



class StartMenu;
class LoadingState;

class Picking;

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

	virtual void OnMouseDown( WPARAM mouse_para, int x, int y );

private:

	MocapGE::D3DModel* ship_model,*cannon_1, *cannon_2;
	MocapGE::SceneObject* ship_;
	MocapGE::PointLight* point_light;
	MocapGE::PointLight* point_light_1;
	float theta;

	MocapGE::Timer* timer_;

	StartMenu* start_menu_;
	LoadingState* loading_;
	Picking* picking_;
};

