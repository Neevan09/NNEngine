#include "Renderer.h"

//-------------------------------------------------------------------
// Initialize Renderer
//-------------------------------------------------------------------
Renderer::Renderer(	ID3D11DepthStencilView * _depthStencilView,	ID3D11RenderTargetView *_backBufferRTV,	
					ID3D11DeviceContext *_context,	IDXGISwapChain *_swapChain,
					Camera *_camera)
{
	depthStencilView = _depthStencilView;
	backBufferRTV = _backBufferRTV;
	context = _context;
	swapChain = _swapChain;
	camera = _camera;

	viewMatrix = camera->GetViewMatrix();
}


Renderer::~Renderer()
{
}

//-------------------------------------------------------------------
// Draw a single game object
//-------------------------------------------------------------------
void Renderer::Draw(GameObject * gameObject, XMFLOAT4 color, bool onCanvas)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer* vBuff;
	ID3D11Buffer* iBuff;

	gameObject->GetMaterial()->GetPixelShader()->SetFloat3("CameraPosition", camera->GetPosition());
	gameObject->GetMaterial()->GetPixelShader()->SetFloat4("color", color);
	if (onCanvas) {
		gameObject->GetMaterial()->GetVertexShader()->SetInt("onCanvas", 1);
		gameObject->GetMaterial()->GetPixelShader()->SetInt("onCanvas", 1);
		gameObject->PrepareMaterial(viewMatrix, camera->GetProjectionMatrix());
	}
	else {
		gameObject->GetMaterial()->GetVertexShader()->SetInt("onCanvas", 0);
		gameObject->GetMaterial()->GetPixelShader()->SetInt("onCanvas", 0);
		gameObject->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix());
	}
	vBuff = gameObject->GetObjectMesh()->GetVertexBuffer();
	iBuff = gameObject->GetObjectMesh()->GetIndexBuffer();
	context->IASetVertexBuffers(0, 1, &vBuff, &stride, &offset);
	context->IASetIndexBuffer(iBuff, DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(
		gameObject->GetObjectMesh()->GetIndexCount(),   // The number of indices to use
		0,												// Offset to the first index we want to use
		0);												// Offset to add to each index when looking up vertices
}

void Renderer::Draw(GameObject * gameObject, XMFLOAT4 color) {
	Draw(gameObject, color, false);
}

void Renderer::Draw(GameObject * gameObject) {
	Draw(gameObject, XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), false);
}

//-------------------------------------------------------------------
// Clear the screen
//-------------------------------------------------------------------
void Renderer::Clear(const float color[4])
{
	// Clear the render target and depth buffer (erases what's on the screen)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
}

//-------------------------------------------------------------------
// Present the swap chain to the buffer for viewing
//-------------------------------------------------------------------
void Renderer::PresentSwapChain()
{
	swapChain->Present(0, 0);
}

//-------------------------------------------------------------------
// Render skybox
//-------------------------------------------------------------------
void Renderer::RenderSky(Mesh *skymesh, SimplePixelShader *skyBoxPS, SimpleVertexShader *skyBoxVS, ID3D11SamplerState* sampler,
ID3D11ShaderResourceView* skySRV,ID3D11RasterizerState* skyRastState,ID3D11DepthStencilState* skyDepthState)
{
	// Set buffers in the input assembler
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// Draw the sky
	ID3D11Buffer* skyVB = skymesh->GetVertexBuffer();
	ID3D11Buffer* skyIB = skymesh->GetIndexBuffer();
	context->IASetVertexBuffers(0, 1, &skyVB, &stride, &offset);
	context->IASetIndexBuffer(skyIB, DXGI_FORMAT_R32_UINT, 0);

	// Set up the sky shaders
	skyBoxVS->SetMatrix4x4("view", camera->GetViewMatrix());
	skyBoxVS->SetMatrix4x4("projection", camera->GetProjectionMatrix());
	skyBoxVS->CopyAllBufferData();
	skyBoxVS->SetShader();

	skyBoxPS->SetShaderResourceView("SkyTexture", skySRV);
	skyBoxPS->SetSamplerState("BasicSampler", sampler);
	skyBoxPS->SetShader();

	// Set up the render states necessary for the sky
	context->RSSetState(skyRastState);
	context->OMSetDepthStencilState(skyDepthState, 0);
	context->DrawIndexed(skymesh->GetIndexCount(), 0, 0);

	// When done rendering, reset any and all states for the next frame
	context->RSSetState(0);
	context->OMSetDepthStencilState(0, 0);
}

void Renderer::SetDepthStencilView(ID3D11DepthStencilView * _depthStencilView)
{
	depthStencilView = _depthStencilView;
}

void Renderer::SetBackBuffer(ID3D11RenderTargetView * _backBufferRTV)
{
	backBufferRTV = _backBufferRTV;
}

