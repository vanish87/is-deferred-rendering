
#pragma once

#include "PreDec.h"
#include "GameState.h"

class GamingState : public MocapGE::GameState
{
public:
	GamingState(void);
	~GamingState(void);

	virtual void Update();

};

