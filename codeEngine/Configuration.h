#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "AudioEngine.h"
class Configuration
{
private:
	std::vector<std::string> soundFileList;
	std::vector<std::string> meshFileList;
	std::vector<std::string> textureFileList;
	std::vector<std::string> vshaderFileList;
	std::vector<std::string> pshaderFileList;

	unsigned __int64 soundFileCount;
	unsigned __int64 meshFileCount;
	unsigned __int64 textureFileCount;
	unsigned __int64 vshaderFileCount;
	unsigned __int64 pshaderFileCount;
	Configuration() {};
public:
	Configuration(Configuration const&) = delete;
	void operator=(Configuration const&) = delete;
	static Configuration& GetInstance()
	{
		static Configuration instance;
		return instance;
	}
	~Configuration();
	void LoadConfig();
	void LoadSoundConfig(AudioEngine *audioEngine);
	void LoadMeshConfig();
	void LoadTextureConfig();
	
	std::vector<std::string> GetSoundFileList();
	std::vector<std::string> GetMeshFileList();
	std::vector<std::string> GetTextureFileList();
	std::vector<std::string> GetVShaderFileList();
	std::vector<std::string> GetPShaderFileList();

	int GetMeshFileCount();
	int GetSoundFileCount();
	int GetTextureFileCount();
	int GetVShaderFileCount();
	int GetPShaderFileCount();
};

