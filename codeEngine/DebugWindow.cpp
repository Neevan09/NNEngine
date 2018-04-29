#include "DebugWindow.h"

DebugWindow::DebugWindow() {
	timePassed = 0.0f;
	cursorActive = false;
	currentLine = L"";
	enabled = true;

	for (int i = 0; i < MAX_LINES; i++) {
		lines.push_back(L"");
	}
}

DebugWindow::~DebugWindow() {

}

vector<wstring> *DebugWindow::GetLines() {
	return &lines;
}
wstring DebugWindow::GetCurrentLine() {
	return (cursorActive) ? currentLine + L"_" : currentLine;
}

void DebugWindow::Update(float deltaTime) {
	timePassed += deltaTime;
	if (timePassed >= BLINK_INTERVAL) {
		timePassed -= BLINK_INTERVAL;
		cursorActive = !cursorActive;
	}
}

void DebugWindow::AddKey(wstring key) {
	currentLine += key;
	ResetBlinker();
}

void DebugWindow::EnterPressed() {
	if (lines.size() >= MAX_LINES) {
		auto it = lines.begin();
		lines.erase(it);
	}
	lines.push_back(currentLine);
	currentLine = L"";
}

void DebugWindow::BackspacePressed() {
	if (currentLine.size() > 0)
		currentLine = currentLine.erase(currentLine.size() - 1);
	ResetBlinker();
}

void DebugWindow::GravePressed() {
	enabled = !enabled;
}

bool DebugWindow::IsEnabled() {
	return enabled;
}

void DebugWindow::ResetBlinker() {
	timePassed = 0.0f;
	cursorActive = true;
}