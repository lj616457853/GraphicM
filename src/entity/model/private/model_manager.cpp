#include <model_manager.h>


std::map<std::string, ObModel> ModelManager::OBModels;


std::map<std::string, APModel> ModelManager::APModels;


std::map<std::string, int>   ModelManager::loademap;




std::map<std::string, AmpAnimation> ModelManager::AnimationM;



std::map<std::string, Animator> ModelManager::Amtor;



 int ModelManager::loadModel(const char* file, string modelname) {
	 std::string path = FileSystem::getRoot() + "/resources/models/" + file;
	 std::string type = path.substr(path.find_last_of(".")+1);
	 bool gltf = type == "gltf";
	 if (gltf) {
		
		 return 0;
	 }
	 else {

		 // apmodel(path);
		 loademap[modelname] = 1;	
		 APModels[modelname] = *new APModel(path);
		 return 1;
	 }
	 
}

int ModelManager::getModel(std::string name) {
	return loademap[name];
}

void      ModelManager::Clear() {

}


AmpAnimation* ModelManager::getAmpAnimation(std::string modelsname) {
	if (AnimationM.find(modelsname) != AnimationM.end() ) {
	
		return &AnimationM[modelsname];
	}
	
	APModel am = ModelManager::APModels[modelsname];
	
	AmpAnimation ampam(am.modelpath,&am);

	AnimationM[modelsname] = ampam;
	return &ampam;
}


Animator* ModelManager::getAmpAnimator(std::string modelsname) {
	if (Amtor.find(modelsname)!=Amtor.end()) {
		return &Amtor[modelsname];
	}
	AmpAnimation animation = AnimationM[modelsname];
	
	Animator ator(&animation);

	Amtor[modelsname] = ator;
	return &ator;

}


