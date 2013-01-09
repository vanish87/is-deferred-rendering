
#pragma once
#include "PreDec.h"
#include "D3DModel.h"

class Cannon
{
public:
	Cannon(void);
	~Cannon(void);

	void Update();
	void Fire();

private:
	MocapGE::D3DModel* model_;
	MocapGE::D3DModel* bullet_;
	
	bool fired;
	bool hit_;
	float bullet_speed_;

	int hp_;
	MocapGE::float3 dir_;
	MocapGE::float3 pos_;

};

