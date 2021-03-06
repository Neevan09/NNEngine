#include "CameraComponent.h"
#include "TransformComponent.h"

namespace codeEngine
{
	using namespace math;

	void CameraComponentData::LookAt(const float3& target)
	{
		TransformComponent t = entity.GetComponent<TransformComponent>();
		assert(true == t);
		// TODO
	}

	void CameraComponentData::LookTo(const float3& direction)
	{
		TransformComponent t = entity.GetComponent<TransformComponent>();
		assert(true == t);
		// TODO
	}

	float4x4 CameraComponentData::CalcViewMatrix() const
	{
		TransformComponent t = entity.GetComponent<TransformComponent>();
		assert(true == t);

		float3 forward = t->GetForwardVector();

		float3 pos = t->GetWorldPosition();

		return lookTo(pos, forward, float3{ 0, 1, 0 });
	}

	float4x4 CameraComponentData::CalcProjectionMatrix() const
	{
		assert(projType == ProjectionType::kProjectionTypePerspective && "Othro is not implemented yet.");
		return perspective(fov * PI / 180.0f, aspect, zNear, zFar);
	}


}

