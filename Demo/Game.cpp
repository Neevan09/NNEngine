#include "Game.h"
#include "PhysicsSystem.h"

using namespace codeEngine;

typedef const rapidjson::Value& value_t;

InputSystem* input;

Game::~Game()
{
	//*********************************************************************************************
	//temp for test
	delete enemy01;
	delete enemy02;
	delete enemy03;
	delete enemy04;
	delete enemy05;
	delete enemy06;
	//*********************************************************************************************

	if (enemyList != nullptr)
	{
		delete enemyList;
	}
	delete comp;
	delete cam;
	delete player;
	delete pControl;
//	delete DebugPlayer;
}

// Intialization of Game components
int32_t Game::Init()
{
	uint32_t ret;

	uiResult = {};

	input = InputSystem::instance();

	debugMode = false;

	if (!debugMode)
	{
		// Create a camera
		cam = new Camera();

		// Create a Player Controller
		pControl = new PController();
		pControl->SetCamera(cam);

		comp = NULL;
		player = NULL;
		enemyList = nullptr;
		
	}

	isTitle_Playing = false;
	isGame_Playing = false;
	isOptions_Playing = false;
	isCredits_Playing = false;
	isLoading_Playing = false;
	isCombat_Playing = false;
	isDeath_Playing = false;
	isLowHealth_Playing = false;
	isCity_Playing = false;
		
	// Load the initial scene (Defalut is Intro)
	// Load other scenes here for fast testing
	//currentScene = intro;

	//PlayBGM(currentScene);

	// Set up initial UI
/*	{
		uiTimer = 0;
		deathMenuSelectedItem = 0;

		uiBGTex_0 = RenderingSystem::instance()->CreateTexture("assets/UI_Textures/title_background.texture", kResourceGlobal);
		uiButTex = RenderingSystem::instance()->CreateTexture("assets/UI_Textures/ui.texture", kResourceGlobal);
		uiGamePadPC = RenderingSystem::instance()->CreateTexture("assets/UI_Textures/gamePad_PC.texture", kResourceGlobal);
		//uiGamePadPS = RenderingSystem::instance()->CreateTexture("assets/UI_Textures/gamePad_PS.texture", kResourceGlobal);

		CHECKED(atlas.LoadFromFile("assets/UI_Textures/ui.json"));

		if (!uiBGTex_0 || !uiButTex)
		{
			return kErrUILoadingFailed - 1;
		}


		uiResult = LoadUI(currentScene);
		assert(uiResult.errorCode != kErrUILoadingFailed);

	}*/

	// Set up scene
	//ret = LoadScene(currentScene);
	//assert(ret == kOK);

	currentScene = level;// menu;
	ret = LoadScene(level);// menu);
	assert(ret == kOK);

	//*********************************************************************************************
	//temp for test
	timePassed = 0.0f;
	loopStart = true;
	//*********************************************************************************************

	return kOK;
}

// Exit the Game
int32_t Game::Shutdown()
{
	return kOK;
}

int32_t Game::FixedUpdate()
{
	uint32_t ret;
	switch (currentScene)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	{
		break;
	}
		case 5:	// Tutorial
		{
			break;
		}
		case 6:	// Level
		{
			if (debugMode)
			{
				//DebugPlayer->Update();
			}
			else
			{
				comp->FixedUpdate(Time::DeltaTime, player->GetPosition(), player->GetForward());
				cam->FixedUpdate(Time::FixedDeltaTime);
				pControl->FixedUpdate(Time::FixedDeltaTime);
				player->FixedUpdate(Time::FixedDeltaTime);
			}
			break;
		}
		case 7:
		case 8:
		case 9:
		case 10:
		{
			break;
		}
		default:
			return kErrGameFixedUpdateFailed;
			break;
	}

	return kOK;
}


