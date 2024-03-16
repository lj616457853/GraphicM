#ifndef RHI_CENTER_H
#define RHI_CENTER_H
#include <baseobj.h>


class RHIContext {
public:
	int width, height;

	//…Ë÷√Cmaera–≈œ¢

	std::vector<Scene*> scene;

	std::vector<std::string> needRenderScene;

	//std::vector<std::string> renderNames;

	std::vector<std::string> shaderNames;
	
	std::vector<std::string> textureNames;


	RHIContext(int width,int height):width(width),height(height) {}

};


class RHICommandManager {
public:

	RHIContext * Context;


	RHICommandManager(RHIContext* context) :Context(context) {
		
	
	};

	void init() {};



	void prepare() {};



	void loop() {};





	void update(float dt) {};




};



#endif // !RHI_CENTER_H
