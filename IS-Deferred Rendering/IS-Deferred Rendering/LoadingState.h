#pragma once

#include "PreDec.h"
#include "GameState.h"

class LoadingState : public MocapGE::GameState
{
public:
	LoadingState(void);
	~LoadingState(void);

	void SetNextPhase(int phase) {phase_to_load_ = phase;};
	GameState* GetNextState(){return next_state_;};

	virtual void Update();

private:
	GameState* next_state_;

};