// Main upate loop
int32_t Game::Update()
{
	uint32_t ret;

	input = InputSystem::instance();

	if (!debugMode)
	{
		pControl->Update();

		// should pause?
		if (pControl->GetPause())
		{
			//Temp for Testing
			Engine::instance()->Quit();

			//TODO
			//Pause the game and load the pasue menu
		}

		// Run UI
		uiResult = LoadUI(currentScene);
		assert(uiResult.errorCode != kErrUILoadingFailed);
	}

	// Switch for game state
	// Only handles fully loaded Scenes
	// Will be overhauled to use JSON file names and possibley multi-threading
	switch (currentScene)
	{
	case 0:	// Intro
	{

		if (uiResult.uiCode == 1)
		{
			// Load next scene
			currentScene = level;// menu;
			ret = LoadScene(level);// menu);
			assert(ret == kOK);

			// Unload last scene
			lastScene = intro;
			ret = UnloadScene(intro);
			assert(ret == kOK);
		}
		break;
	}
	case 1:	// Menu
	{
		if (uiResult.uiCode == 2)
		{
			// Load next scene
			currentScene = loading;
			ret = LoadScene(loading);
			assert(ret == kOK);

			// Unload last scene
			lastScene = menu;
			ret = UnloadScene(menu);
			assert(ret == kOK);
		}
		else if (uiResult.uiCode == 5)
		{
			// Load next scene
			currentScene = loading;
			levelToLoad = tutorial;
			ret = LoadScene(loading);
			assert(ret == kOK);

			// Unload last scene
			lastScene = menu;
			ret = UnloadScene(menu);
			assert(ret == kOK);
		}
		else if (uiResult.uiCode == 6)
		{
			// Load next scene
			currentScene = loading;
			levelToLoad = level;
			ret = LoadScene(loading);
			assert(ret == kOK);

			// Unload last scene
			lastScene = intro;
			ret = UnloadScene(intro);
			assert(ret == kOK);
		}
		else if (uiResult.uiCode == 10)
		{
			// Load next scene
			currentScene = credits;
			ret = LoadScene(credits);
			assert(ret == kOK);

			// Unload last scene
			lastScene = menu;
			ret = UnloadScene(menu);
			assert(ret == kOK);
		}
		{
			int camXMod = inverseCameraAxisX;
			int camYMod = inverseCameraAxisY;

			if (camXMod == 0) { camXMod = 1; }
			else if (camXMod == 1) { camXMod = -1; }
			if (camYMod == 0) { camYMod = -1; }
			else if (camYMod == 1) { camYMod = 1; }
			pControl->SetControlMods(1, 1, camYMod, camXMod);
		}
		break;
	}
	case 4:	// Loading
	{
		if (levelToLoad = level)
		{
			currentScene = level;
			ret = LoadScene(level);
			assert(ret == kOK);

			// Unload last scene
			lastScene = tutorial;
			ret = UnloadScene(tutorial);
			assert(ret == kOK);
		}
		else if (levelToLoad = tutorial)
		{

		}
		break;
	}
	case 5:	// Tutorial
	{
		if (uiResult.uiCode == 6)
		{
			// Load next scene
			currentScene = level;
			ret = LoadScene(level);
			assert(ret == kOK);

			// Unload last scene
			lastScene = tutorial;
			ret = UnloadScene(tutorial);
			assert(ret == kOK);
		}
		break;
	}
	case 6:	// Level
	{
		if (debugMode)
		{
			//debugPlayer->Update();
		}
		else
		{
			//if (input->IsButtonDown(ButtonId::kKeyEnter))
			//{
			//	// Load next scene
			//	currentScene = credits;
			//	ret = LoadScene(credits);
			//	assert(ret);

			//	// Unload last scene
			//	lastScene = level;
			//	ret = UnloadScene(level);
			//	assert(ret);
			//}

			// May also need to run player update here
			comp->Update(Time::DeltaTime, player->GetPosition(), player->GetForward());
			cam->Update(Time::DeltaTime);
			pControl->UpdateP(Time::DeltaTime);
			player->Update(Time::DeltaTime);
			//enemy01->Update(Time::DeltaTime);
			//enemy02->Update(Time::DeltaTime);
			//enemy03->Update(Time::DeltaTime);
			//enemy04->Update(Time::DeltaTime);
			//enemy05->Update(Time::DeltaTime);
			//enemy06->Update(Time::DeltaTime);

			//*******************************************************************************
			// Temp Enemy control code
			if (loopStart)
			{
				startTime = Time::TotalTime;
				loopStart = false;
			}

			timePassed = Time::TotalTime - startTime;

			//if (timePassed > 0 && timePassed <= 10)	// Move Forward
			//{
			//	enemy01->Move(Time::DeltaTime, false, math::float3{0, 0, 1});
			//}
			//else if (timePassed > 10 && timePassed <= 20) // Move Left
			//{
			//	enemy01->Move(Time::DeltaTime, false, math::float3{-1, 0, 0 });
			//}
			//else if (timePassed > 20 && timePassed <= 30) // Move Backward
			//{
			//	enemy01->Move(Time::DeltaTime, false, math::float3{0, 0, -1});
			//}
			//else if (timePassed > 30 && timePassed <= 40) // Move Right
			//{
			//	enemy01->Move(Time::DeltaTime, false, math::float3{1, 0, 0});
			//}

			if (timePassed > 40)
			{
				timePassed = 0.0f;
				loopStart = true;
			}
		}
		assert(!(timePassed < 0) && timePassed < 50);
		//*******************************************************************************

		break;
	}
	case 8: // End of Level
	{
		if (uiResult.uiCode == 5)
		{
			// Load next scene
			currentScene = tutorial;
			ret = LoadScene(tutorial);
			assert(ret == kOK);

			// Unload last scene
			lastScene = death;
			ret = UnloadScene(death);
			assert(ret == kOK);
		}
		else if (uiResult.uiCode == 6)
		{
			// Load next scene
			currentScene = level;
			ret = LoadScene(level);
			assert(ret == kOK);

			// Unload last scene
			lastScene = death;
			ret = UnloadScene(death);
			assert(ret == kOK);
		}
		else if (uiResult.uiCode == 10)
		{
			// Load next scene
			currentScene = credits;
			ret = LoadScene(credits);
			assert(ret == kOK);

			// Unload last scene
			lastScene = death;
			ret = UnloadScene(death);
			assert(ret == kOK);
		}
		break;
	}
	case 9: // End of Level
	{
		if (uiResult.uiCode == 10)
		{
			// Load next scene
			currentScene = credits;
			ret = LoadScene(credits);
			assert(ret == kOK);

			// Unload last scene
			lastScene = levelEnd;
			ret = UnloadScene(levelEnd);
			assert(ret == kOK);
		}
		break;
	}
	case 10: // Credits
	{
		if (uiResult.uiCode == 1)
		{
			// Load next scene
			currentScene = menu;
			ret = LoadScene(menu);
			assert(ret == kOK);

			// Unload last scene
			lastScene = credits;
			ret = UnloadScene(credits);
			assert(ret == kOK);
		}
		break;
	}
	default:
		return kErrGameUpdateFailed;
		break;
	}

	return kOK;
}// End of Update Loop

