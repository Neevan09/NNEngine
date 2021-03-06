#pragma once
#include <string>
#include <CodeMath.h>

enum CharacterState
{
	kNoState,
	kIdleOutCombat,
	kIdleInCombat,
	kInjuredIdle,
	kWalk,
	kInjuredWalk,
	kRun,
	kJumpingPrepare,
	kJumpUp,
	kJumpAir,
	kJumpDown,
	kRunJump,
	kAttack,
	kAdjustPosition,
	kHit,
	kHitHeadWeak,
	kDodge,
	kRoll,
	kRunRoll,
	kDead
};

enum HitPosition
{
	kHigh,
	kMid,
	kLow
};

enum HitDirection
{
	kHitForward,
	kHitBackward,
	kHitLeft,
	kHitRight
};

enum CombatDirection
{
	kDirForward,
	kDirLeft,
	kDirRight
};

enum HitPower
{
	kWeak,
	kPowerful,
	kKO
};

enum Combat
{
	kNone,
	kPunchJabL,
	kPunchJabR,
	kPunchHookL,
	kPunchHookR,
	kPunchUpperCutL,
	kPunchUpperCutR,
	kKickStraightMidR,
	kKickKnee,
	kKickAxeKick,
	kKickHorseKick,
	kSwordAttackR,
	kSwordAttackRL,
	kSwordAttackSpU,
	kSwordAttackComboLL,
	kNumberOfItems
};

enum customBool
{
	kTrue,
	kFalse,
	kDoesntMatter
};

struct CombatMoveDetails
{
	// AT - Attack Time
	// ET - Effect Time
	// ED - Effect Distance
	// HT - Hit Time
	// Dmg = Damage Amount
	// Name doesn't matter but is good for recognising.

	std::string name;
	float AT;
	float AD;
	float ET;
	float ED;
	float HT;
	float Dmg;
	HitPosition pos;
	CombatDirection dir;
	HitPower power;
	float strikeDistance;
};

struct CharacterDetails
{
	std::string			modelName;
	std::string			diffuseName;
	std::string			normalMapName;
	std::string			tag;
	codeEngine::math::float3	position;
	codeEngine::math::float3	scale;
	float				health;
	float				walkSpeed;//
	float				sprintSpeed;
	float				jumpPower;
	float				rollDodgeCost;
	codeEngine::math::float2	capsuleColliderSize;
	codeEngine::math::float3	colliderOrigin;
	float				acceleration;
	float				deacceleration;
	// Add more as needed
};


struct CombatManagerDetails
{
	int moveDir;
	float inCombatDuration;
	float maxShotDistance;
	float minShotDistance;
	float jumpUpTime;
	float jumpAirTime;
	float jumpDownTime;
	float comboTimer;
	float maxComboTime;
	float dodgeTime;
	float rollTime;
	float rollSpeed;
	float hitTime;
	float hitMaxWalkSpeed;
	float adjustSpeed;
	float adjustMinDistance;
	float adjustMaxDistance;
	float adjustAgle;
};

//struct PathNode
//{
//	std::string name;
//	codeEngine::math::float3 position;
//	PathNode* nearby_1;
//	PathNode* nearby_2;
//	PathNode* nearby_3;
//	PathNode* nearby_4;
//};

struct SpawnNode
{
	codeEngine::math::float3 position;
};

struct TriggerNode
{
	codeEngine::math::float3 position;
};

struct State
{

	customBool hasTarget;
	// int targetState;
	customBool canTargetAttack;
	customBool canIAttack;
	customBool targetStunned;
	customBool amIStunned;
	customBool isTargetFacingMe;
	customBool amIFacingTheTarget;

	std::string myPrevMove;
	std::string targetPrevMove;

	State(customBool _hasTarget = kTrue, customBool _canTargetAttack = kDoesntMatter, customBool _canIAttack = kTrue, 
		customBool _targetStunned = kDoesntMatter, customBool _amIStunned = kFalse, 
		customBool _isTargetFacingMe = kDoesntMatter, customBool _amIFacingTheTarget = kTrue, 
		std::string _myPrevMove = "", std::string _targetPrevMove = "")
	{
		hasTarget = _hasTarget;
		canTargetAttack = _canTargetAttack;
		canIAttack = _canIAttack;
		targetStunned = _targetStunned;
		amIStunned = _amIStunned;
		isTargetFacingMe = _isTargetFacingMe;
		amIFacingTheTarget = _amIFacingTheTarget;
		myPrevMove = _myPrevMove;
		targetPrevMove = _targetPrevMove;
	}

};

struct Action
{
	// Combat attributes are taken care of by the Combat. The final State contains the estimated final state.
	std::string name;

	State intialState;
	State finalState;

	Combat combat;

	float actionTime;
	float damage;

	// We need a preference to make sure that the actions would not be repeated.
	int preference;


	Action(std::string _name, State _initialState, State _finalState, Combat _combat, 
		float actionTime, float _damage, int _preference = 10)
	{
		name = _name;
		intialState = _initialState;
		finalState = _finalState;
		combat = _combat;
		actionTime = actionTime;
		damage = _damage;
		preference = _preference;
	}

};

struct UIResult
{
	uint32_t errorCode;	// Error handling code
	uint32_t uiCode;	// UI Code for action
};