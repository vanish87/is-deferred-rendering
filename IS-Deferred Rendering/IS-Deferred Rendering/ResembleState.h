#pragma once

#include "PreDec.h"
#include "GameState.h"
#include "Vector.h"

#include "D3DModel.h"

class Ship;
class Picking;

class ResembleState : public MocapGE::GameState
{
public:
	ResembleState(Ship* ship);
	~ResembleState(void);
	
	virtual void Update();

	virtual void OnKeyDown( WPARAM key_para );

	virtual void OnKeyUp( WPARAM key_para );

	virtual void OnMouseDown( WPARAM mouse_para, int x, int y );

	virtual void OnMouseUp( WPARAM mouse_para, int x, int y );

	virtual void OnMouseMove( WPARAM mouse_para, int x, int y );

private:
	Ship* ship_;

	MocapGE::float3 ship_dir_;
	MocapGE::float3 ship_pos_;
	MocapGE::float3 ship_up_ ;

	bool first_flag_;
	bool left_ctr_down_;

	int cam_speed_deg_;

	bool mouse_down_;
	MocapGE::float2 pre_pos;
	MocapGE::float3 ship_pos;

	Picking* picking_;
	bool picked_;
	MocapGE::D3DModel* picked_model_;
};
