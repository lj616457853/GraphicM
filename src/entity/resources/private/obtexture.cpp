#include <obtexture.h>


OBTexture::OBTexture() {
}

void OBTexture::Generate(unsigned int width, unsigned int height, unsigned char* data)
{

	this->width = width;
	this->height = height;
	this->imagedata = data;
}
