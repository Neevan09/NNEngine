#pragma once

#include <algorithm>
#include <fstream>
#include <map>
#include "Material.h"
#include <string>
#include <sstream>
#include <vector>
#include "UICanvas.h"
using namespace std;

enum UIObjectType {
	Button = 0,
	RadioButton,
	Text
};

struct UIObjectInfo {
	UIObjectType objType;
	float x;
	float y;
	float width;
	float height;
	float r;
	float g;
	float b;
	string text;
	Alignment alignment;
	int radioGroup;
};

class UIObjectLoader {
public:
	UIObjectLoader();
	void LoadObjectsFromFile(string filename, UICanvas &canvas, Material *material);
private:
	void CreateObjects(vector<UIObjectInfo> info, UICanvas &canvas, Material *material);
};