#pragma once

#include "GameObject.h"

class UIObject : public GameObject {
protected:
	XMFLOAT4 color;
	XMFLOAT4 currentColor;
	float x;
	float y;
	float width;
	float height;
	bool mouseOver;
	bool mousePressed;
public:
	UIObject();
	UIObject(Mesh* mesh, Material* material, float x, float y, float width, float height, XMFLOAT4 color);
	virtual ~UIObject();
	void SetColor(XMFLOAT4 color);
	XMFLOAT4 GetColor();
	float GetX();
	float GetY();
	virtual void Update(float deltaTime, float totalTime);
	virtual void OnMouseMove(float x, float y);
	virtual void OnMouseDown(float x, float y);
	virtual void OnMouseUp();
};