#pragma once
#include <array>
#include "../Scene.h"
#include "../../GameObjects/Behaviours/Testers/AABoxColliderDrawer.h"
#include "../../GameObjects/Behaviours/Testers/CircleColliderDrawer.h"
#include "../../Systems/GameInput.h"

#include "../../GameObjects/Behaviours/Billiards/BilliardBall.h"
#include "../../GameObjects/Behaviours/Billiards/BilliardStick.h"
#include "../../GameObjects/Behaviours/Billiards/Holes/BilliardsBoardHole.h"

#include "../../GameObjects/Behaviours/Billiards/ScoreDisplay/ConsolePlayerScoresDisplay.h"

#include "../../GameObjects/Behaviours/Billiards/SceneManager/BilliardsGameSceneManager.h"



class BilliardsGameplayManager;

class BilliardsGameScene : public Scene
{
protected:
	virtual void CreateGameObjects() override;
	virtual void DoStart() override;
	virtual void DoUpdate() override;
	virtual void OnDestroy() override;

private:
	BilliardsGameplayManager* CreateGameplayGameObjects();
};