#include "Character.h"
#include <PhysicsSystem.h>

using namespace codeEngine;


Character::Character()
{}

Character::~Character()
{
	delete combatManager;
	delete gCharacter;
}

void Character::Init(bool isPlayer, void* comp, CombatManagerDetails combatDetails)
{
	combatManager = new CombatManager(isPlayer, comp, this, combatDetails);
	gCharacter = new GameplayAnimationMachine(combatManager);
	physics = codeEngine::PhysicsSystem::instance();
	once = true;
	inAir = false;
}


void Character::Update(float dT)
{
	if (once)
	{
		once = false;
		pCharacter->LockRotation(true, true, true);
	}
	combatManager->Update(dT);
}

void Character::UpdateState(float dT)
{}




// Handle movement on the ground
void Character::HandleGroundedMovement(math::float3 _moveDir, bool _hasInput, bool _jump, float dT)
{
	float accelMod = 0.0f;
	float deaccelMod = 0.0f;
	// check whether conditions are right to allow a jump
	if (_jump && !jump && isGrounded)
	{
		//charAudio.Stop();
		//charAudio.PlayOneShot(jumpFX);

		combatManager->SetIsJumping(true);
		jump = true;
		hasJumped = true;
		stateTimer = 0;
	}

	if (!isSprinting && !isRolling)
	{
		accelMod = dT * kAccelerate;
		deaccelMod = dT * kDeaccelerate;
	}
	else if (isRolling)
	{
		accelMod = dT * kAccelerate * 1.0f;
	}
	else
	{
		accelMod = dT * kAccelerate * 2.0f;
		deaccelMod = dT * kDeaccelerate * 1.5f;
	}


	if (_hasInput && isGrounded)
	{
		speed += accelMod;
		if (speed > moveSpeedMultiplier)
			speed = moveSpeedMultiplier;

		math::float3 moveVector = _moveDir * speed;
		velocity.x = moveVector.x;
		velocity.z = moveVector.z;
	}

	else if (isGrounded)
	{
		speed -= deaccelMod;
		if (speed < 0.0f) speed = 0.0f;
		math::float3 moveVector = -(charRot * math::float3{ 0, 0, 1 }) * speed;
		velocity.x = moveVector.x;
		velocity.z = moveVector.z;
	}
} // end ground movement


 // Handle airborne movement
 
void Character::HandleAirborneMovement(math::float3 _moveDir, bool _hasInput, float dT)
{
	if (hasJumped)
	{
		velocity.y = lastVelocity.y - gravityMultiplier * dT;
	}
	else
	{
		velocity.y = lastVelocity.y - (gravityMultiplier * dT * 20.0f);
	}
	//velocity.y = lastVelocity.y - gravityMultiplier * dT;
	float x = _moveDir.x * airborneSpeedMultiplier * dT;
	float z = _moveDir.z * airborneSpeedMultiplier * dT;

	velocity.x += x;
	velocity.z += z;

	if (velocity.x > airbornMaxVelocity)
	{
		velocity.x = airbornMaxVelocity;
	}
	else if (velocity.x < -airbornMaxVelocity)
	{
		velocity.x = -airbornMaxVelocity;
	}
	if (velocity.z > airbornMaxVelocity)
	{
		velocity.z = airbornMaxVelocity;
	}
	else if (velocity.z < -airbornMaxVelocity)
	{
		velocity.z = -airbornMaxVelocity;
	}

	if(!jump && isGrounded)
	{
		velocity.y = 0;
	}
} //end airborne movement


 // check to see if player is on the ground and its status
