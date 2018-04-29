#pragma once

#include <AnimationComponent.h>
#include <TransformComponent.h>
#include<CodeMath.h>

// Player Companion Class
// Sword/Gun for the player to use in combat
// Also may act as the map displayer, among other possible features
class Companion
{
public:
	Companion(codeEngine::math::float3);
	~Companion();

	void Update(float, codeEngine::math::float3, codeEngine::math::float3);
	void FixedUpdate(float, codeEngine::math::float3, codeEngine::math::float3);

	void SetInUse(bool);
	void SetActive(bool);
	void SetTarget(codeEngine::math::float3);

	bool ActiveSelf();
	codeEngine::math::float3 GetPosition();

private:
	codeEngine::TransformComponent	tComp;
	codeEngine::AnimationComponent	aComp;

	bool inUse;
	bool targetSet;
	bool isActive;

	codeEngine::math::float3 target;
	codeEngine::math::float3 targetLastPos;
	codeEngine::math::float3 targetFwd;


	float distance = 50.0f;
	float height = 1.7f;
	float heightDamping = 1.0f;
	float positionDamping = 2.0f;
	float rotationDamping = 1.0f;
};