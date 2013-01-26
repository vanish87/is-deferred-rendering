#pragma once

#include "PreDec.h"
#include "GameState.h"

class LoadingState : public MocapGE::GameState
{
public:
	LoadingState(void);
	~LoadingState(void);
	
	virtual void Update();

};