void Character::CheckGroundStatus()
{
	math::float3 pos{ pCharacter->GetPosition() };

	RayTestResult hitInfo = {};
	{
		math::float3 rayStart = pos + math::float3{ 0, 1, 0 };
		math::float3 rayEnd = pos + math::float3{ 0, -0.08f, 0 };

		isGrounded = physics->RayTest(rayStart, rayEnd, &hitInfo);
	}

	// Additional checks to see if the character is standing on an object
	if (!isGrounded)
	{
		math::float3 rayStart = pos + math::float3{ 0, 1, 0.5f };
		math::float3 rayEnd = pos + math::float3{ 0, -0.08f, 0 };

		isGrounded = physics->RayTest(rayStart, rayEnd, &hitInfo);
	}
	if (!isGrounded)
	{
		math::float3 rayStart = pos + math::float3{ 0, 1, -0.5f };
		math::float3 rayEnd = pos + math::float3{ 0, -0.08f, 0 };

		isGrounded = physics->RayTest(rayStart, rayEnd, &hitInfo);
	}
	if (!isGrounded)
	{
		math::float3 rayStart = pos + math::float3{ 0.5f, 1, 0 };
		math::float3 rayEnd = pos + math::float3{ 0, -0.08f, 0 };

		isGrounded = physics->RayTest(rayStart, rayEnd, &hitInfo);
	}
	if (!isGrounded)
	{
		math::float3 rayStart = pos + math::float3{ -0.5f, 1, 0 };
		math::float3 rayEnd = pos + math::float3{ 0, -0.08f, 0 };

		isGrounded = physics->RayTest(rayStart, rayEnd, &hitInfo);
	}

	if (!isGrounded && hasJumped)
	{
		inAir = true;
	}
	if (inAir && isGrounded)
	{
		//charAudio.Stop();
		//charAudio.PlayOneShot(landFX);
		hasJumped = false;
		inAir = false;
	}

	

	//// Raytest to check if the player is standing on an object
	//if (physics->RayTest(pos, end, &hitInfo))
	//{
	//	if (hitInfo.hitWorldNormal.y < 0.9f)
	//	{
	//		moveSpeedMultiplier = slopeSpeedMultiplier;
	//	}
	//	groundNormal = hitInfo.hitWorldNormal;

	//	if (!isGrounded && hasJumped)
	//	{
	//		//charAudio.Stop();
	//		//charAudio.PlayOneShot(landFX);
	//		hasJumped = false;
	//	}
	//	isGrounded = true;
	//}
	//else
	//{
	//	isGrounded = false;
	//	groundNormal = tCharacter->GetUpVector();
	//}
}// end CheckGroundStatus






 //-------------------------------------------------------------------------------------------------
 // Actions
void Character::Attack() {}
void Character::Dodge() {}
void Character::Die() {}

// Sprint (move faster)
void Character::Sprint(bool _sprint)
{
	// TODO
	// Set dashing to true if cool allows
	isSprinting = _sprint;
}

void Character::Special(float, bool, bool) {}


// TODO
// Check to see if forward/right need to be negated to be right
// Force the player to move a bit
void Character::ForceMove(float speed, float dT, int direction)
{
	math::float3 moveDir = {};
	if (direction == 0)
	{
		//tCharacter->Translate(tCharacter->GetForwardVector() * dT * speed);
		moveDir = tCharacter->GetForwardVector();
	}
	else if (direction == 1)
	{
		//tCharacter->Translate(tCharacter->GetForwardVector() * dT * -speed);
		moveDir = tCharacter->GetForwardVector() * -1.0f;
	}
	else if (direction == 2)
	{
		//tCharacter->Translate(tCharacter->GetRightVector() * dT * -speed);
		moveDir = tCharacter->GetRightVector() * -1.0f;
	}
	else if (direction == 3)
	{
		//tCharacter->Translate(tCharacter->GetRightVector() * dT * speed);
		moveDir = tCharacter->GetRightVector();
	}

	if (speed > 500.0f)
		speed = 500.0f;

	math::float3 moveVector = moveDir * speed;
	velocity.x = moveVector.x;
	velocity.z = moveVector.z;
}

// Force the player to move a bit
void Character::ForceMove(float speed, float dT, math::float3 direction)
{
	//tPlayer->Translate += direction * speed * dT;
	tCharacter->Translate(direction * dT * speed);

}

// Force move in the players currentlly facing direction
void Character::ForceMove(float speed, float dT)
{
	math::float3 moveDir = tCharacter->GetForwardVector();

	if (speed > 500.0f)
		speed = 500.0f;

	math::float3 moveVector = moveDir * speed;
	velocity.x = moveVector.x;
	velocity.z = moveVector.z;
}

// Deal damage to the character
void Character::TakeDamage(float dmg)
{
	if (dmg > 0)
	{
		health -= dmg;
	}
	if (health <= 0)
	{
		isDead = true;
		Die();
	}
}

// Use special Energy
// Combo Special move (Sword/Gun)
void Character::UseSpecial(float energyUsed, bool light, bool isGun)
{
	// TODO
	// Use special energy
}

//-------------------------------------------------------------------------------------------------
// Setters

// Set the Animation Parameter
void Character::AnimationParameter(int _animationParameter)
{
	animationParameter = _animationParameter;
}

