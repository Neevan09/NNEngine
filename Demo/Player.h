#pragma once

#include "Character.h"
//#include "Gun.h"

class Player : public Character
{
public:
	Player(CharacterDetails, void*);
	~Player();

	void MoveReg(float, bool, codeEngine::math::float3, codeEngine::math::quat);
	//void MoveEnemy(float, bool, codeEngine::math::float3);
	void Update(float);
	void FixedUpdate(float);
	void UpdateState(float);

	void Attack(bool, float);
	void Dodge(codeEngine::math::float3);
	void Die();
	void Interact();
	void Special(bool, float);
	void VisionHack();

	/*
	void AnimationParameter(int _animationParameter);
	
	void CheckGroundStatus();
	void CurrentState(CharacterState _currentState);
	
	
	void ForceMove(float, float, int);
	void ForceMove(float, float, codeEngine::math::float3);
	void HasEffect(bool _hasEffect);
	void HandleAirborneMovement(codeEngine::math::float3);
	void HandleGroundedMovement(bool);
	
	void LastState(CharacterState _lastState);
	
	void Sprint(bool);
	void StateTimer(float _stateTimer);
	

	bool HasEffect();
	bool IsDead();
	bool IsGrounded();

	float StateTimer();*/

	/*codeEngine::math::float3 GetPosition();
	codeEngine::math::float3 GetForward();
	codeEngine::math::float3 GetRight();*/

	//CharacterState CurrentState();
	//CharacterState LastState();

private:

	codeEngine::TransformComponent	tPlayer;
	codeEngine::PhysicsComponent		pPlayer;
	codeEngine::AnimationComponent	aPlayer;
	codeEngine::PhysicsSystem*		physics;
	
	codeEngine::math::quat camRotation;

	codeEngine::math::float3 lastMoveDir;

	//Gun* gun;

	bool attackButtonDown;
	bool specialButtonDown;
	float attackButtonTimer;
	float minHoldTime;
	float maxHoldTime;
	float specialButtonTimer;
	float rollDodgeCost;

	//GameObject charBody;
	//GameObject camera;

	
	//CapstoneAnimation animator;

	//Rigidbody rigidbody;


	// Audio
	/*
	AudioSource charAudio;
	AudioSource charCombatAudio;
	AudioClip footsteps1;
	AudioClip footsteps2;
	AudioClip footsteps3;
	AudioClip footsteps4;
	AudioClip jumpFX;
	AudioClip landFX;
	*/
};
