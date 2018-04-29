#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
class Camera
{
private:
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
	
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT3 upDirection;
	DirectX::XMFLOAT4 rotation;
	DirectX::XMFLOAT3 newUp;
	
	float rotationX;
	float rotationY;
	float speed;
	float camSensitivity;
public:
	Camera();
	Camera(float x, float y, float z);
	~Camera();
	
	void SetViewMatrix();
	DirectX::XMFLOAT4X4 GetViewMatrix();
	
	void SetProjectonMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();
	
	void SetPosition(DirectX::XMFLOAT3);
	DirectX::XMFLOAT3  GetPosition();

	void SetDirection(DirectX::XMFLOAT3);
	DirectX::XMFLOAT3  GetDirection();
	DirectX::XMFLOAT3  GetUp();

	void SetRotationX(float rotX);
	float GetRotationX();
	
	void SetRotationY( float rotY);
	float GetRotationY();
	
	void Rotate();
	void UpdateFromInput(float deltatime);
	void UpdateViewMatrix();

	void UpdateProjectionMatrix(int width, int height);
	void MoveRelative(float x, float y, float z);
	void MoveAbsolute(float x, float y, float z);
};

