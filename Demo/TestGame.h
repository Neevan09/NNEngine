#pragma once

#include <Module.h>

#include <Entity.h>
#include <TransformComponent.h>
#include <CameraComponent.h>
#include <RenderingComponent.h>
#include <AnimationComponent.h>
#include <PhysicsComponent.h>
#include <LightComponent.h>
#include <AudioManager.h>

class TestGame : public codeEngine::Module
{
public:
	virtual int32_t Init() override;

	virtual int32_t Shutdown() override;

	virtual int32_t Update() override;

private:
	codeEngine::TransformComponent	tGround;
	codeEngine::TransformComponent	tBox;
	codeEngine::TransformComponent	tPlayer;
	codeEngine::TransformComponent	tCamera;
	codeEngine::TransformComponent    tSun;
	codeEngine::TransformComponent    tMoon;
	codeEngine::TransformComponent    tBulb;
	codeEngine::PhysicsComponent		pPlayer;
	codeEngine::AnimationComponent	anim;
	codeEngine::CameraComponent		cam;
	codeEngine::LightComponent        lSun;
	codeEngine::LightComponent        lMoon;
	codeEngine::LightComponent        lBulb;
	float pitch;
	float yaw;
	float speed;
	bool inAir;

//	codeEngine::AudioSource			gameplay { "assets/sounds/Game_Play.wav" };
//	codeEngine::AudioSource			jumpSfx { "assets/sounds/Jump.wav" };
};