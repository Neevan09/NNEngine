#pragma once

#include <CodeEngine.h>
#include <AudioManager.h>
#include "Character.h"
#include "PController.h"
#include "Camera.h"
#include "Enemy.h"
//#include "Utility.h"

using namespace codeEngine;

// Game: The main game loop.
// Runs all things game related.
class Game : public codeEngine::Module
{
public:
	virtual int32_t Init() override;

	virtual int32_t Shutdown() override;

	virtual int32_t Update() override;

	virtual int32_t FixedUpdate() override;

	virtual ~Game() override;

private:
	// Temp variables for testing
	codeEngine::TransformComponent tIntro;
	codeEngine::TransformComponent tCube;
	codeEngine::TransformComponent tBox;
	codeEngine::TransformComponent tBox2;
	codeEngine::TransformComponent tBox3;
	codeEngine::TransformComponent tBox4;
	codeEngine::TransformComponent tBox5;
	codeEngine::TransformComponent tBox6;
	codeEngine::TransformComponent tBox7;
	codeEngine::TransformComponent tGround;

	codeEngine::TransformComponent tStair;
	codeEngine::PhysicsComponent pStair;

	// Class variables
	//codeEngine::CameraComponent cam;
	Camera* cam;
	Player* player;
	Companion* comp;
	PController* pControl;

	std::vector<Character*>* enemyList;
	std::vector<PathNode*>* pathNodes;

	enum sceneType
	{
		intro = 0,
		menu = 1,
		options = 2,
		help = 3,
		loading = 4,
		tutorial = 5,
		level = 6,
		pause = 7,
		death = 8,
		levelEnd = 9,
		credits = 10,
	};

	codeEngine::SceneManager sceneMgr;
	sceneType currentScene;
	sceneType lastScene;
	sceneType levelToLoad;

	// Member Functions
	// These may be temporary or call the needed functions.
	// Will change int to file name later for JSON use.
	uint32_t LoadScene(sceneType);
	uint32_t UnloadScene(sceneType);
	uint32_t LoadOnTop(sceneType);
	uint32_t UnloadOffTop(sceneType);
	UIResult LoadUI(sceneType);

	//*********************************************************************************************
	//temp for test
	Enemy* enemy01;
	Enemy* enemy02;
	Enemy* enemy03;
	Enemy* enemy04;
	Enemy* enemy05;
	Enemy* enemy06;
	float timePassed;
	float startTime;
	bool loopStart;
	//*********************************************************************************************

	bool debugMode;

	//Utility::GhostPlayer* DebugPlayer;

	// Sound

	uint32_t PlayBGM(sceneType scene);
//	uint32_t StopBGM(codeEngine::AudioSource*);

	/*codeEngine::AudioSource*			currentTrack = {};
	codeEngine::AudioSource			title_BGM{ "assets/sounds/Title_BGM.wav" };
	codeEngine::AudioSource			game_BGM{ "assets/sounds/Game_BGM.wav" };
	codeEngine::AudioSource			options_BGM{ "assets/sounds/Options_BGM.wav" };
	codeEngine::AudioSource			credits_BGM{ "assets/sounds/Credits_BGM.wav" };
	codeEngine::AudioSource			loading_BGM{ "assets/sounds/Loading_BGM.wav" };
	codeEngine::AudioSource			combat_BGM{ "assets/sounds/Combat_BG.wav" };
	codeEngine::AudioSource			death_BGM{ "assets/sounds/Death_BGM.wav" };
	codeEngine::AudioSource			lowHealth_BGM{ "assets/sounds/Heart_Beat.wav" };
	codeEngine::AudioSource			city_BGA{ "assets/sounds/City_BGA.wav" };*/

	bool isTitle_Playing;
	bool isGame_Playing;
	bool isOptions_Playing;
	bool isCredits_Playing;
	bool isLoading_Playing;
	bool isCombat_Playing;
	bool isDeath_Playing;
	bool isLowHealth_Playing;
	bool isCity_Playing;
	
	// UI
	codeEngine::TextureHandle			uiBGTex_0;
	codeEngine::TextureHandle			uiButTex;
	codeEngine::TextureHandle			uiGamePadPC;
	codeEngine::Atlas					atlas;

	uint32_t					mainMenuSelectedItem;
	uint32_t					levelMenuSelectedItem;
	uint32_t					optionMenuSelectedItem;
	uint32_t					helpMenuSelectedItem;
	uint32_t					creditsMenuSelectedItem;
	uint32_t					endMenuSelectedItem;
	uint32_t					deathMenuSelectedItem;

	bool						mainMenuFocused;
	bool						levelMenuFocused;
	bool						optionMenuFocused;
	bool						helpMenuFocused;

	uint32_t					inverseCameraAxisX;
	uint32_t					inverseCameraAxisY;

	UIResult					uiResult;

	uint32_t					uiTimer;
};