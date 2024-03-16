#ifndef OB_TEXTURE_H
#define OB_TEXTURE_H
#include <iostream>
#include <glad/glad.h>

class OBTexture {
public:
	int width, height, nrComponents;
	std::string name;
	unsigned char* imagedata;
	std::string path;
	std::string type;
	OBTexture();
	// generates texture from image data
	void Generate(unsigned int width, unsigned int height, unsigned char* data);

};




#endif // !OB_TEXTURE_H
