
#pragma once

#include "PreDec.h"
#include "D3DModel.h"

#include "Cannon.h"

class Ship
{
public:
	Ship(void);
	~Ship(void);

	void Update();

private:

	MocapGE::D3DModel model_;
	std::vector<Cannon*> cannons_;

	float acceleration_;
	int hp_;
	float phi_;
	float theta_;
	MocapGE::float3 pos_;
};

