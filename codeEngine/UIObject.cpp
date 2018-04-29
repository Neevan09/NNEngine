#include "UIObject.h"

UIObject::UIObject() : GameObject() {
	
}
UIObject::UIObject(Mesh* mesh, Material* material, float x, float y, float width, float height, XMFLOAT4 color) : GameObject(mesh, material) {
	this->color = color;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	currentColor = color;
	mouseOver = false;
	mousePressed = false;
}
UIObject::~UIObject() {

}

void UIObject::SetColor(XMFLOAT4 color) {
	this->color = color;
}

XMFLOAT4 UIObject::GetColor()
{
	return currentColor;
}

float UIObject::GetX() {
	return x;
}

float UIObject::GetY() {
	return y;
}

void UIObject::Update(float deltaTime, float totalTime) {
	
}

void UIObject::OnMouseMove(float x, float y) {

}

void UIObject::OnMouseDown(float x, float y) {

}

void UIObject::OnMouseUp() {

}