#ifndef GL_COMMAND_H
#define GL_COMMAND_H
#include <glad/glad.h>
#include <rhi_center.h>
#include <gl_resource.h>
#include <res_manager.h>
#include <model_manager.h>
#include <gl_resource.h>
#include <GLFW/glfw3.h>


class GLCommandManager {
public:
	GLFWwindow* window = nullptr;

	RHIContext *context;

	
	static Camera * camera;
	// camera
	
	//float lastX = SCR_WIDTH / 2.0f;
	//float lastY = SCR_HEIGHT / 2.0f;
	static float lastX;
	static float lastY;


	static bool firstMouse;

	// timing
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;


	static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		// make sure the viewport matches the new window dimensions; note that width and 
	   // height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}


	static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
		float xposs = static_cast<float>(xpos);
		float yposs = static_cast<float>(ypos);

		if (firstMouse)
		{
			lastX = xposs;
			lastY = yposs;
			firstMouse = false;
		}

		float xoffset = xposs - lastX;
		float yoffset = lastY - yposs; // reversed since y-coordinates go from bottom to top

		lastX = xposs;
		lastY = yposs;

		camera->ProcessMouseMovement(xoffset, yoffset);
	}


	static  void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	
	}



	static void processInput(GLFWwindow* window) {


	}
	GLCommandManager(RHIContext *context);


	~GLCommandManager();

	static std::map<std::string,SceneView*> viewMap;


	static std::map<std::string, Scene*> sceneMap;


	static std::map<std::string, int> sceneIMap;

	static std::map<std::string, int> viewIMap;

	
	void processContext();


	void processScene(SceneView * view);

	void processSceneFR(SceneView* view);


	void processScenePost(SceneView* view);


	void processSceneDeffer(SceneView* view);


	void processSceneRender(SceneView* view);


	void processSceneAMPModel(SceneView* view);


	void processSceneGLTFModel(SceneView* view);



	void init();



	void prepare();



	void loop();



	void update(float dt);





};



#endif // !GL_COMMAND_H
