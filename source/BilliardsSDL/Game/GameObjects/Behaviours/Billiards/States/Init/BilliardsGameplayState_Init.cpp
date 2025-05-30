#include "BilliardsGameplayState_Init.h"




BilliardsGameplayState_Init::BilliardsGameplayState_Init(BilliardsGameplayStateBlackboard* blackboard)
    : BilliardsGameplayState(blackboard)
{}

BilliardsGameplayState_Init::~BilliardsGameplayState_Init()
{
}



void BilliardsGameplayState_Init::DoEnter()
{
    GameRenderManager::GetInstance()->SetBackgroundColor(Colors::DarkPurple);
    GetBlackboard()->GetSpecialEventsManager()->PositionBallsRandomly();
}

bool BilliardsGameplayState_Init::Update()
{
    SetNextState(Type::PlacingBalls);
    return true;
}

void BilliardsGameplayState_Init::Exit()
{
}
