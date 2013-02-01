#pragma once
#include "App.h"
#include "Vector.h"
#include "D3DModel.h"
#include <vector>


class StartMenu;
class ResembleState;
class GamingState;

class Picking;
class Cannon;

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

	typedef std::vector<Cannon*> PartList;

	MocapGE::PointLight* point_light;
	MocapGE::PointLight* point_light_1;
	float theta;

	MocapGE::Timer* timer_;

	StartMenu* start_menu_;
	GamingState* gaming_;
	ResembleState* resembling_;
	MocapGE::Mesh* aabb_mesh_;

};

