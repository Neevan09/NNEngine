#include "Game.h"
#include "Vertex.h"
#include "WICTextureLoader.h"
#include "AudioEngine.h"
#include <functional>
#include <DDSTextureLoader.h>
// For the DirectX Math library
using namespace DirectX;

// Create toggle buttons here
KeyToggle ToggleEnter(VK_RETURN);
KeyToggle ToggleTab(VK_TAB);
KeyToggle ToggleShift(VK_SHIFT);

// -------------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// -------------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.\n");
#endif
	
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Release any (and all!) DirectX objects
	// we've made in the Game class
	
	//Delete Camera
	if (camera != nullptr)
		delete camera;
	//Delete Renderer
	if (renderer != nullptr)
		delete renderer;
	//Delete game objects
	for (auto gameobjects : gameObjects)
	{
		delete gameobjects;
	}

	//Resource manager is deleting the resources
	//Delete meshes
	//for (auto mesh : meshes)
	//{
		//delete mesh;
	//}

	//Delete materials
	for (auto material : materials)
	{
		delete material;
	}
	
	delete canvas;
	delete rm;
	// Delete audio resources
	audioEngine->ShutDown();

	// Skybox
	skyRastState->Release();
	skyDepthState->Release();
}

void Game::PlayObjectSound() 
{
	//std::cout << "Red button" << std::endl;
	gameObjects[1]->PlayObjectSound(0, 3);
}
void Game::StopObjectSound()
{
	//std::cout << "Red button" << std::endl;
	gameObjects[1]->StopObjectSound(0,3);
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Initialize audio
	audioEngine->Init();
	//Initialize the game
	LoadShaders();
	CreateCamera();
	CreateRenderer();
	CreateLights();

	rProps.push_back(ENVIRONMENT_UNDERWATER);
	rProps.push_back(ENVIRONMENT_GENERIC);
	rProps.push_back(ENVIRONMENT_PADDEDCELL);
	rProps.push_back(ENVIRONMENT_ROOM);
	rProps.push_back(ENVIRONMENT_BATHROOM);
	rProps.push_back(ENVIRONMENT_LIVINGROOM);
	rProps.push_back(ENVIRONMENT_STONEROOM);
	rProps.push_back(ENVIRONMENT_AUDITORIUM);
	rProps.push_back(ENVIRONMENT_CONCERTHALL);
	rProps.push_back(ENVIRONMENT_CAVE);
	rProps.push_back(ENVIRONMENT_ARENA);
	rProps.push_back(ENVIRONMENT_HANGER);
	rProps.push_back(ENVIRONMENT_CARPETTEDHALLWAY);
	rProps.push_back(ENVIRONMENT_HALLWAY);
	rProps.push_back(ENVIRONMENT_STONECORRIDOR);
	rProps.push_back(ENVIRONMENT_ALLEY);
	rProps.push_back(ENVIRONMENT_FOREST);
	rProps.push_back(ENVIRONMENT_CITY);
	rProps.push_back(ENVIRONMENT_MOUNTAINS);
	rProps.push_back(ENVIRONMENT_QUARRY);
	rProps.push_back(ENVIRONMENT_PLAIN);
	rProps.push_back(ENVIRONMENT_PARKINGLOT);
	rProps.push_back(ENVIRONMENT_SEWERPIPE);
	rProps.push_back(ENVIRONMENT_REVERB_OFF);

	auto prop = audioEngine->SetReverbProperties(rProps[0]);
	audioEngine->SetEnvironmentReverb(prop, AudioVector3{ 0,0,0 }, 0.0f, 1000.0f);
	audioEngine->ActivateReverb(false);
	audioEngine->Set3dListenerAndOrientation(AudioVector3{ camera->GetPosition().x,camera->GetPosition().y,camera->GetPosition().z },	// Listener position in on the camera
											 AudioVector3{ 0,0,1 }, AudioVector3{ 0,1,0 });												// Listener on th camera
	
	
	// Create objects in the game
	CreateGameObjects();

	// Create UI canvas
	CreateCanvas();

	// Create states for sky rendering
	D3D11_RASTERIZER_DESC rs = {};
	rs.CullMode = D3D11_CULL_FRONT;
	rs.FillMode = D3D11_FILL_SOLID;
	device->CreateRasterizerState(&rs, &skyRastState);

	D3D11_DEPTH_STENCIL_DESC ds = {};
	ds.DepthEnable = true;
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ds.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&ds, &skyDepthState);

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	// Initialize resource manager
	rm = new ResourceManager(device, context,audioEngine);
	rm->LoadFromConfig();
}

