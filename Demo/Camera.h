#pragma once

#include <PhysicsSystem.h>
#include <TransformComponent.h>
#include <CameraComponent.h>

// Camera class
// Has a camera component and takes input from PController
class Camera
{
public:
	Camera();
	~Camera();

	// TODO
	// move, rotate, 
	void Rotate(codeEngine::math::float2);
	void FixedUpdate(float);
	void Update(float);

	codeEngine::math::quat GetRotation();
	codeEngine::math::float3 GetForward();

	void SetClearColor(codeEngine::math::float4);
	void SetDistMod(codeEngine::math::float3);
	void SetFov(float);
	void SetPosition(codeEngine::math::float3);
	void SetSensitivity(float);
	void SetSkybox(codeEngine::TextureHandle);
	void SetTarget(codeEngine::math::float3);

private:
	//void AdjustCameraPosition(RayTestResult, codeEngine::math::float3, float dT);
	bool BumperCheck(codeEngine::math::float3);
	bool movedBack;

	codeEngine::CameraComponent		cam;
	codeEngine::TransformComponent	tCamera;
	codeEngine::PhysicsSystem*		physics;

	codeEngine::math::float3 camTarget;
	codeEngine::math::quat camRot;
	codeEngine::math::float3 camPos;
	codeEngine::math::float3 distMod;			// sets the distance between target and look at target
	codeEngine::math::float3 targetLastPos;	// target's last known position
	codeEngine::math::float3 toTarget;
	codeEngine::math::float3 bumperRayOffset;	// allows offset of the bumper ray from target origin

	float fov;
	float pitch;
	float yaw;
	float sensitive;
	float bumperDistanceCheck = 2.0f;	// length of bumper ray
	float bumperMaxDistance = 0.5f;
	float damping = 6.0f;				// damping
	float lowerTiltAngle = 45.0;		// lower limit of camera Y tilt
	float upperTiltAngle = 110.0f;		// upper limit of camera Y tilt
	float minDistance = 3.0f;			// closet camera should get
	float maxDistance = 6.0f;			// furthest camera should get
	float adjustLerpTime = 0.0f;
	float lerpTime = 1.0f;
	float distanceFromTarget;

	const float kMaxPitch = codeEngine::math::PI * 0.25f;
	const float kMinPitch = codeEngine::math::PI * -0.125f;
};