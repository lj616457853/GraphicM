#ifndef RES_MANAGER_H
#define RES_MANAGER_H
#include <obshader.h>
#include <obtexture.h>
#include <filesystem.h>
#include <iostream>
#include <fstream>
#include <stb_image.h>
#include <sstream>


class ResDataManagger {
public:
	static std::map<std::string, OBTexture>     textureDatas;

	static std::map<std::string, ShaderData>    shaderDatas;


    static ShaderData    LoadShaderDt(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);


    // retrieves a stored sader
    static ShaderData    GetShaderDt(std::string name);

    // loads (and generates) a texture from file
    static OBTexture LoadTextureDt(const char* file, bool alpha, std::string name);


    // retrieves a stored texture
    static OBTexture GetTextureDt(std::string name);
    // properly de-allocates all loaded resources


private:

    static ShaderData    loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);

     
    // loads a single texture from file
    static OBTexture loadTextureFromFile(const char* file, bool alpha);

};

#endif // !RES_SHADER_MANAGER
