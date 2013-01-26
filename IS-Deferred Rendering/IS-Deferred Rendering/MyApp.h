#pragma once
#include "App.h"
#include "Vector.h"


class StartMenu;
class LoadingState;
class GamingState;

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
	virtual void OnMouseMove( WPARAM mouse_para, int x, int y );
	virtual void OnMouseUp( WPARAM mouse_para, int x, int y );

private:


	MocapGE::PointLight* point_light;
	MocapGE::PointLight* point_light_1;
	float theta;

	MocapGE::Timer* timer_;

	StartMenu* start_menu_;
	GamingState* gaming_;
	MocapGE::Mesh* aabb_mesh_;

};

