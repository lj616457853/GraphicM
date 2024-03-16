#ifndef BASE_OBJ_H
#define BASE_OBJ_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <map>

enum  class VIEWTYPE
{
    RENDERVIEW,
    AMPVIEW,
    GLTFVIEW
};


enum class RENDERTYPE {
    RENDER,
    POSTRENDER,
    DEEFERRENDER
};;


class SceneView{
public:
    glm::vec2   Position, Size, Velocity;
    glm::vec3   Color;
    float       Rotation;
    
    VIEWTYPE type= VIEWTYPE::RENDERVIEW;

    RENDERTYPE rdType = RENDERTYPE::RENDER;
   
    std::string renderName;


    std::string viewName;
    
    std::map<std::string, std::string> shadermap;


    SceneView(std::string name, glm::vec2 pos, glm::vec2 size, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f)) :
        Position(pos), Size(size), Color(color), Velocity(velocity), Rotation(0), renderName(name), viewName(name)
    {
        
    };


    void setSahder(std::string renderName,std::string shaderName) {
        shadermap[renderName] = shaderName;
    }

    void update(glm::vec2   Position, glm::vec2   Size,glm::vec2   Velocity, float Rotation) {
        this->Position = Position;
        this->Size = Size;
        this->Velocity = Velocity;
        
        this->Rotation = Rotation;
    };
    void update(glm::vec2   Position, glm::vec2   Size, glm::vec2   Velocity) {
        this->Position = Position;
        this->Size = Size;
        this->Velocity = Velocity;
    };

    void update(glm::vec2   Position, glm::vec2   Size) {
        this->Position = Position;
        this->Size = Size;
    };



};


class Scene {
public:
    std::string sceneName;

    std::vector<SceneView *> views;

    


};


#endif // !BASSE_OBJ_H
