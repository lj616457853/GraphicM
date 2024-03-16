#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H
#include <glad/glad.h>
#include <obmodel.h>
#include <filesystem.h>
#include <map>
#include <string>




class ModelManager {
public:
	// resource storage
	static std::map<std::string, ObModel>    OBModels;

	static std::map<std::string, APModel>     APModels;


	static std::map<std::string, AmpAnimation> AnimationM;



	static std::map<std::string, Animator> Amtor;


	static std::map<std::string, int>     loademap;

	static int loadModel(const char* file,string modelname);

	static int getModel(std::string modelsname);


	static AmpAnimation* getAmpAnimation(std::string modelsname);

	static Animator* getAmpAnimator(std::string modelsname);

	static void      Clear();
private:
	ModelManager() {};
	//static ObModel loadModelFromFile(const char* file);
	

};


#endif // !MODEL_MANAGER_H
