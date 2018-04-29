#pragma once

#include <CodeEngine.h>

enum CharacterStates
{
	kStateIdle,
	kStateFalling,
	kStateRunning,
	kStateJumpingPrepare,
	kStateJumpingUp,
};

class CharacterControllerJam : public codeEngine::Module
{
public:
	virtual int32_t Init() override;

	virtual int32_t Shutdown() override;

	virtual int32_t Update() override;

	virtual int32_t FixedUpdate() override;

private:

	void AddAnimState(const char* name, bool isLoop = true, const char* clipname = nullptr);

private:
	codeEngine::SceneManager			sceneMgr;
	codeEngine::CameraComponent		cam;
	codeEngine::AnimationComponent	anim;
	codeEngine::TransformComponent	tPlayer;
	codeEngine::TransformComponent	tCamera;
	codeEngine::PhysicsComponent		pPlayer;
	codeEngine::TransformComponent	tBox;
	// Dummy light
	codeEngine::TransformComponent    tSun;
	codeEngine::LightComponent        lSun;
	codeEngine::TransformComponent    tMoon;
	codeEngine::TransformComponent    tBulb;
	codeEngine::LightComponent        lMoon;
	codeEngine::LightComponent        lBulb;
	float						pitch;
	float						yaw;
	float						speed;

	CharacterStates				state;
	bool						inAir;
};