#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#pragma once
#include "PreDec.h"

namespace MocapGE
{
	class GameState
	{
	public:
		GameState(void);
		~GameState(void);

		void SetParent(GameState* parent){parent_ = parent;};
		GameState* GetParent(){return parent_;};

		virtual void Update() = 0;

		int GetStateOP() const { return state_op_; }
		int GetNextPhase() const { return phase_to_load_; }  

	private:
		GameState* parent_;

	protected:
		int state_op_;
		int phase_to_load_;
	};



}

#endif

