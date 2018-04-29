#pragma once
#include <DirectXMath.h>
#include "Mesh.h"
#include "Material.h"
#include "SimpleShader.h"
#include "Lights.h"
#include <vector>
#include "AudioEngine.h"
class GameObject
{
protected:
	DirectX::XMFLOAT4X4 objectWorldMatrix;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
	Mesh *objectMesh;
	Material* material;
	std::vector<std::string> sounds;
	std::vector<int> soundChannels;
	AudioEngine *soundManager;
	void LoadSound();
public:
	GameObject();
	GameObject(Mesh* mesh, Material* material);
	GameObject(Mesh * mesh, Material* mat, std::vector<std::string> soundfiles, AudioEngine * sManager);
	~GameObject();

	//Setters
	void SetObjectWorldMatrix(DirectX::XMMATRIX);
	void SetObjectPosition(DirectX::XMFLOAT3);
	void SetObjectRotation(DirectX::XMFLOAT3);
	void SetObjectScale(DirectX::XMFLOAT3);

	//Getters
	DirectX::XMFLOAT4X4 GetObjectWorldMatrix();
	DirectX::XMFLOAT3 GetObjectPosition();
	DirectX::XMFLOAT3 GetObjectRotation();
	DirectX::XMFLOAT3 GetObjectScale();
	Mesh* GetObjectMesh();
	Material* GetMaterial();

	void UpdateWorldMatrix();
	void Move(DirectX::XMVECTOR velocityVector);
	void Move(float x, float y, float z);
	void Rotate(float x, float y, float z);

	void PrepareMaterial(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);
	
	void PlayObjectSound(int Id, float fadeIn = 0.0f);
	void StopObjectSound(int Id, float fadeOut = 0.0f);
};

