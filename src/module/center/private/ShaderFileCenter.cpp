#include <ShaderFileCenter.h>


ShaderFileManager* ShaderFileManager::sFileManager = nullptr;
ShaderFileManager::ShaderFileManager(int size) {
	HashTable<string, string> table(size);
	shaderFileTable = new HashTable<string, string>(size);
	string basepath = FileSystem::getRoot()+"/resources/shader/glsl" ;
	findShaderPaths(basepath);
    readShader();
}
void ShaderFileManager::traverseDir(const std::filesystem::path& root, int depth ) {
    try {
        if (std::filesystem::is_directory(root) && std::filesystem::exists(root)) {
            for (const auto& entry : std::filesystem::directory_iterator(root)) {
                for (int i = 0; i < depth; i++) {
                    std::cout << " ";
                }
                std::cout << entry.path().filename() << std::endl;

                if (std::filesystem::is_directory(entry)) {
                    traverseDir(entry, depth + 1); // µÝ¹é±éÀú×ÓÄ¿Â¼
                }
                else {
                
                    filepaths.push_back(ReplaceAll(entry.path().string(), "\\", "/"));
                
                }
            }
        }
        else {
            std::cout << "root is not directory or exists " << std::endl;
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "exception:" << ex.what() << std::endl;
    }
}
string ShaderFileManager::ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

void ShaderFileManager::findShaderPaths(string path) {
	std::cout << "shaderfile" << std::endl;
    std::filesystem::path root(path);
    traverseDir(path);
   
}

std::vector<string> ShaderFileManager::getShaderFiles() {
    return filepaths;
}

string ShaderFileManager::fileLoad(string path) {
    std::string shaderCode;

    std::ifstream vShaderFile;
    
   
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
   
    try
    {
        // open files
        vShaderFile.open(path);
      
        std::stringstream vShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
      
        // close file handlers
        vShaderFile.close();
      
        // convert stream into string
        shaderCode = vShaderStream.str();
       
      
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    return shaderCode;
}

void ShaderFileManager::readShader() {
    vector<string>::iterator path;

    for (path = filepaths.begin(); path != filepaths.end(); path++) {
        if (shaderFileTable->contains(*path)) {
            continue;
        }
        string code = fileLoad(*path);

        shaderFileTable->add(*path,code);
    
    }

}

void ShaderFileManager::addShaderFile(string path) {
    if (!shaderFileTable->contains(path)) {
        shaderFileTable->add(path, fileLoad(path));
    }
  
}


string ShaderFileManager::getShaderFile(string path) {
    if (shaderFileTable->contains(path)) {
        return  *shaderFileTable->get(path);
    }
    string code = fileLoad(path);
    shaderFileTable->add(path,code);
	return  code;
}


void ShaderFileManager::removeShader(string path) {
    if (shaderFileTable->contains(path)) {
        shaderFileTable->remove(path);
    }
}
