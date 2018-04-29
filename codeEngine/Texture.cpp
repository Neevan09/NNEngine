#include "Texture.h"



Texture::Texture(const std::wstring& filePath, ID3D11Device * _device, ID3D11DeviceContext * _context)
{
	auto period = filePath.find_last_of(L".");
	auto extention = filePath.substr(period + 1, filePath.size());

	if (extention == L"dds")
	{
		//Load DDS texture
		DirectX::CreateDDSTextureFromFile(_device, filePath.c_str(), 0, &srv);
		CreateCubeSampler(_device);
		return;
	}

	// Load other texture
	DirectX::CreateWICTextureFromFile(_device, _context, filePath.c_str(), 0, &srv);
	CreateTextureSampler(_device);
}


Texture::~Texture()
{
	//Multiple deletes throws exception
	if (this->srv)
	{
		srv->Release();
		srv = 0;
	}
	if (this->sampler)
	{
		sampler->Release();
		sampler = 0;
	}
}

void Texture::CreateCubeSampler(ID3D11Device * _device)
{
	// Create a sampler state that holds options for sampling
	// The descriptions should always just be local variables
	D3D11_SAMPLER_DESC samplerDesc = {}; // The {} part zeros out the struct!
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX; // Setting this allows for mip maps to work! (if they exist)

											// Ask DirectX for the actual object
	_device->CreateSamplerState(&samplerDesc, &sampler);
}

void Texture::CreateTextureSampler(ID3D11Device * _device)
{
	D3D11_SAMPLER_DESC sampler_desc = {};
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // Tri-linear filtering
	sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampler_desc.MaxAnisotropy = 16;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	_device->CreateSamplerState(&sampler_desc, &sampler);
}

ID3D11ShaderResourceView * Texture::GetSRV() const
{
	return srv;
}
ID3D11SamplerState* Texture::GetSampler() const
{
	return sampler;
}