// Load a scene
// Most scenes actually have nothing to load
// Will load from JSON files later, later as in never
uint32_t Game::LoadScene(sceneType num)
{
	switch (num)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	{
		break;
	}
		case 5:
		{
			// TODO
			// Tutorial Scene
			// Fully Loaded
			break;
		}
		case 6:
		{
			// Setup the Scene
			CHECKED(sceneMgr.Init());

			//CHECKED(sceneMgr.LoadScene("assets/scenes/Node_Export.json"));
			//CHECKED(sceneMgr.LoadScene("assets/scenes/Engine_Tut.json"));
			//CHECKED(sceneMgr.LoadScene("assets/scenes/OptiTest.json"));
			//CHECKED(sceneMgr.LoadScene("assets/scenes/OptiTest_Batched.json"));
			//CHECKED(sceneMgr.LoadScene("assets/scenes/EngineExport.json"));
			//CHECKED(sceneMgr.LoadScene("assets/scenes/Tutorial_WIP.json"));
			CHECKED(sceneMgr.LoadScene("assets/scenes/Tutorial_WIP_Batched.json"));
			//CHECKED(sceneMgr.LoadScene("assets/scenes/Tutorial_WIP.json"));
			//CHECKED(sceneMgr.LoadScene("assets/scenes/EngineExport.json"));

			int pathLength = sceneMgr.GetPathNodesLength();
			if (pathLength > 0)
			{
				pathNodes = new std::vector<PathNode*>();
				pathNodes = sceneMgr.GetPathNodes();
				assert(pathNodes != NULL);

				// loop through nodes and link up references to nearby nodes
				// TODO this could be slow and need to be redone later or elsewhere
				for (uint32_t i = 0; i < pathNodes->size(); i++)
				{
					std::string name = pathNodes->at(i)->name;
					for (uint32_t j = 0; j < pathNodes->size(); j++)
					{
						if (pathNodes->at(j)->nearby_1->name == name)
						{
							pathNodes->at(j)->nearby_1 = pathNodes->at(i);
						}
						else if (pathNodes->at(j)->nearby_2->name == name)
						{
							pathNodes->at(j)->nearby_2 = pathNodes->at(i);
						}
						else if (pathNodes->at(j)->nearby_3->name == name)
						{
							pathNodes->at(j)->nearby_3 = pathNodes->at(i);
						}
						else if (pathNodes->at(j)->nearby_4->name == name)
						{
							pathNodes->at(j)->nearby_4 = pathNodes->at(i);
						}
					}
				}
			}// end if path length > 0

			if (debugMode)
			{
				//DebugPlayer = new Utility::GhostPlayer(codeEngine::math::float3{ -80.0f, 13.0f, 887.0f });
			}
			else
			{
				CharacterDetails playerDetails = {};
				playerDetails.tag = "player";
				playerDetails.modelName = "assets/archer.model";
				playerDetails.diffuseName = "assets/archer_0.texture";
				playerDetails.normalMapName = "assets/archer_1.texture";
				playerDetails.capsuleColliderSize = { 50.0f, 90.0f };
				playerDetails.colliderOrigin = { 0.0f, 100.0f, 0.0f };
				playerDetails.scale = { 0.01f, 0.01f, 0.01f };
				playerDetails.position = sceneMgr.GetPlayerSpawnPoint();
				playerDetails.health = 200.0f;
				playerDetails.jumpPower = 5.0f;
				playerDetails.walkSpeed = 4.0f;
				playerDetails.sprintSpeed = 8.0f;
				playerDetails.acceleration = 4.0f;
				playerDetails.deacceleration = 10.0f;
				playerDetails.rollDodgeCost = 10.0f;


				// Setup the Player's Companion
				if (comp == NULL)
				{
					comp = new Companion(playerDetails.position);
					pControl->SetCompanion(comp);
				}

				// Setup the Player
				if (player == NULL)
				{
					player = new Player(playerDetails, comp);
					pControl->SetPlayer(player);
				}
				assert(player != NULL);

				TextureHandle tex = RenderingSystem::instance()->CreateTexture("assets/nightSky.dds");

				cam->SetSkybox(tex);
				cam->SetPosition(playerDetails.position);
				cam->SetTarget(playerDetails.position);				
				
				/*CharacterDetails enemyDetails = {};
				enemyDetails.tag = "player";
				enemyDetails.modelName = "assets/archer.model";
				enemyDetails.diffuseName = "assets/archer_0.texture";
				enemyDetails.normalMapName = "assets/archer_1.texture";
				enemyDetails.capsuleColliderSize = { 50.0f, 90.0f };
				enemyDetails.colliderOrigin = { 0.0f, 100.0f, 0.0f };
				enemyDetails.scale = { 0.01f, 0.01f, 0.01f };
				enemyDetails.health = 200.0f;
				enemyDetails.jumpPower = 5.0f;
				enemyDetails.walkSpeed = 4.0f;
				enemyDetails.sprintSpeed = 8.0f;
				enemyDetails.acceleration = 4.0f;
				enemyDetails.deacceleration = 10.0f;
				enemyDetails.rollDodgeCost = 10.0f;
				enemyDetails.position = codeEngine::math::float3{413.0f, 2.0f, 77.0f};*/

				//*********************************************************************************************
				//temp for test
				/*Model* model = RenderingSystem::instance()->CreateModel("assets/archer.model");

				Material* material = RenderingSystem::instance()->CreateMaterial(MaterialType::kMaterialDeferredGeometryOpaqueSkinned);
				TextureHandle diffuse = RenderingSystem::instance()->CreateTexture("assets/archer_0.texture");
				TextureHandle normalMap = RenderingSystem::instance()->CreateTexture("assets/archer_1.texture");

				material->SetTexture(diffuse);
				material->SetNormalMap(normalMap);*/

				//enemy01 = new Enemy(enemyDetails, model, material);
				//enemyDetails.position = codeEngine::math::float3{ 410.0f, 2.0f, 70.0f };
				//enemy02 = new Enemy(enemyDetails, model, material);
				//enemyDetails.position = codeEngine::math::float3{ 423.0f, 2.0f, 67.0f };
				//enemy03 = new Enemy(enemyDetails, model, material);
				//enemyDetails.position = codeEngine::math::float3{ 424.0f, 2.0f, 67.0f };
				//enemy04 = new Enemy(enemyDetails, model, material);
				//enemyDetails.position = codeEngine::math::float3{ 411.0f, 2.0f, 70.0f };
				//enemy05 = new Enemy(enemyDetails, model, material);
				//enemyDetails.position = codeEngine::math::float3{ 414.0f, 2.0f, 77.0f };
				//enemy06 = new Enemy(enemyDetails, model, material);
				//*********************************************************************************************

				// Add each enemy to the enemy list
				// enemyList = new std::vector<Character*>();
				// enemyList.pushback(enemy01);

				player->GetCombatManager()->SetEnemyList(enemyList);
			}
			break;
		}
		case 7:
		case 8:
		case 9:
		case 10:
		{
			break;
		}
		default:
			return kErrSceneLoadFailed;
			break;
	}// End of Switch

	uint32_t ret = PlayBGM(currentScene);
	assert(ret == kOK);

	return kOK;
}

