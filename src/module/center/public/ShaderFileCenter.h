#pragma   once  
#ifndef SHADER_FILE_C
#define SHADER_FILE_C
#include <string>

#include <filesystem.h>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iostream>


#include <map1.h>
#include <vector>


class ShaderFileManager {
private:
	
	static ShaderFileManager* sFileManager;


	HashTable<string, string>* shaderFileTable;

	ShaderFileManager(int size);

	vector<string> filepaths;
public:
	static ShaderFileManager* getShaderManger(int size = 100) {
		if (ShaderFileManager::sFileManager == nullptr) {
			ShaderFileManager::sFileManager = new ShaderFileManager(size);
		}
		return ShaderFileManager::sFileManager;
	}

	void traverseDir(const std::filesystem::path& root, int depth = 0);

	string ReplaceAll(std::string str, const std::string& from, const std::string& to);

	void findShaderPaths(string path);

	
	string fileLoad(string path);
	
	void addShaderFile(string path);


	string getShaderFile(string path);
	
	std::vector<string> getShaderFiles();

	void removeShader(string path);


	void readShader();

};





#endif // !SHADER_FILE_C