// ---------------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// ---------------------------------------------------------------
void Game::CreateCamera()
{
	camera = new Camera();
	camera->UpdateViewMatrix();
	camera->UpdateProjectionMatrix(width, height);

}

//---------------------------------------------------------------
// Initialize the renderer
//---------------------------------------------------------------
void Game::CreateRenderer()
{
	renderer = new Renderer(depthStencilView, backBufferRTV, context, swapChain, camera);
}

// ---------------------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// ---------------------------------------------------------------------
void Game::CreateGameObjects()
{
	// Load audio
	// Configuration::GetInstance().LoadSoundConfig(audioEngine);

	// Skybox
	t = rm->GetTextureByName(L"SunnyCubeMap.dds");
	skySRV = t->GetSRV();
	sampler = t->GetSampler();

	// Create materials
	materials.push_back(new Material(rm->GetVShaderByName(L"VertexShader1.cso"), rm->GetPShaderByName(L"PixelShader1.cso"), rm->GetTextureByName(L"texture01.jpg")));
	materials.push_back(new Material(rm->GetVShaderByName(L"VertexShader1.cso"), rm->GetPShaderByName(L"PixelShader1.cso"), rm->GetTextureByName(L"texture01.png")));
	materials.push_back(new Material(rm->GetVShaderByName(L"VertexShader1.cso"), rm->GetPShaderByName(L"PixelShader1.cso"), rm->GetTextureByName(L"steel.jpg")));
	materials.push_back(new Material(rm->GetVShaderByName(L"VertexShader1.cso"), rm->GetPShaderByName(L"PixelShader1.cso"), rm->GetTextureByName(L"heart.png")));
	
	meshes.push_back(rm->GetMeshByName("cube.obj"));
	meshes.push_back(rm->GetMeshByName("sphere.obj"));
	meshes.push_back(rm->GetMeshByName("cone.obj"));
	meshes.push_back(rm->GetMeshByName("helix.obj"));
	meshes.push_back(rm->GetMeshByName("torus.obj"));
	
	// Sound to be attached to an object
	std::vector<std::string> sound;
	sound.push_back("../../Assets/Sounds/drumloop.wav");

	// Initialize game objects
	gameObjects.push_back(new GameObject(meshes[0], materials[0]));
	gameObjects.push_back(new GameObject(meshes[1], materials[1], sound, audioEngine));
	gameObjects.push_back(new GameObject(meshes[2], materials[1]));
	gameObjects.push_back(new GameObject(meshes[3], materials[0]));
	gameObjects.push_back(new GameObject(meshes[4], materials[0]));

	// Position game objects
	gameObjects[1]->SetObjectPosition(XMFLOAT3(-1.5, 0, 0));
	gameObjects[1]->UpdateWorldMatrix();

	gameObjects[2]->SetObjectPosition(XMFLOAT3(1.5, 0, 0));
	gameObjects[2]->UpdateWorldMatrix();

	gameObjects[3]->SetObjectPosition(XMFLOAT3(3, 0, 0));
	gameObjects[3]->UpdateWorldMatrix();

	gameObjects[4]->SetObjectPosition(XMFLOAT3(-3, 0, 0));
	gameObjects[4]->UpdateWorldMatrix();

}

