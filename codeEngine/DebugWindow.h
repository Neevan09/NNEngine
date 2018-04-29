#pragma once

#include <string>
#include <vector>
using namespace std;

const float BLINK_INTERVAL = 0.5f;
const int MAX_LINES = 15;

class DebugWindow {
public:
	DebugWindow();
	~DebugWindow();
	vector<wstring> *GetLines();
	wstring GetCurrentLine();
	void Update(float deltaTime);
	void AddKey(wstring key);
	void EnterPressed();
	void BackspacePressed();
	void GravePressed();
	bool IsEnabled();
private:
	bool enabled;
	vector<wstring> lines;
	wstring currentLine;
	float timePassed;
	bool cursorActive;
	void ResetBlinker();
};