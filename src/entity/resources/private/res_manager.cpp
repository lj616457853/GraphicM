#include <res_manager.h>

std::map<std::string, OBTexture>     ResDataManagger::textureDatas;

 std::map<std::string, ShaderData>    ResDataManagger::shaderDatas;

 ShaderData    ResDataManagger::LoadShaderDt(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name) {
    
     ShaderData  da =loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
     shaderDatas[name] = da;
     return shaderDatas[name];
 
 }


    // retrieves a stored sader
 ShaderData    ResDataManagger::GetShaderDt(std::string name) {
     return shaderDatas[name];
 }

    // loads (and generates) a texture from file
 OBTexture  ResDataManagger::LoadTextureDt(const char* file, bool alpha, std::string name) {
     textureDatas[name] = loadTextureFromFile(file, alpha);
     return textureDatas[name];
 
 }


    // retrieves a stored texture
 OBTexture ResDataManagger::GetTextureDt(std::string name) {
     return textureDatas[name];
 
 }
    // properly de-allocates all loaded resources


 ShaderData ResDataManagger::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile) {
     // 1. retrieve the vertex/fragment source code from filePath
     std::string vertexCode;
     std::string fragmentCode;
     std::string geometryCode;
     try
     {
         std::string vspath = FileSystem::getShaderPath(vShaderFile);
         std::string fspath = FileSystem::getShaderPath(fShaderFile);
         // open files
         std::ifstream vertexShaderFile(vspath);
         std::ifstream fragmentShaderFile(fspath);
         std::stringstream vShaderStream, fShaderStream;
         // read file's buffer contents into streams
         vShaderStream << vertexShaderFile.rdbuf();
         fShaderStream << fragmentShaderFile.rdbuf();
         // close file handlers
         vertexShaderFile.close();
         fragmentShaderFile.close();
         // convert stream into string
         vertexCode = vShaderStream.str();
         fragmentCode = fShaderStream.str();
         // if geometry shader path is present, also load a geometry shader
         if (gShaderFile != nullptr)
         {
             std::string gspath = FileSystem::getShaderPath(gShaderFile);
             std::ifstream geometryShaderFile(gspath);
             std::stringstream gShaderStream;
             gShaderStream << geometryShaderFile.rdbuf();
             geometryShaderFile.close();
             geometryCode = gShaderStream.str();
         }
     }
     catch (std::exception e)
     {
         std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
     }
    // const char* vShaderCode = vertexCode.c_str();
     //const char* fShaderCode = fragmentCode.c_str();
     //const char* gShaderCode = geometryCode.c_str();
     // 2. now create shader object from source code
     ShaderData shaderData;
     shaderData.vsSahderData = vertexCode;
     shaderData.fsShaderData = fragmentCode;
     if (gShaderFile!=nullptr) {
         shaderData.gsSahderData = geometryCode;
     }
    
     return shaderData;
 }


    // loads a single texture from file
 OBTexture ResDataManagger::loadTextureFromFile(const char* file, bool alpha) {
     // create texture object
     OBTexture texture;
     std::string path = FileSystem::getRoot() + "/resources/textures/" + file;
     // load image
     int width, height, nrChannels;
     unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
     // now generate texture
     texture.nrComponents = nrChannels;
     texture.Generate(width, height, data);
     // and finally free image data
    // stbi_image_free(data);
     return texture;
 
 }