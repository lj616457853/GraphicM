#ifndef SHDER_CENTER
#define SHDER_CENTER

#include <ShaderFileCenter.h>
#include <ShaderMap.h>

class ShaderCenter
{
	
private:
	static ShaderCenter*  shaderCenter;
	
	int asid;
	
	std::vector<ShaderPSO*> psos;
		
	HashTable<string, ShaderPSO*>* shaderByPath;

	HashTable<int, ShaderPSO*>* shaderById;
	
	HashTable<string, int>* idByPath;

	ShaderCenter();

public:
	void filesToPso();
	
	int getShaderId(string path);

	ShaderPSO* getShaderPsoById(int id);

	ShaderPSO* getShaderPsoByPath(string path);


	string hadlepath(string pathsoure);

	static ShaderCenter* getShaderCenter() {
		if (shaderCenter==nullptr) {
			shaderCenter = new ShaderCenter();
			
		}
		return shaderCenter;
	}

};







#endif // !SHDER_CENTER
