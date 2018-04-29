#include "GameObject.h"

using namespace DirectX;
GameObject::GameObject()
{
}

//-------------------------------------------------------------------
// Initialize GameObjects with a mesh and material
//-------------------------------------------------------------------
GameObject::GameObject(Mesh * mesh,Material* mat)
{
	objectMesh = mesh;
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&objectWorldMatrix, XMMatrixTranspose(W));

	position = XMFLOAT3(0, 0, 0);
	rotation = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);

	// Assign a material to the game object
	material = mat;

	soundManager = nullptr;
}

//------------------------------------------------------------------------------
// Initialize GameObjects with a mesh, material and attach sounds to the object
//------------------------------------------------------------------------------
GameObject::GameObject(Mesh * mesh, Material* mat, std::vector<std::string> soundfiles, AudioEngine *sManager)
{
	objectMesh = mesh;
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&objectWorldMatrix, XMMatrixTranspose(W));

	position = XMFLOAT3(0, 0, 0);
	rotation = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);

	// Assign a material to the game object
	material = mat;

	sounds = soundfiles;
	soundManager = sManager;
	LoadSound();
}

GameObject::~GameObject()
{
}

//-------------------------------------------------------------------
// Set the world matrix of the gameobject
//-------------------------------------------------------------------
void GameObject::SetObjectWorldMatrix(DirectX::XMMATRIX newWorldMatrix)
{
	XMStoreFloat4x4(&objectWorldMatrix, XMMatrixTranspose(newWorldMatrix));		// Transpose is stored to be used in shader
}

//-------------------------------------------------------------------
// Set position of game object
//-------------------------------------------------------------------
void GameObject::SetObjectPosition(DirectX::XMFLOAT3 newPosition )
{
	position = newPosition;
}

//-------------------------------------------------------------------
// Set rotation of the game object
//-------------------------------------------------------------------
void GameObject::SetObjectRotation(DirectX::XMFLOAT3 newRotation)
{
	rotation = newRotation;
}

//-------------------------------------------------------------------
// Set the scale of the gameobject
//-------------------------------------------------------------------
void GameObject::SetObjectScale(DirectX::XMFLOAT3 newScale)
{
	scale = newScale;
}

//-------------------------------------------------------------------
// Return the world matrix
//-------------------------------------------------------------------
DirectX::XMFLOAT4X4 GameObject::GetObjectWorldMatrix()
{
	return objectWorldMatrix;
}

//-------------------------------------------------------------------
// Return the object position
//-------------------------------------------------------------------
DirectX::XMFLOAT3 GameObject::GetObjectPosition()
{
	return position;
}

//-------------------------------------------------------------------
// Return the object rotation
//-------------------------------------------------------------------
DirectX::XMFLOAT3 GameObject::GetObjectRotation()
{
	return rotation;
}

//-------------------------------------------------------------------
// Return the scale of the game object
//-------------------------------------------------------------------
DirectX::XMFLOAT3 GameObject::GetObjectScale()
{
	return scale;
}

//-------------------------------------------------------------------
// Return the mesh of the game object
//-------------------------------------------------------------------
Mesh* GameObject::GetObjectMesh()
{
	return objectMesh;
}

//-------------------------------------------------------------------
// Update the world matrix values of the gameobject
//-------------------------------------------------------------------
void GameObject::UpdateWorldMatrix()
{
	
	XMMATRIX scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX rotMatrix = XMMatrixRotationRollPitchYaw(rotation.x,rotation.y,rotation.z);
	XMMATRIX translateMatrix = XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&objectWorldMatrix, XMMatrixTranspose(scaleMatrix * rotMatrix * translateMatrix));
	for (auto channel : soundChannels)
	{
		soundManager->SetChannel3DPosition(channel, AudioVector3{position.x,position.y,position.z});
	}
}

//-------------------------------------------------------------------
// Move the game object with given velocity
//-------------------------------------------------------------------
void GameObject::Move(XMVECTOR velocityVector)
{
	XMVECTOR pos = XMVectorSet(position.x, position.y, position.z, 0);
	pos += velocityVector;
	XMStoreFloat3(&position, pos);
}

//-------------------------------------------------------------------
// Move object according to individual values
// Overloaded for abstarction
//-------------------------------------------------------------------
void GameObject::Move(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;
	UpdateWorldMatrix();
}

//-------------------------------------------------------------------
// Rotate game object
//-------------------------------------------------------------------
void GameObject::Rotate(float x, float y, float z)
{
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;
	UpdateWorldMatrix();
}

//----------------------------------------------------------------------
// Set the material shader values with right textures and shader values
//----------------------------------------------------------------------
void GameObject::PrepareMaterial(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	this->material->GetVertexShader()->SetMatrix4x4("view", viewMatrix);
	this->material->GetVertexShader()->SetMatrix4x4("projection", projectionMatrix);
	this->material->GetVertexShader()->SetMatrix4x4("world", this->objectWorldMatrix);

	this->material->GetVertexShader()->SetShader();

	this->material->GetPixelShader()->SetShaderResourceView("DiffuseTexture", this->material->GetSRV());
	if(this->material->GetSampler() != nullptr)
		this->material->GetPixelShader()->SetSamplerState("Sampler", this->material->GetSampler());
	this->material->GetPixelShader()->SetShader();

	this->material->GetVertexShader()->CopyAllBufferData();
	this->material->GetPixelShader()->CopyAllBufferData();
}

//-------------------------------------------------------------------
// Return the material of the object
//-------------------------------------------------------------------
Material* GameObject::GetMaterial()
{
	return material;
}

//-------------------------------------------------------------------
// Load Sound from sounds vector
//-------------------------------------------------------------------
void GameObject::LoadSound()
{
	if (!sounds.empty())
	{
		for (auto sound : sounds)
		{
			soundManager->LoadSound(sound, true, true);
		}
	}
}

//-------------------------------------------------------------------
// Play Sound
// Id is the sound id in the sounds vector
// fadeIn is the time taken to fade the sound to max volume in seconds
//-------------------------------------------------------------------
void GameObject::PlayObjectSound(int Id, float fadeIn)
{
	if(!sounds.empty() && Id < sounds.size() && !(Id < soundChannels.size()) )	// Checks if the sound is playing currently
		soundChannels.push_back(soundManager->PlaySounds(sounds[Id], AudioVector3{ position.x,position.y,position.z }, soundManager->VolumeToDecibal(1.0f), fadeIn));
}

//---------------------------------------------------------------------
// Stop Sound
// Id is the sound id in the sound channels vector
// fadeIn is the time taken to fade the sound to zero volume in seconds
//---------------------------------------------------------------------
void GameObject::StopObjectSound(int Id, float fadeOut)
{
	if (!soundChannels.empty())
	{
		soundManager->StopChannel(soundChannels[Id], fadeOut);
		soundChannels.erase(soundChannels.begin() + Id);		// removes the sound that is not playing from channel list
	}
}