#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Mesh.h"
#include "GameObject.h"
#include "Camera.h"
#include "Lights.h"
#include <vector>
#include "Renderer.h"
#include "UICanvas.h"
#include "UIObjectLoader.h"
#include "UserInputHandler.h"
#include "AudioEngine.h"
#include "Configuration.h"
#include "ResourceManager.h"
class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

	// Inorder to stop aniamtion in update
	bool stopAnimation = false;

	//For loading objects into the canvas from an external file
	UIObjectLoader objLoader;
	
	// Components of the scene
	Renderer *renderer;
	Camera *camera;
	std::vector<Mesh*> meshes;
	std::vector<GameObject*> gameObjects;
	std::vector<Material*> materials;
	UICanvas *canvas;

	// Lights in the scene
	DirectionalLight dLight1, dLight2;
	PointLight pLight;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	// Audio Engine
	AudioEngine *audioEngine;

	// Skybox
	ID3D11SamplerState* sampler;
	ID3D11ShaderResourceView* skySRV;
	ID3D11RasterizerState* skyRastState;
	ID3D11DepthStencilState* skyDepthState;
	
	void LoadShaders();
	void CreateCamera();
	void CreateGameObjects();
	void CreateRenderer();
	void CreateLights();
	void CreateCanvas();
	void PlaySounds();
	void PlayObjectSound();
	void StopObjectSound();

	vector<ReverbProperties> rProps;
	ResourceManager *rm;
	Texture *t;
};

