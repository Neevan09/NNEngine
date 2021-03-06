#pragma once

#include "Common.h"

#include "Entity.h"
#include "TransformComponent.h"
#include "FBXResourceManager.h"
#include <vector>

namespace codeEngine
{
	struct PathNode
	{
		std::string name;
		codeEngine::math::float3 position;
		PathNode* nearby_1;
		PathNode* nearby_2;
		PathNode* nearby_3;
		PathNode* nearby_4;
	};

	class SceneManager
	{
	public:

		int32_t Init();

		int32_t LoadScene(const char* filename);

		std::vector<PathNode*>* GetPathNodes();

		int32_t GetPathNodesLength();

		codeEngine::math::float3 GetPlayerSpawnPoint();

	private:

		std::vector<PathNode*>* pathNodes;

		codeEngine::math::float3 pSpawnPoint;

		int32_t AddPathNode(const rapidjson::Value& value, int32_t);

		int32_t LoadSceneEntity(const rapidjson::Value& value, TransformComponent parent);

		int32_t LoadComponents(const rapidjson::Value& value, Entity e);

	private:
		ResourceManager		resMgr;
	};
}