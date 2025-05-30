#pragma once
#include "PlayerThinkingState.h"


class PlayerThinkingState_MovingAround : public PlayerThinkingState
{
public:
	PlayerThinkingState_MovingAround(BilliardsGameplayStateBlackboard* blackboard, BilliardsPlayer* player);

	virtual bool Update() override;
	virtual void Exit() override;

protected:
	virtual void DoEnter() override;

};