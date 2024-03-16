#include <egi.h>


EgineU::EgineU(unsigned int width, unsigned int height) :Width(width),Height(height) {

}
EgineU::~EgineU() {
	glfwTerminate();
}


void EgineU::Init() {
	ModelManager::loadModel("vampire/dancing_vampire.dae","avatar");
	ModelManager::getAmpAnimation("avatar");
	ModelManager::getAmpAnimator("avatar");
	
	ResDataManagger::LoadShaderDt("vampire/anim_model.vs","vampire/anim_model.fs",nullptr,"ashader");

	

	SceneView* view = new SceneView("avatar", {0,0}, {1,1});
	
	view->setSahder("avatar","ashader");
	view->type = VIEWTYPE::AMPVIEW;

	RHIContext* context = new RHIContext(Width,Height);
	
	context->needRenderScene.push_back("avc");
	Scene* scene = new Scene();
	scene->sceneName = "avc";
	scene->views.push_back(view);
	context->scene.push_back(scene);

	command = new GLCommandManager(context);

	command->init();
	command->processContext();

	
}



void EgineU::Render() {
	command->loop();

}

