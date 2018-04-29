#pragma once
#include <CodeEngine.h>
#include <LightComponent.h>

class AnimationDemo : public codeEngine::Module
{
public:
	virtual int32_t Init() override;

	virtual int32_t Shutdown() override;

	virtual int32_t Update() override;

private:
	codeEngine::Entity				eBox;

	codeEngine::CameraComponent		cam;
	codeEngine::AnimationComponent	anim;
	codeEngine::TransformComponent	tPlayer;
	codeEngine::TransformComponent	tPlayer2;
	codeEngine::TransformComponent	tBox;
	codeEngine::TransformComponent	tCamera;

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
};

