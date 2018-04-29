#include "UICanvas.h"
#include "UIObjectLoader.h"

UIObjectLoader::UIObjectLoader() {

}

void UIObjectLoader::LoadObjectsFromFile(string filename, UICanvas &canvas, Material *material) {
	//Load each line from the file
	vector<string> lines;
	ifstream infile(filename);
	if (!infile.good())
		return;
	while (infile) {
		string line;
		if (!getline(infile, line)) {
			break;
		}
		lines.push_back(line);
	}

	//Parse each line and store values into a struct
	vector<UIObjectInfo> allObjectInfo;
	for (auto line : lines) {
		vector<string> splitString;
		istringstream ss(line);
		while (ss) {
			string s;
			if (!getline(ss, s, ','))
				break;
			splitString.push_back(s);
		}
		
		UIObjectInfo info;
		if (splitString[0] == "Button" || splitString[0] == "RadioButton") {
			info.objType = (splitString[0] == "Button") ? UIObjectType::Button : UIObjectType::RadioButton;
			info.x = stof(splitString[1]);
			info.y = stof(splitString[2]);
			info.width = stof(splitString[3]);
			info.height = stof(splitString[4]);
			info.r = stof(splitString[5]);
			info.g = stof(splitString[6]);
			info.b = stof(splitString[7]);
			if (splitString[0] == "Button")
				info.text = splitString[8];
			if (splitString[0] == "RadioButton")
				info.radioGroup = stoi(splitString[8]);
		}
		else if (splitString[0] == "TextEntity") {
			Alignment align = Alignment::Left;
			if (splitString[2] == "Left")
				align = Alignment::Left;
			else if (splitString[2] == "Right")
				align = Alignment::Right;
			else if (splitString[2] == "Center")
				align = Alignment::Center;
			else {
				return;
			}

			info.objType = UIObjectType::Text;
			info.text = splitString[1];
			info.alignment = align;
			info.x = stof(splitString[3]);
			info.y = stof(splitString[4]);
		}
		else {
			return;
		}
		allObjectInfo.push_back(info);
	}

	CreateObjects(allObjectInfo, canvas, material);
}

void UIObjectLoader::CreateObjects(vector<UIObjectInfo> allInfo, UICanvas &canvas, Material *material) {
	wstring str;
	UIRadioGroup *getRadioGroup;
	for (auto info : allInfo) {
		switch (info.objType) {
		case UIObjectType::Button:
			str = wstring(info.text.begin(), info.text.end());
			canvas.CreateButton(info.x, info.y, info.width, info.height, material, str.c_str(), XMFLOAT4(info.r, info.g, info.b, 1.0f));
			break;

		case UIObjectType::RadioButton:
			//Create radio buttons and create a radio group for them if one does not already exist
			getRadioGroup = canvas.CreateRadioGroup(info.radioGroup);
			canvas.CreateRadioButton(info.x, info.y, info.width, info.height, XMFLOAT4(info.r, info.g, info.b, 1.0f), getRadioGroup);
			break;

		case UIObjectType::Text:
			str = wstring(info.text.begin(), info.text.end());
			canvas.CreateText(str.c_str(), info.alignment, info.x, info.y);
			break;

		}
	}
}