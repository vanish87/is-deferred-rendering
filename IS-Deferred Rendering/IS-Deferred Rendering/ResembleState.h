#pragma once

#include "PreDec.h"
#include "GameState.h"

class ResembleState : public MocapGE::GameState
{
public:
	ResembleState(void);
	~ResembleState(void);
	
	virtual void Update();

};
