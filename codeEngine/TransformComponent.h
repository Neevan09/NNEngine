#pragma once

#include "Component.h"
#include "Transform.h"
#include <vector>

namespace codeEngine
{
	class TransformComponentData;

	typedef Component<TransformComponentData> TransformComponent;

	class TransformComponentData
	{
	public:
		TransformComponentData() : TransformComponentData(Entity()) {}

		TransformComponentData(Entity e)
			:
			entity(e),
			parent(),
			children()//,
			//dirty(1)
		{}

		void							SetParent(TransformComponent parent, bool updateChildren = true);

		TF_INLINE TransformComponent	GetParent() const { return parent; }

		void							AddChild(TransformComponent child);

		void							RemoveChild(TransformComponent child);
		void							RemoveChild(uint32_t childIdx);

		TF_INLINE uint32_t				GetNumChildren() const { return uint32_t(children.size()); }

		TF_INLINE TransformComponent	GetChild(uint32_t childIdx) const { return children[childIdx]; }

		TF_INLINE Entity				GetEntity() const { return entity; }

		TF_INLINE const Transform&		GetLocalTransform() const { return localTransform; }

		const Transform&				GetWorldTransform() const { return worldTransform; }

	public:

		// auxiliary functions

		TF_INLINE void					SetLocalPosition(const math::float3& pos)
		{
			localTransform.SetTranslation(pos);
			UpdateTransfromInHierachy();
		}

		TF_INLINE void					Translate(const math::float3& t)
		{
			localTransform.SetTranslation(localTransform.GetTranslation() + t);
			UpdateTransfromInHierachy();
		}

		TF_INLINE void					SetLocalRotation(const math::quat& quat)
		{
			localTransform.SetRotation(quat);
			UpdateTransfromInHierachy();
		}

		TF_INLINE void					SetLocalScale(const math::float3& scale)
		{
			localTransform.SetScale(scale);
			UpdateTransfromInHierachy();
		}

		TF_INLINE math::float3			GetLocalPosition() const
		{
			return localTransform.GetTranslation();
		}

		// get position coordinates in world space
		TF_INLINE math::float3			GetWorldPosition() const
		{
			return GetWorldTransform().TransformPosition(math::float3());
		}

		TF_INLINE math::float3			WorldToLocalPosition(math::float3 world) const
		{
			return inverse(GetWorldTransform().GetMatrix()) * math::float4(world, 1.0f);
		}

		TF_INLINE math::float3			GetLocalScale() const
		{
			return localTransform.GetScale();
		}

		TF_INLINE math::float3			GetWorldScale() const
		{
			return worldTransform.GetScale();
		}

		// get right vector in world space
		TF_INLINE math::float3			GetRightVector() const
		{
			return GetWorldTransform().TransformVector(math::float3{ 1, 0, 0 });
		}

		// get up vector in world space
		TF_INLINE math::float3			GetUpVector() const
		{
			return GetWorldTransform().TransformVector(math::float3{ 0, 1, 0 });
		}

		// get forward vector in world space
		TF_INLINE math::float3			GetForwardVector() const
		{
			return GetWorldTransform().TransformVector(math::float3{ 0, 0, 1 });
		}

		TF_INLINE math::quat			GetWorldRotation() const
		{
			return GetWorldTransform().GetRotation();
		}

		// local space
		TF_INLINE void					FaceTo(const math::float3& target)
		{
			if (math::length(target) <= 0.0f)
				return;

			math::float3 fwd{ 0, 0, 1 };
			math::float3 newDir = math::normalize(target);

			float cosTheta = math::dot(fwd, newDir);
			math::float3 axis = math::normalize(math::cross(fwd, newDir));

			if (std::fabsf(cosTheta) >= 1.0 - FLT_EPSILON)
			{
				axis = math::float3{ 0, 1, 0 };
			}

			float theta = std::acosf(cosTheta);
			math::quat q = math::angleAxis(theta, axis);
			assert(math::length(q) > 0.9995f);
			SetLocalRotation(q);
		}

	private:

		void							UpdateTransfromInHierachy();

	private:
		Entity							entity;
		TransformComponent				parent;
		std::vector<TransformComponent>	children;

		Transform						localTransform;
		Transform						worldTransform;
		//uint32_t						dirty : 1;
	};
}