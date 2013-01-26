
#pragma once

#include "PreDec.h"
#include "GameState.h"

#include "D3DModel.h"
#include "D3DSkyDome.h"

#include "Picking.h"

class GamingState : public MocapGE::GameState
{
public:
	GamingState(void);
	~GamingState(void);

	virtual void Update();

	virtual void OnKeyDown( WPARAM key_para );

	virtual void OnMouseDown( WPARAM mouse_para, int x, int y );

	virtual void OnMouseUp( WPARAM mouse_para, int x, int y );

	virtual void OnMouseMove( WPARAM mouse_para, int x, int y );

private:
	MocapGE::D3DModel* ship_model,*cannon_1, *cannon_2;
	MocapGE::SceneObject* ship_;

	MocapGE::D3DSkyDome* sky_;

	MocapGE::float2 pre_pos;
	MocapGE::float3 ship_pos;
	int speed_;

	bool mouse_down_;
	bool picked;

	Picking* picking_;

};

