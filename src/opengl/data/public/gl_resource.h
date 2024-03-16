#ifndef GL_RESOURCE_H
#define GL_RESOURCE_H
#include <glad/glad.h>
#include <model_manager.h>
#include <res_manager.h>
#include <stb_image.h>
#include <obmodel.h>
#include <learnopengl/camera.h>
#include <shaderg.h>
struct TextureM {

	unsigned int id;

	std::string type;

	std::string path;
};

class GLMesh {
public:
	APMesh* apMesh;

    ~GLMesh() {
       
    }
	
	std::vector<TextureM> textures;
	unsigned int VAO;

	GLMesh(APMesh* apMesh) {
		this->apMesh = apMesh;
        procesMesh(apMesh->textures);
        setupMesh();
	}



    void Draw(Shader& shader)
    {
        // bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to string
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to string
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to string

            // now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(apMesh->indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

private:
    // render data 
    unsigned int VBO, EBO;

    void procesMesh(std::vector<OBTexture> txs) {
        for (OBTexture  tex: txs) {
            TextureM tm;
            tm.id = generateTex(tex);
            tm.path = tex.path;
            tm.type = tex.type;
            textures.push_back(tm);
        }
    }

    int generateTex(OBTexture tex) {

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(tex.path.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << tex.path << std::endl;
            stbi_image_free(data);
        }

      
        return textureID;
    }

    // initializes all the buffer objects/arrays
    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, apMesh->vertices.size() * sizeof(APVertex), &apMesh->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, apMesh->indices.size() * sizeof(unsigned int), &apMesh->indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(APVertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(APVertex), (void*)offsetof(APVertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(APVertex), (void*)offsetof(APVertex, TexCoords));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(APVertex), (void*)offsetof(APVertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(APVertex), (void*)offsetof(APVertex, Bitangent));
        // ids
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(APVertex), (void*)offsetof(APVertex, m_BoneIDs));

        // weights
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(APVertex), (void*)offsetof(APVertex, m_Weights));
        glBindVertexArray(0);
    }


};


class GLAmpModel {
public :
    std::vector<GLMesh> meshs;
    bool gammaCorrection;
    APModel* model;
    GLAmpModel() {};
    
    GLAmpModel(APModel* model) {
        this->model = model;
        for (APMesh mesh : model->meshes) {
            process(&mesh);
        }
      
    }
    ~GLAmpModel() {
        
    }

    void process(APMesh * apMesh) {
        GLMesh mesh(apMesh);
        meshs.push_back(mesh);
    }

    void draw(Shader& shader) {
        for (GLMesh ms:meshs) {
            ms.Draw(shader);
        }
    }
};


class GLOBTexture2D {
public:

    OBTexture* texture;


    unsigned int ID;
    // texture Format
    unsigned int Internal_Format; // format of texture object
    unsigned int Image_Format; // format of loaded image
    // texture configuration
    unsigned int Wrap_S; // wrapping mode on S axis
    unsigned int Wrap_T; // wrapping mode on T axis
    unsigned int Filter_Min; // filtering mode if texture pixels < screen pixels
    unsigned int Filter_Max; // filtering mode if texture pixels > screen pixels

    GLOBTexture2D() {}
    GLOBTexture2D(OBTexture* obtexture);
    //void Initial(OBTexture* obtexture);

    void Bind() const;
};


class GLAmpLModelManager {
public:
    

    static std::map<std::string, GLAmpModel> glAmpModelMap;


    static GLAmpModel loadGLAmpModel(std::string name, std::string  modelDataName);

    static GLAmpModel getGLModel(std::string name);

    
private: 

    GLAmpLModelManager() {}


};


class GLResManager {
public:
    static std::map<std::string, Shader> glShaderM;

    static std::map<std::string, GLOBTexture2D> glTextureM;


    static Shader getGLShader(std::string shaderName);


    static GLOBTexture2D getGLTexture(std::string textName);


private:
    GLResManager() {
    
    }

};




class GLRenderer {
public:
    Shader shader;
    unsigned int quadVAO;

    GLRenderer(Shader shader) {
        this->shader = shader;
    }

    void initdata(float verticess[]) {
        // configure VAO/VBO
        unsigned int VBO;
        float vertices[] = {
            // pos      // tex
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
        };

        glGenVertexArrays(1, &this->quadVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(this->quadVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    };



    void draw(Camera* camera, GLOBTexture2D texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f)) {
        // prepare transformations
        this->shader.Use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
        model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

        model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

        this->shader.SetMatrix4("model", model);

        // render textured quad
        this->shader.SetVector3f("spriteColor", color);

        glActiveTexture(GL_TEXTURE0);
        texture.Bind();

        glBindVertexArray(this->quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);


    };



};


#endif // !GL_RESOURCE_H
