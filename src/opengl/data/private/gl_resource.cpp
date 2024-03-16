#include <gl_resource.h>


GLOBTexture2D::GLOBTexture2D(OBTexture* obtexture) :Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR) {

	glGenTextures(1, &this->ID);
	this->texture = obtexture;
	

	//Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
	this->Internal_Format = GL_RGB;
	this->Image_Format = GL_RGB;
	this->Wrap_S = GL_REPEAT;

	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, obtexture->width, obtexture->height, 0, this->Image_Format, GL_UNSIGNED_BYTE, obtexture->imagedata);
	// set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLOBTexture2D::Bind() const {
	glBindTexture(GL_TEXTURE_2D, this->ID);
}


std::map<std::string, GLAmpModel> GLAmpLModelManager::glAmpModelMap;


GLAmpModel GLAmpLModelManager::loadGLAmpModel(std::string name, std::string  modelDataName) {
	int idx = ModelManager::getModel(modelDataName);
	if (idx!=1) {
		return  nullptr;
	}
	APModel data = ModelManager::APModels[modelDataName];
	

	GLAmpModel ampModel(&data);

	glAmpModelMap[name] = ampModel;

	return ampModel;
}

GLAmpModel GLAmpLModelManager::getGLModel(std::string name) {
	return glAmpModelMap[name];
}



std::map<std::string, Shader> GLResManager::glShaderM;

std::map<std::string, GLOBTexture2D> GLResManager::glTextureM;


Shader GLResManager::getGLShader(std::string shaderName) {
	if (glShaderM.find(shaderName) != glShaderM.end()) {
		return glShaderM[shaderName];
	}
	ShaderData dt =ResDataManagger::GetShaderDt(shaderName);
	Shader glShader;
	glShader.Compile(dt.vsSahderData.c_str(), dt.fsShaderData.c_str(), nullptr);
	glShaderM[shaderName] = glShader;
	return glShader;
}


GLOBTexture2D GLResManager::getGLTexture(std::string textName) {
	if (glTextureM.find(textName) != glTextureM.end()) {
		return glTextureM[textName];
	}
	OBTexture tx=  ResDataManagger::GetTextureDt(textName);
	GLOBTexture2D texture(&tx);
	glTextureM[textName] = texture;
	return texture;

}

