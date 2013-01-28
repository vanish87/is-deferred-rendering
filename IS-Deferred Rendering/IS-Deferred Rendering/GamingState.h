
#pragma once

#include "PreDec.h"
#include "GameState.h"

#include "D3DModel.h"
#include "D3DSkyDome.h"

#include "Picking.h"
#include "Ship.h"

class GamingState : public MocapGE::GameState
{
public:
	GamingState(void);
	~GamingState(void);

	virtual void Update();

	virtual void OnKeyDown( WPARAM key_para );

	virtual void OnKeyUp(WPARAM key_para);

	virtual void OnMouseDown( WPARAM mouse_para, int x, int y );

	virtual void OnMouseUp( WPARAM mouse_para, int x, int y );

	virtual void OnMouseMove( WPARAM mouse_para, int x, int y );

	Ship* GetShip(){ return ship_;};
private:
	MocapGE::D3DModel* ship_model,*cannon_1, *cannon_2;
	MocapGE::SceneObject* scene_ship_;

	MocapGE::D3DSkyDome* sky_;

	Ship* ship_;
	bool spacekey_down_, wkey_down_, skey_down_;

};

