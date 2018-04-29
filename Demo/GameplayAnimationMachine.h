#pragma once
#include <AnimationComponent.h>
#include "GameplayDS.h"

class CombatManager;

class GameplayAnimationMachine
{
public:
	GameplayAnimationMachine(CombatManager*);
	~GameplayAnimationMachine();

	void Play(CharacterState, float, size_t);
	void SetAnimComp(codeEngine::AnimationComponent);

private:
	codeEngine::AnimationComponent	aComp;
	CombatManager*				combatManager;
};