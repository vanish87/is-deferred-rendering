
#pragma once
#include "PreDec.h"
#include "D3DModel.h"

class Cannon
{
public:
	Cannon(MocapGE::D3DModel* model, MocapGE::D3DModel* bullet);
	~Cannon(void);

	void Update(MocapGE::float4x4 parent);
	void Fire();
	MocapGE::float3 GetPos();
	void SetPos( MocapGE::float3 pos );
private:
	MocapGE::D3DModel* model_;
	MocapGE::D3DModel* bullet_;
	
	bool fired;
	bool hit_;
	float bullet_speed_;

	int hp_;
	MocapGE::float3 dir_;
	//relative pos to ship
	MocapGE::float3 pos_;

};

