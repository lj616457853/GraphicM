#include <gl_command.h>

std::map<std::string, SceneView*> GLCommandManager::viewMap;


std::map<std::string, Scene*> GLCommandManager::sceneMap;


std::map<std::string, int> GLCommandManager::sceneIMap;

std::map<std::string, int> GLCommandManager::viewIMap;


Camera* GLCommandManager::camera =nullptr;

float GLCommandManager::lastX = 0;
float GLCommandManager::lastY = 0;


bool GLCommandManager::firstMouse = true;


GLCommandManager::GLCommandManager(RHIContext* context){
	this->context = context;
	int ht = context->height;
	int wt = context->width;

	lastX = wt / 2.0f;
	lastY = ht / 2.0f;


	camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

}





void GLCommandManager::processContext() {
	std::vector<Scene*>  sences =  context->scene;

	for (Scene* sence: sences) {
		std::vector<SceneView*> views  = sence->views;
		 // 装备gl scene 场景
		for (SceneView* view:views) {
			processScene(view);
			viewMap[view->viewName] = view;

		}
		sceneMap[sence->sceneName] = sence;
	}

	std::vector<std::string> txNames = context->textureNames;
	for (std::string tx: txNames) {
		GLResManager::getGLTexture(tx);
	}
	std::vector<std::string>  shNames=  context->shaderNames;

	for (std::string sn: shNames) {
		GLResManager::getGLShader(sn);
	}
}

void GLCommandManager::processScene(SceneView* view) {
	
	RENDERTYPE renderType =view->rdType;
	switch (renderType)
	{
	case RENDERTYPE::RENDER:
		sceneIMap[view->renderName] = 0;
		processSceneFR(view);

		break;
	case RENDERTYPE::POSTRENDER:
		sceneIMap[view->renderName] = 1;
		processScenePost(view);
		break;
	case RENDERTYPE::DEEFERRENDER:
		sceneIMap[view->renderName] = 2;
		processSceneDeffer(view);
		break;
	default:
		break;
	}

}

GLCommandManager::~GLCommandManager() {
	if (window) {
		delete(window);
	}

	if (context) {
		delete(context);
	}

	if (camera) {
		delete(camera);
	}

}

void GLCommandManager::processSceneFR(SceneView* view) {
	VIEWTYPE vt =  view->type;

	switch (vt)
	{
	case VIEWTYPE::RENDERVIEW:
		sceneIMap[view->renderName] = 0;
		processSceneRender(view);

		break;
	case VIEWTYPE::AMPVIEW:
		sceneIMap[view->renderName] = 1;
		processSceneAMPModel(view);

		break;
	case VIEWTYPE::GLTFVIEW:

		sceneIMap[view->renderName] = 2;
		processSceneGLTFModel(view);

		break;
	default:
		break;
	}
}

void GLCommandManager::processSceneRender(SceneView* view) {

}


void GLCommandManager::processSceneAMPModel(SceneView* view) {
	std::string name = view->viewName;
	GLAmpLModelManager::loadGLAmpModel(name,name);
	std::string shaderName =  view->shadermap[view->renderName];
	GLResManager::getGLShader(shaderName);

}


void GLCommandManager::processSceneGLTFModel(SceneView* view) {


}




void GLCommandManager::processScenePost(SceneView* view) {


}


void GLCommandManager::processSceneDeffer(SceneView* view) {


}



void GLCommandManager::init() {
	//camera = new Camera();
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	window = glfwCreateWindow(context->width, context->height, "3DMODEL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// configure global opengl state
	// -----------------------------
		// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}



void GLCommandManager::prepare() {

	
}



void GLCommandManager::loop() {
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic'
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// input
		// -----
		processInput(window);

		// --------------------
		for (std::string sc:context->needRenderScene ) {
		
			//获取shader
			Scene* scene = sceneMap[sc];

			std::vector<SceneView*> views = scene->views;

			for (SceneView* view : views) {

				std::string modeName = view->viewName;
				AmpAnimation* act=  ModelManager::getAmpAnimation(modeName);
				Animator* atorn= new Animator(act);
				//Animator* ator = ModelManager::getAmpAnimator(modeName);
				//ator->UpdateAnimation(deltaTime);
				atorn->UpdateAnimation(deltaTime);
				std::string shaderName = view->shadermap[view->renderName];
				Shader shader = GLResManager::getGLShader(shaderName);
				shader.Use();
				// view/projection transformations
				glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)context->width / (float)context->height, 0.1f, 100.0f);
				glm::mat4 viewN = camera->GetViewMatrix();
				shader.SetMatrix4("projection", projection);
				shader.SetMatrix4("view", viewN);

				std::string name = view->renderName;

				auto transforms = atorn->GetFinalBoneMatrices();

				for (int i = 0; i < transforms.size(); ++i) {
					std::string de ="finalBonesMatrices[" + std::to_string(i) + "]";

					shader.SetMatrix4(de.c_str(), transforms[i]);
				}
					

				// render textured quad
				//shader.SetVector3f("spriteColor", view->Color);
				
				if (GLAmpLModelManager::glAmpModelMap.find(name)!=GLAmpLModelManager::glAmpModelMap.end()) {

					// render the loaded model
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f)); // translate it down so it's at the center of the scene
					model = glm::scale(model, glm::vec3(.5f, .5f, .5f));	// it's a bit too big for our scene, so scale it down
					shader.SetMatrix4("model", model);
					//ourModel.Draw(ourShader);


					GLAmpModel ampmodel = GLAmpLModelManager::glAmpModelMap[name];
					
					ampmodel.draw(shader);
				
				
				}
				
			}

			

			


		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


}



void GLCommandManager::update(float dt){
	
	
}

