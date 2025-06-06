#include "ResolvingBoardState_ReturningMissplacedBalls.h"



ResolvingBoardState_ReturningMissplacedBalls::ResolvingBoardState_ReturningMissplacedBalls(BilliardsGameplayStateBlackboard* blackboard)
	: ResolvingBoardState(blackboard), m_playerkeepsPlaying(false),
	m_repositionMissplacedBallTimer(0.5f), m_missplacedBalls(nullptr), m_missplacedBallsI(0)
{}

ResolvingBoardState_ReturningMissplacedBalls::~ResolvingBoardState_ReturningMissplacedBalls()
{}



void ResolvingBoardState_ReturningMissplacedBalls::DoEnter()
{
	const std::vector<BilliardBall*>& wellplacedBalls = 
		GetBlackboard()->GetSpecialEventsManager()->GetWellplacedBalls();

	m_missplacedBalls = &GetBlackboard()->GetSpecialEventsManager()->GetMissplacedBalls();
	m_missplacedBallsI = 0;

	m_repositionMissplacedBallTimer.ClearTime();

	m_playerkeepsPlaying = wellplacedBalls.size() > 0 && m_missplacedBalls->size() < 1;

}


bool ResolvingBoardState_ReturningMissplacedBalls::Update()
{
	m_repositionMissplacedBallTimer.Update(GameTime::GetInstance()->GetDeltaTime());
	if (!m_repositionMissplacedBallTimer.HasFinished())
	{
		return false;
	}


	const bool finishedPlacingBalls = m_missplacedBallsI >= m_missplacedBalls->size();
	if (finishedPlacingBalls)
	{
		if (GetBlackboard()->p_victoryAchieved)
		{
			SetNextState(Type::PlayerVictory);
			m_playerkeepsPlaying = false;
		}
		else
		{
			SetNextState(m_playerkeepsPlaying ? Type::KeepPlayer : Type::ChangePlayer);
		}		
		return true;
	}
	

	RepositionCurrentBall();
	m_repositionMissplacedBallTimer.ClearTime();
	m_missplacedBallsI++;

	return false;
}

void ResolvingBoardState_ReturningMissplacedBalls::Exit()
{
	for (size_t i = 0; i < m_missplacedBalls->size(); ++i)
	{
		m_missplacedBalls->at(i)->SetUsingPhysics();
	}

	GetBlackboard()->GetSpecialEventsManager()->ClearMissplacedBalls();
	GetBlackboard()->GetSpecialEventsManager()->ClearWellplacedBalls();	
}



void ResolvingBoardState_ReturningMissplacedBalls::RepositionCurrentBall()
{
	BilliardBall* currentBall = m_missplacedBalls->at(m_missplacedBallsI);
	const Vector2<float> goalPosition =	GetBlackboard()->GetSpecialEventsManager()->FindRandomValidPositionForBall(currentBall);

	currentBall->PlayExitEnterHoleAnimation(goalPosition, m_repositionMissplacedBallTimer.GetDuration() * 0.9f);
}