// Set Character's current state
void Character::CurrentState(CharacterState _currentState)
{
	currentState = _currentState;
}

// Set if action has an effect
void Character::HasEffect(bool _hasEffect)
{
	hasEffect = _hasEffect;
}

// Set the character's last state
void Character::LastState(CharacterState _lastState)
{
	lastState = _lastState;
}

// Set the State Timer
void Character::StateTimer(float _stateTimer)
{
	stateTimer = _stateTimer;
}

// Set transform component
void Character::SetComponents(codeEngine::TransformComponent _tCharacter, codeEngine::PhysicsComponent _pCharacter, 
	codeEngine::AnimationComponent _aCharacter)
{
	tCharacter = _tCharacter;
	pCharacter = _pCharacter;
	aCharacter = _aCharacter;
}

//-------------------------------------------------------------------------------------------------
// Getters

// Get the combat manager
CombatManager* Character::GetCombatManager()
{
	return combatManager;
}

// Get the character's tag
std::string Character::GetTag()
{
	return tag;
}

// Return Character Position
codeEngine::math::float3 Character::GetPosition()
{
	return tCharacter->GetLocalPosition();
}

// Return Character Forward
codeEngine::math::float3 Character::GetForward()
{
	return tCharacter->GetForwardVector();
}

// Return Character Right
codeEngine::math::float3 Character::GetRight()
{
	return tCharacter->GetRightVector();
}

// Return if player is grounded
bool Character::IsGrounded()
{
	return isGrounded;
}

// Return if player is dead
bool Character::IsDead()
{
	return isDead;
}

// Return if action has effect
bool Character::HasEffect()
{
	return hasEffect;
}

// Return the state timer
float Character::StateTimer()
{
	return stateTimer;
}

// Return current state
CharacterState Character::CurrentState()
{
	return currentState;
}

// Return the last state
CharacterState Character::LastState()
{
	return lastState;
}

// Get Custom crossfade duration for combat idle animation
float Character::GetAnimationDuration(CharacterState state)
{
	float duration = 0.03f;
	switch (state)
	{
	case kIdleOutCombat:
		duration = 0.03f;
		break;
	case kIdleInCombat:
		duration = 0.03f;
		break;
	case kWalk:
		duration = 0.03f;
		break;
	case kRun:
		duration = 0.03f;
		break;
	case kJumpingPrepare:
		duration = 0.03f;
		break;
	case kJumpUp:
		duration = 0.03f;
		break;
	case kJumpAir:
		duration = 0.03f;
		break;
	case kJumpDown:
		duration = 0.03f;
		break;
	case kDead:
		duration = 0.0f;
		break;
	case kRoll:
		duration = 0.03f;
		break;
	case kAttack:
	{
		switch (combatManager->GetCurrentCombat())
		{
		case kNone:
			duration = 0.03f;
			break;
		case kPunchJabL:
			duration = 0.03f;
		case kPunchJabR:
			duration = 0.03f;
			break;
		case kPunchHookL:
			duration = 0.03f;
			break;
		case kPunchHookR:
			duration = 0.03f;
			break;
		case kPunchUpperCutL:
			duration = 0.03f;
			break;
		case kPunchUpperCutR:
			duration = 0.03f;
			break;
		case kKickStraightMidR:
			duration = 0.03f;
			break;
		case kKickKnee:
			duration = 0.06f;
			break;
		case kKickAxeKick:
			duration = 0.06f;
			break;
		case kKickHorseKick:
			duration = 0.06f;
			break;
		case kSwordAttackR:
			duration = 0.06f;
			break;
		case kSwordAttackRL:
			duration = 0.06f;
			break;
		case kSwordAttackSpU:
			duration = 0.06f;
			break;
		case kSwordAttackComboLL:
			duration = 0.06f;
			break;
		case kNumberOfItems:
			duration = 0.03f;
			break;
		default:
			break;
		}
	}
	break;
	case kAdjustPosition:
		duration = 0.03f;
		break;
	case kHit:
		duration = 0.03f;
		/*combatManager.HitPosition pos = (combatManager.HitPosition)((parameter / 100) % 10);
		combatManager.HitDirection dir = (combatManager.HitDirection)((parameter / 10) % 10);
		combatManager.HitPower power = (combatManager.HitPower)((parameter / 1) % 10);
		string animationName = "A_Hit_" + pos.ToString() + "_" + dir.ToString() + "_" + power.ToString();
		aComp->Play(animationName, -1, 0);*/
		break;
	}

	return duration;
}