#pragma once

#include "CodeMath.h"

namespace codeEngine
{
	class Transform
	{
	public:
		Transform()
			:
			rotation(),
			translation(),
			scale({ 1.0f, 1.0f, 1.0f })
		{}

		Transform(math::float3 translation, math::quat rotation, math::float3 scale)
			:
			rotation(rotation),
			translation(translation),
			scale(scale)
		{}

		TF_INLINE const math::float3&	GetTranslation() const { return translation; }
		TF_INLINE const math::quat&		GetRotation() const { return rotation; }
		TF_INLINE const math::float3&	GetScale() const { return scale; }

		TF_INLINE void					SetTranslation(const math::float3& t) { translation = t; }
		TF_INLINE void					SetTranslation(float x, float y, float z) { translation = { x, y, z }; }

		TF_INLINE void					SetRotation(const math::quat& q) { rotation = q; }
		TF_INLINE void					SetRotation(float theta, const math::float3& axis) { rotation = math::angleAxis(theta, axis); }
		TF_INLINE void					SetRotation(float pitch, float yaw, float roll) { rotation = math::euler(pitch, yaw, roll); }

		TF_INLINE void					SetScale(const math::float3& s) { scale = s; }
		TF_INLINE void					SetScale(float s) { scale = { s, s, s }; }
		TF_INLINE void					SetScale(float x, float y, float z) { scale = { x, y, z }; }

	public:
		// a * b  - apply transform a and then transform b
		Transform operator * (const Transform&) const;
		Transform operator * (const float multiplier) const;
		Transform& operator *= (const float multiplier);

		Transform Inverse() const;

		math::float4x4				GetMatrix() const;

	public:
		math::float3				TransformVector(const math::float3& v) const;
		math::float4				TransformVector(const math::float4& v) const;
		math::float3				TransformPosition(const math::float3& v) const;
		math::float4				TransformPosition(const math::float4& v) const;

		void						SetToRelativeTransform(const Transform & parent);
		void						Blend(const Transform other, float weight);
		void						Additive(const Transform other, float weight);

	private:
		math::quat					rotation;
		math::float3				translation;
		math::float3				scale;
	};
}