// Unload a scene
// Most scenes have nothing to unload
uint32_t Game::UnloadScene(sceneType num)
{
	switch (num)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	{
		break;
	}
	case 5:
	{
		//tBox4.Destroy();
		//tBox5.Destroy();
		break;
	}
	case 6:
	{
		//tBox6.Destroy();
		//tBox7.Destroy();
		break;
	}
	case 7:
	case 8:
	case 9:
	case 10:
	{
		break;
	}
	default:
		return kErrSceneUnloadFailed;
		break;
	}

	return kOK;
}

// Load a scene on top of another
uint32_t Game::LoadOnTop(sceneType num)
{
	// Temp code until JSON is implemented
		switch (num)
		{
		case 7:
		{
			// TODO
			// Top Load Pause Scene
			break;
		}
		default:
			return kErrSceneLoadFailed;
			break;
		}

	return kOK;
}

// Unload a top loaded scene
uint32_t Game::UnloadOffTop(sceneType num)
{
	// Temp code until JSON is implemented
	switch (num)
	{
	case 7:
	{
		// TODO
		// Unload Pause Scene
		break;
	}
	default:
		return kErrSceneUnloadFailed;
		break;
	}

	return kOK;
}

// Load UI
UIResult Game::LoadUI(sceneType num)
{
	UIResult result = {};

	result.errorCode = kErrUILoadingFailed;
	result.uiCode = 0;

	GUI* gui = GUI::instance();

	gui->SetCanvasSize(1920, 1080);

	switch (num)
	{
	case 0:
	{
		//  Initial UI
		{
			// Background texture
			gui->SetupLayer(0, uiBGTex_0);
			gui->Texture(0, -960, -540, 1920, 1080);

			// Title Text
			// Layer, pos x, pos, y, size, text, color, algin, algin 2
			gui->Text(0, 0, 0, 256, "Code Engine", math::float4(0, 1, 1, 1), kTextAlignCenter | kTextAlignMiddle);

			// Button
			gui->SetupLayer(1, uiButTex);

			GUIStyle style1 = {
				{ 1, 1, 1, 0.5f },
				{ 1, 1, 1, 1 },
				atlas.rects[0],
				atlas.rects[0],
			};

			gui->BeginMenu(mainMenuSelectedItem, true);

			gui->BeginMenuItem();
			// layer, pos x, pos y, width, height, style
			gui->Image(1, -150, 200, 288, 53, style1);
			gui->EndMenuItem();

			mainMenuSelectedItem = gui->EndMenu();

			if (input->IsButtonReleased(kKeyEnter) || input->IsButtonReleased(kGamepadFaceDown))
			{
				// Only one button
				result.uiCode = 1;

				mainMenuFocused = true;
			}
		}
		break;
	}
	case 1:
	{
		//  Title UI
		{
			gui->SetupLayer(0, uiBGTex_0);
			gui->Texture(0, -960, -540, 1920, 1080);

			gui->SetupLayer(1, uiButTex);

			GUIStyle style1 = {
				{ 1, 1, 1, 0.5f },
				{ 1, 1, 1, 1 },
				atlas.rects[0],
				atlas.rects[0],
			};
			GUIStyle style2 = {
				{ 1, 1, 1, 0.5f },
				{ 1, 1, 1, 1 },
				atlas.rects[1],
				atlas.rects[1],
			};
			GUIStyle style3 = {
				{ 1, 1, 1, 0.5f },
				{ 1, 1, 1, 1 },
				atlas.rects[5],
				atlas.rects[5],
			};
			GUIStyle style4 = {
				{ 1, 1, 1, 0.5f },
				{ 1, 1, 1, 1 },
				atlas.rects[2],
				atlas.rects[2],
			};
			GUIStyle style5 = {
				{ 1, 1, 1, 0.5f },
				{ 1, 1, 1, 1 },
				atlas.rects[4],
				atlas.rects[4],
			};


			GUIStyle style6 = {
				{ 1, 1, 1, 0.5f },
				{ 1, 1, 1, 1 },
				atlas.rects[3],
				atlas.rects[3],
			};
			GUIStyle style7 = {
				{ 1, 1, 1, 0.5f },
				{ 1, 1, 1, 1 },
				atlas.rects[7],
				atlas.rects[7],
			};
			GUIStyle style8 = {
				{ 1, 1, 1, 0.5f },
				{ 1, 1, 1, 1 },
				atlas.rects[6],
				atlas.rects[6],
			};

			if (!optionMenuFocused	 && !helpMenuFocused)
			{
				gui->Text(0, 0, -400, 128, "Code Engine", math::float4(0, 1, 1, 1), kTextAlignCenter | kTextAlignTop);

				// main menu
				gui->BeginMenu(mainMenuSelectedItem, mainMenuFocused);

				gui->BeginMenuItem();
				gui->Image(1, -500, 0, 288, 53, style1);
				gui->EndMenuItem();

				gui->BeginMenuItem();
				gui->Image(1, -500, 60, 288, 53, style2);
				gui->EndMenuItem();

				gui->BeginMenuItem();
				gui->Image(1, -500, 120, 288, 53, style3);
				gui->EndMenuItem();

				gui->BeginMenuItem();
				gui->Image(1, -500, 180, 288, 53, style4);
				gui->EndMenuItem();

				gui->BeginMenuItem();
				gui->Image(1, -500, 240, 288, 53, style5);
				gui->EndMenuItem();

				mainMenuSelectedItem = gui->EndMenu();

				if (levelMenuFocused)
				{
					gui->BeginMenu(levelMenuSelectedItem, levelMenuFocused);

					gui->BeginMenuItem();
					gui->Image(1, -144, 0, 288, 53, style6);
					gui->EndMenuItem();

					gui->BeginMenuItem();
					gui->Image(1, -144, 60, 288, 53, style7);
					gui->EndMenuItem();

					gui->BeginMenuItem();
					gui->Image(1, -144, 120, 288, 53, style8);
					gui->EndMenuItem();

					levelMenuSelectedItem = gui->EndMenu();
				}
			}
			else if(optionMenuFocused)
			{
				gui->BeginMenu(optionMenuSelectedItem, optionMenuFocused);

				gui->BeginMenuItem();
				gui->Label(1, -400, -100, 400, 50, 36, "Inverse Camera Axis X", style1, kTextAlignLeft | kTextAlignMiddle);

				gui->BeginSwitch(300, -100, 100, 50, inverseCameraAxisX);
				gui->Option(1, 36, "Off", style1);
				gui->Option(1, 36, "On", style1);
				inverseCameraAxisX = gui->EndSwitch();

				gui->EndMenuItem();

				gui->BeginMenuItem();
				gui->Label(1, -400, -50, 400, 50, 36, "Inverse Camera Axis Y", style1, kTextAlignLeft | kTextAlignMiddle);

				gui->BeginSwitch(300, -50, 100, 50, inverseCameraAxisY);
				gui->Option(1, 36, "Off", style1);
				gui->Option(1, 36, "On", style1);
				inverseCameraAxisY = gui->EndSwitch();

				gui->EndMenuItem();

				gui->BeginMenuItem();
				gui->Image(1, -144, 120, 288, 53, style8);
				gui->EndMenuItem();

				optionMenuSelectedItem = gui->EndMenu();
			}
			else if (helpMenuFocused)
			{
				gui->BeginMenu(helpMenuSelectedItem, helpMenuFocused);

				gui->SetupLayer(1, uiGamePadPC);
				gui->Texture(1, -480, -270, 960, 540);

				gui->SetupLayer(2, uiButTex);

				gui->BeginMenuItem();
				gui->Image(2, -150, 400, 288, 53, style8);
				gui->EndMenuItem();

				helpMenuSelectedItem = gui->EndMenu();
			}

			if (input->IsButtonReleased(kKeyEnter) || input->IsButtonReleased(kGamepadFaceDown))
			{
				if (mainMenuFocused)
				{
					switch (mainMenuSelectedItem)
					{
					case 0:
						mainMenuFocused = false;
						levelMenuFocused = true;
						levelMenuSelectedItem = 0;
						break;
					case 1:
						mainMenuFocused = false;
						optionMenuFocused = true;
						optionMenuSelectedItem = 0;
						break;
					case 2:
						mainMenuFocused = false;
						helpMenuFocused = true;
						break;
					case 3:
						mainMenuFocused = false;
						creditsMenuSelectedItem = 0;
						result.uiCode = 10;
						break;
					case 4:
						Engine::instance()->Quit();
						break;
					default:
						break;
					}
				}
				else if (levelMenuFocused)
				{
					switch (levelMenuSelectedItem)
					{
					case 0:
						uiTimer = 0;
						levelMenuFocused = false;
						result.uiCode = 5;
						break;
					case 1:
						uiTimer = 0;
						levelMenuFocused = false;
						result.uiCode = 6;
						break;
					case 2:
						mainMenuFocused = true;
						levelMenuFocused = false;
						break;
					default:
						break;
					}
				}
				else if (optionMenuFocused)
				{
					switch (optionMenuSelectedItem)
					{
					case 2:
						mainMenuFocused = true;
						optionMenuFocused = false;
						break;
					default:
						break;
					}
				}
				else if (helpMenuFocused)
				{
					switch (helpMenuSelectedItem)
					{
					case 0:
						helpMenuFocused = false;
						mainMenuFocused = true;
						break;
					default:
						break;
					}
				}
			}
		}
		break;
	}
	case 2:
	{
		// TODO
		gui->SetupLayer(0, uiBGTex_0);
		gui->Texture(0, -960, -540, 1920, 1080);

		gui->SetupLayer(1, uiButTex);

		GUIStyle style1 = {
			{ 1, 1, 1, 0.5f },
			{ 1, 1, 1, 1 },
			atlas.rects[0],
			atlas.rects[0],
		};
		GUIStyle style8 = {
			{ 1, 1, 1, 0.5f },
			{ 1, 1, 1, 1 },
			atlas.rects[6],
			atlas.rects[6],
		};

		{
			gui->BeginMenu(optionMenuSelectedItem, optionMenuFocused);

			gui->BeginMenuItem();
			gui->Label(1, -400, -100, 400, 50, 36, "Inverse Camera Axis X", style1, kTextAlignLeft | kTextAlignMiddle);

			gui->BeginSwitch(300, -100, 100, 50, inverseCameraAxisX);
			gui->Option(1, 36, "Off", style1);
			gui->Option(1, 36, "On", style1);
			inverseCameraAxisX = gui->EndSwitch();

			gui->EndMenuItem();

			gui->BeginMenuItem();
			gui->Label(1, -400, -50, 400, 50, 36, "Inverse Camera Axis Y", style1, kTextAlignLeft | kTextAlignMiddle);

			gui->BeginSwitch(300, -50, 100, 50, inverseCameraAxisY);
			gui->Option(1, 36, "Off", style1);
			gui->Option(1, 36, "On", style1);
			inverseCameraAxisY = gui->EndSwitch();

			gui->EndMenuItem();

			gui->BeginMenuItem();
			gui->Image(1, -144, 120, 288, 53, style8);
			gui->EndMenuItem();

			optionMenuSelectedItem = gui->EndMenu();
		}

		if (input->IsButtonReleased(kKeyEnter) || input->IsButtonReleased(kGamepadFaceDown))
		{
			switch (optionMenuSelectedItem)
			{
			case 2:
				mainMenuFocused = true;
				//result.uiCode = ;
				break;
			default:
				break;
			}
		}

		break;
	}
	case 3:
	{
		// TODO
		// Help UI
		break;
	}
	case 4:
	{
		// Background texture
		gui->SetupLayer(0, uiBGTex_0);
		gui->Texture(0, -960, -540, 1920, 1080);

		// TODO Find better loading feeback
		// Layer, pos x, pos, y, size, text, color, algin, algin 2
		if(uiTimer < 16)
			gui->Text(0, -300, 500, 128, "Loading", math::float4(0, 1, 1, 1), kTextAlignLeft | kTextAlignMiddle);
		else if(uiTimer > 15 && uiTimer < 31)
			gui->Text(0, -300, 500, 128, "Loading.", math::float4(0, 1, 1, 1), kTextAlignLeft | kTextAlignMiddle);
		else if (uiTimer > 30 && uiTimer < 46)
			gui->Text(0, -300, 500, 128, "Loading..", math::float4(0, 1, 1, 1), kTextAlignLeft | kTextAlignMiddle);
		else if (uiTimer > 45 && uiTimer < 61)
			gui->Text(0, -300, 500, 128, "Loading...", math::float4(0, 1, 1, 1), kTextAlignLeft | kTextAlignMiddle);
		else if (uiTimer > 60 && uiTimer < 76)
			gui->Text(0, -300, 500, 128, "Loading....", math::float4(0, 1, 1, 1), kTextAlignLeft | kTextAlignMiddle);

		uiTimer++;
		if (uiTimer > 75)
			uiTimer = 0;
		break;
	}
	case 5:
	{
		// TODO
		//  Tutorial UI
		break;
	}
	case 6:
	{
		// TODO
		//  Main Level UI
		break;
	}
	case 7:
	{
		// TODO
		// Pause UI
		break;
	}
	case 8:
	{
		gui->SetupLayer(0, uiBGTex_0);
		gui->Texture(0, -960, -540, 1920, 1080);

		gui->SetupLayer(1, uiButTex);

		GUIStyle style1 = {
			{ 1, 1, 1, 0.5f },
			{ 1, 1, 1, 1 },
			atlas.rects[9],
			atlas.rects[9],
		};
		GUIStyle style2 = {
			{ 1, 1, 1, 0.5f },
			{ 1, 1, 1, 1 },
			atlas.rects[3],
			atlas.rects[3],
		};
		GUIStyle style3 = {
			{ 1, 1, 1, 0.5f },
			{ 1, 1, 1, 1 },
			atlas.rects[10],
			atlas.rects[10],
		};

		gui->Text(0, 0, -150, 128, "You", math::float4(0, 1, 1, 1), kTextAlignCenter | kTextAlignMiddle);

		gui->Text(0, 0, -50, 128, "Died", math::float4(0, 1, 1, 1), kTextAlignCenter | kTextAlignMiddle);

		gui->BeginMenu(deathMenuSelectedItem, true);

		gui->BeginMenuItem();
		gui->Image(1, -150, 60, 288, 53, style1);
		gui->EndMenuItem();

		gui->BeginMenuItem();
		gui->Image(1, -150, 120, 288, 53, style2);
		gui->EndMenuItem();

		gui->BeginMenuItem();
		gui->Image(1, -150, 180, 288, 53, style3);
		gui->EndMenuItem();

		deathMenuSelectedItem = gui->EndMenu();

		if (input->IsButtonReleased(kKeyEnter) || input->IsButtonReleased(kGamepadFaceDown))
		{
			switch (deathMenuSelectedItem)
			{
			case 0:
				result.uiCode = 6;
				break;
			case 1:
				result.uiCode = 5;
				break;
			case 2:
				result.uiCode = 10;
				break;
			default:
				break;
			}
		}

		break;
	}
	case 9:
	{
		// Background texture
		gui->SetupLayer(0, uiBGTex_0);
		gui->Texture(0, -960, -540, 1920, 1080);

		gui->Text(0, 0, -50, 128, "Level", math::float4(0, 1, 1, 1), kTextAlignCenter | kTextAlignMiddle);

		gui->Text(0, 0, 50, 128, "Complete", math::float4(0, 1, 1, 1), kTextAlignCenter | kTextAlignMiddle);

		// Button
		gui->SetupLayer(1, uiButTex, 512u, 1024u);

		GUIStyle style1 = {
			{ 1, 1, 1, 0.5f },
			{ 1, 1, 1, 1 },
			atlas.rects[2],
			atlas.rects[2],
		};

		gui->BeginMenu(endMenuSelectedItem, true);

		gui->BeginMenuItem();
		// layer, pos x, pos y, width, height, style
		gui->Image(1, -150, 200, 288, 53, style1);
		gui->EndMenuItem();

		endMenuSelectedItem = gui->EndMenu();

		if (input->IsButtonReleased(kKeyEnter) || input->IsButtonReleased(kGamepadFaceDown))
		{
			// Only one button
			result.uiCode = 10;
			creditsMenuSelectedItem = 0;
		}

		break;
	}
	case 10:
	{
		{
			// Background texture
			
			

			gui->BeginMenu(creditsMenuSelectedItem, true);

			gui->BeginMenuItem();
			// layer, pos x, pos y, width, height, style
			//gui->Image(1, -150, 490, 288, 53, style1);
			gui->EndMenuItem();

			creditsMenuSelectedItem = gui->EndMenu();

			if (input->IsButtonReleased(kKeyEnter) || input->IsButtonReleased(kGamepadFaceDown))
			{
				// Only one button
				result.uiCode = 1;

				mainMenuFocused = true;
			}
		}
		break;
	}
	default:
		result.errorCode = kErrUILoadingFailed; // Not OK
		break;
	}

	result.errorCode = kOK;

	return result;
}