// -------------------------------------------------------------------
// Create lights in the scene and assigns it to the appropriate shader
// -------------------------------------------------------------------
void Game::CreateLights()
{
	// Initializing lights in the scene
	dLight1.ambientColor = XMFLOAT4(0.1, 0.1, 0.1, 1.0);
	dLight1.diffuseColor = XMFLOAT4(1, 1, 1, 1);
	dLight1.direction = XMFLOAT3(-1, 1, 0);

	dLight2.ambientColor = XMFLOAT4(0.1, 0.1, 0.1, 1.0);
	dLight2.diffuseColor = XMFLOAT4(0, 0.2, 0.3, 1);
	dLight2.direction = XMFLOAT3(1, -1, 0);

	pLight.color = XMFLOAT4(0.3, 0.3, 0, 1);
	pLight.position = XMFLOAT3(0, 1, 0);

	// Setting the lights to the shader
	rm->GetPShaderByName(L"PixelShader1.cso")->SetData(
		"dLight1",  // The name of the (eventual) variable in the shader
		&dLight1,   // The address of the data to copy
		sizeof(DirectionalLight)); // The size of the data to copy

	rm->GetPShaderByName(L"PixelShader1.cso")->SetData(
		"dLight2",  // The name of the (eventual) variable in the shader
		&dLight2,   // The address of the data to copy
		sizeof(DirectionalLight)); // The size of the data to copy

	rm->GetPShaderByName(L"PixelShader1.cso")->SetData(
		"pLight",  // The name of the (eventual) variable in the shader
		&pLight,   // The address of the data to copy
		sizeof(PointLight)); // The size of the data to copy
	
}

int id = 0;
int fileNumber = 0;
int env = 0;
// -------------------------------------------------------------
// Function to handle sound playback in the game
// -------------------------------------------------------------
void Game::PlaySounds()
{
	if (ToggleTab && Configuration::GetInstance().GetSoundFileCount() != 0)
	{
		audioEngine->StopAllChannels();		// Stop all channels without any fade
		id = audioEngine->PlaySounds(Configuration::GetInstance().GetSoundFileList()[fileNumber], AudioVector3{ 0, 0, 0 }, audioEngine->VolumeToDecibal(1.0f), 3);
		if ((fileNumber + 1) == Configuration::GetInstance().GetSoundFileCount())	// loop if last sound is played
			fileNumber = 0;
		else
			fileNumber++;
		audioEngine->SetLowPassGain(id, 0);
		std::cout << audioEngine->GetLowPassGain(id) << "\n";
	}
	if (KeyInput::IsKeyDown(KEY_O))
	{
		audioEngine->StopChannel(id,5);		// fade out to zero volume in 5 seconds
		id--;
	}
	if (ToggleEnter)
	{
		audioEngine->ActivateReverb(false);
		env++;
		if (env >= rProps.size())
		{
			env = 0;
		}
		std::cout << env << std::endl;
		auto prop = audioEngine->SetReverbProperties(rProps[env]);
		audioEngine->SetEnvironmentReverb(prop, AudioVector3{ 0,0,0 }, 0.0f, 1000.0f);
		audioEngine->ActivateReverb(true);
	}
}

//----------------------------------------------------------
// Create UI canvas and UI elements on the canvas
//----------------------------------------------------------
void Game::CreateCanvas() {

	canvas = new UICanvas(device, context, rm->GetVShaderByName(L"VertexShader1.cso"), rm->GetPShaderByName(L"PixelShader1.cso"));
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 0.5f, 0.0f, 1.0f);
	XMFLOAT4 gray = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

	UIButton *playButton = canvas->CreateButton(2.5f, -1.625f, 0.5f, 0.5f,materials[2], L"Play");
	UIButton *stopButton = canvas->CreateButton(3.0f, -1.625f, 0.5f, 0.5f, materials[2], L"Stop", red);

	//Heart
	canvas->CreateObject(3.0f, 1.5f, 0.4f, 0.4f, materials[3], XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0F));
	
	playButton->AssignFunc([&] { PlayObjectSound(); });	// Calls PlayObjectSound when play button is clicked
	stopButton->AssignFunc([&] { StopObjectSound(); });	// Calls StopObjectSound when stop button is clicked

	objLoader.LoadObjectsFromFile("../../Assets/UIObjects.txt", (*canvas), materials[0]);
}

// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// -------------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();
	renderer->SetDepthStencilView(depthStencilView);
	renderer->SetBackBuffer(backBufferRTV);
	camera->UpdateProjectionMatrix(width, height);
}

//AudioEngine audioEngine;
// --------------------------------------------------------
// Update game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	camera->UpdateFromInput(deltaTime);
	if (!stopAnimation)
	{
		float sinTime = sin(totalTime * 2);

		gameObjects[0]->Rotate(0,deltaTime,0);
	}

	// Move game object
	if (KeyInput::IsKeyDown(VK_UP))
	{
		gameObjects[1]->Move(0, 10 * deltaTime, 0);
	}
	if (KeyInput::IsKeyDown(VK_DOWN))
	{
		gameObjects[1]->Move(0, -10 * deltaTime, 0);
	}
	if (KeyInput::IsKeyDown(VK_RIGHT))
	{
		gameObjects[1]->Move(10 * deltaTime, 0, 0);
	}
	if (KeyInput::IsKeyDown(VK_LEFT))
	{
		gameObjects[1]->Move(-10 * deltaTime,0, 0);
	}

	// Quit if the escape key is pressed
	if (KeyInput::IsKeyDown(VK_ESCAPE))
		Quit();
	if (KeyInput::WasKeyReleased('I'))
		stopAnimation = !stopAnimation;
	/*if (ToggleEnter)
		stopAnimation = !stopAnimation;*/

	//Canvas and Debug Window interactions
	canvas->Update(deltaTime, totalTime);
	canvas->GetKeyInputString(KeyInput::GetKeyPressedWString());

	if (KeyInput::WasKeyPressed(VK_RETURN)) {
		canvas->EnterPressed();
	}
	if (KeyInput::WasKeyPressed(VK_BACK)) {
		canvas->BackspacePressed();
	}
	if (KeyInput::WasKeyPressed(KEY_GRAVE)) {
		canvas->GravePressed();
	}

	//Audio engine interactions
	audioEngine->Set3dListenerAndOrientation(AudioVector3{ camera->GetPosition().x,camera->GetPosition().y,camera->GetPosition().z },		// Listener at camera position
											 AudioVector3{ camera->GetDirection().x,camera->GetDirection().y,camera->GetDirection().z },	// Listener forward direction = camera's forward direction
											 AudioVector3{ camera->GetUp().x,camera->GetUp().y,camera->GetUp().z });																		// Listener Up direction
	audioEngine->Update();

	//Without this, this was causing problems
	KeyInput::ResetKeys();
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };
	renderer->Clear(color);

	// Draw each object
	for (auto gameObject : gameObjects)
	{
		renderer->Draw(gameObject);
	}
	
	// Draw Sky
	renderer->RenderSky(meshes[0], rm->GetPShaderByName(L"SkyPS.cso"), rm->GetVShaderByName(L"SkyVS.cso"), sampler, skySRV, skyRastState, skyDepthState);

	// Draw Canvas
	canvas->Draw(renderer);
	canvas->DrawSprites();

	// Present to screen
	renderer->PresentSwapChain();
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
	canvas->OnMouseDown(x, y);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
	canvas->OnMouseUp();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	if (buttonState & 0x0001) 
	{  
		camera->SetRotationY( y - prevMousePos.y );
		camera->SetRotationX( x - prevMousePos.x );
		camera->Rotate();
		//std::cout << "\n" << camera->GetDirection().x << " " << camera->GetDirection().y << " " << camera->GetDirection().z ;
	}
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	canvas->OnMouseMove(x, y, camera->GetProjectionMatrix(), camera->GetViewMatrix());
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion