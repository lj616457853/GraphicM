#ifndef EGI_H
#define EGI_H
#include <glad/glad.h>
#include <model_manager.h>
#include <resource_manager.h>

#include <gl_command.h>


class EgineU {
public:

	unsigned int            Width, Height;

	GLCommandManager* command;

	EgineU(unsigned int width, unsigned int height);
	~EgineU();


	void Init();



	void Render();


};




#endif // !EGI_H
