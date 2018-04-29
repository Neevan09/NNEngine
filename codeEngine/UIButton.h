#pragma once

#include "UIObject.h"
#include <string>
#include <vector>
#include <functional>
using namespace std;

enum ButtonState {
	Normal = 0,
	Hover,
	Pressed
};

class UIButton : public UIObject {
protected:
	XMFLOAT4 hoverColor;
	XMFLOAT4 pressedColor;
	ButtonState state;
	wstring text;
	vector<void*> delegateFunctions;
	std::function<void()> myfunction;
public:
	UIButton();
	UIButton(Mesh* mesh, Material* material, float x, float y, float width, float height, wstring text, XMFLOAT4 color);
	virtual ~UIButton();
	virtual void CheckState();
	virtual void Update(float deltaTime, float totalTime);
	void OnMouseMove(float mouseX, float mouseY);
	void OnMouseDown(float mouseX, float mouseY);
	void OnMouseUp();
	void AssignDelegateFunction(void *function);
	void AssignFunc(std::function<void()> myfunc);
	virtual void OnClick();
	wstring GetText();
};