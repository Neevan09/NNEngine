#include "Configuration.h"
#include <sstream>

//Configuration::Configuration()
//{
//	//Instance = new Configuration();
//	//soundFileCount = 0;
//}

Configuration::~Configuration()
{
}

void Configuration::LoadConfig()
{
	std::string filename;

	std::ifstream configFile("../../Assets/config.txt");
	std::string line;

	if (configFile.is_open())
	{
		while (std::getline(configFile, line))
		{
			std::istringstream sin(line.substr(line.find("=") + 1));
			if (line.find("Texture") != -1)
			{
				sin >> filename;
				textureFileList.push_back(filename);
			}
			if (line.find("Mesh") != -1)
			{
				sin >> filename;
				meshFileList.push_back(filename);
			}
			if (line.find("VShader") != -1)
			{
				sin >> filename;
				vshaderFileList.push_back(filename);
			}
			if (line.find("PShader") != -1)
			{
				sin >> filename;
				pshaderFileList.push_back(filename);
			}
			if (line.find("Sound") != -1)
			{
				sin >> filename;
				soundFileList.push_back(filename);
			}
		}
		textureFileCount	= textureFileList.size();
		meshFileCount		= meshFileList.size();
		vshaderFileCount	= vshaderFileList.size();
		pshaderFileCount	= pshaderFileList.size();
		soundFileCount		= soundFileList.size();
	}
	else
	{
		std::cout << "\n No config file found.\n";
		//return false;
	}
}

//-------------------------------------------------------------------
// Read in texture files to be loaded
//-------------------------------------------------------------------
void Configuration::LoadTextureConfig()
{
	std::string filename;

	std::ifstream configFile("../../Assets/Texture/textureConfig.txt");
	std::string line;

	if (configFile.is_open())
	{
		while (std::getline(configFile, line))
		{
			std::istringstream sin(line.substr(line.find("=") + 1));
			if (line.find("Texture") != -1)
			{
				sin >> filename;
				textureFileList.push_back(filename);
			}
		}
		textureFileCount = textureFileList.size();
	}
	else
	{
		std::cout << "\n No config file found.\n";
		//return false;
	}
}

//-------------------------------------------------------------------
// Read in mesh files to be loaded
//-------------------------------------------------------------------
void Configuration::LoadMeshConfig()
{
	std::string filename;

	std::ifstream configFile("../../Assets/Models/meshConfig.txt");
	std::string line;

	if (configFile.is_open())
	{
		while (std::getline(configFile, line))
		{
			std::istringstream sin(line.substr(line.find("=") + 1));
			if (line.find("Mesh") != -1)
			{
				sin >> filename;
				meshFileList.push_back(filename);
			}
		}
		meshFileCount = meshFileList.size();
	}
	else
	{
		std::cout << "\n No config file found.\n";
		//return false;
	}
}

//-------------------------------------------------------------------
// Load Sounds from a config file to a cache
//-------------------------------------------------------------------
void Configuration::LoadSoundConfig(AudioEngine *audioEngine)
{
	std::string filename;

	std::ifstream configFile("../../Assets/Sounds/soundConfig.txt");
	std::string line;

	if (configFile.is_open())
	{
		while (std::getline(configFile, line))
		{
			std::istringstream sin(line.substr(line.find("=") + 1));
			if (line.find("Sound") != -1)
			{
				sin >> filename;
				soundFileList.push_back(filename);
			}
		}
		soundFileCount = soundFileList.size();
		for (auto file : soundFileList)
		{
			audioEngine->LoadSound(file, true, true);
		}
	}
	else
	{
		std::cout << "\n No config file found.\n";
		//return false;
	}

}

//-------------------------------------------------------------------
// Return the loaded sound file list
//-------------------------------------------------------------------
std::vector<std::string> Configuration::GetSoundFileList()
{
	return soundFileList;
}

//-------------------------------------------------------------------
// Return the number of sound files in the config file
//-------------------------------------------------------------------
int Configuration::GetSoundFileCount()
{
	return soundFileCount;
}

//-------------------------------------------------------------------
// Return the loaded mesh file list
//-------------------------------------------------------------------
std::vector<std::string> Configuration::GetMeshFileList()
{
	return meshFileList;
}

//-------------------------------------------------------------------
// Return the number of mesh files in the config file
//-------------------------------------------------------------------
int Configuration::GetMeshFileCount()
{
	return meshFileCount;
}

//-------------------------------------------------------------------
// Return the loaded texture file list
//-------------------------------------------------------------------
std::vector<std::string> Configuration::GetTextureFileList()
{
	return textureFileList;
}

//-------------------------------------------------------------------
// Return the number of texture files in the config file
//-------------------------------------------------------------------
int Configuration::GetTextureFileCount()
{
	return textureFileCount;
}

//-------------------------------------------------------------------
// Return the loaded vertex shader file list
//-------------------------------------------------------------------
std::vector<std::string> Configuration::GetVShaderFileList()
{
	return vshaderFileList;
}

//-------------------------------------------------------------------
// Return the number of vertex shader files in the config file
//-------------------------------------------------------------------
int Configuration::GetVShaderFileCount()
{
	return vshaderFileCount;
}

//-------------------------------------------------------------------
// Return the loaded pixel shader file list
//-------------------------------------------------------------------
std::vector<std::string> Configuration::GetPShaderFileList()
{
	return pshaderFileList;
}

//-------------------------------------------------------------------
// Return the number of pixel shader files in the config file
//-------------------------------------------------------------------
int Configuration::GetPShaderFileCount()
{
	return pshaderFileCount;
}