// Play Background Music
uint32_t Game::PlayBGM(sceneType scene)
{
	uint32_t ret = kOK;

	switch (scene)
	{
	case Game::intro:
	{
		if (!isTitle_Playing)
		{
			//title_BGM.Play();
			isTitle_Playing = true;
		}
	}
		break;
	case Game::menu:
		if (!isTitle_Playing)
		{
		//	title_BGM.Play();
			isTitle_Playing = true;
		}
		if (isCredits_Playing)
		{
			//ret = StopBGM(&credits_BGM);
			isCredits_Playing = false;
		}
		break;
	case Game::options:
		break;
	case Game::help:
		break;
	case Game::loading:
		if (!isLoading_Playing)
		{
		//	loading_BGM.Play();
			isLoading_Playing = true;
		}
		if (isTitle_Playing)
		{
		//	ret = StopBGM(&title_BGM);
			isTitle_Playing = false;
		}
		break;
	case Game::tutorial:
	case Game::level:
		if (!isGame_Playing)
		{
			//game_BGM.Play();
			isGame_Playing = true;
		}
		if (isLoading_Playing)
		{
			//ret = StopBGM(&loading_BGM);
			isLoading_Playing = false;
		}
		break;
	case Game::pause:
		
		break;
	case Game::death:
		if (!isDeath_Playing)
		{
			//death_BGM.Play();
			isDeath_Playing = true;
		}
		if (isGame_Playing)
		{
			//ret = StopBGM(&game_BGM);
			isGame_Playing = false;
		}
		break;
	case Game::levelEnd:
		if (!isTitle_Playing)
		{
			//title_BGM.Play();
			isTitle_Playing = true;
		}
		if (isGame_Playing)
		{
			///ret = StopBGM(&game_BGM);
			isGame_Playing = false;
		}
		break;
	case Game::credits:
		if (!isCredits_Playing)
		{
		//	credits_BGM.Play();
			isCredits_Playing = true;
		}
		if (isTitle_Playing)
		{
			///ret = StopBGM(&title_BGM);
			isTitle_Playing = false;
		}
		break;
	default:
		return kErrAudioFailure;
		break;
	}

	return ret;
}

// Stop background track; BGM_1 = 1, BGM_2 = 2, BGA_1 = 3 
//uint32_t Game::StopBGM(codeEngine::AudioSource* source)
//{
//	if (nullptr == source)
//	{
//		return kErrAudioFailure;
//	}
//
//	source->Stop();
//
//	return kOK;
//}