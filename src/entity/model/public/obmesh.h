#ifndef OB_MESH_H
#define OB_MESH_H
#include <glad/glad.h>

#include<assimp/quaternion.h>
#include<assimp/vector3.h>
#include<assimp/matrix4x4.h>
#include<glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <assimp/scene.h>
#include <list>
#define GLM_ENABLE_EXPERIMENTAL

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <map>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <obtexture.h>

#define MAX_BONE_INFLUENCE 4
using namespace std;


struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotation
{
	glm::quat orientation;
	float timeStamp;
};

struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};


struct BoneInfo
{
	/*id is index in finalBoneMatrices*/
	int id;

	/*offset matrix transforms vertex from model space to bone space*/
	glm::mat4 offset;

};

class AssimpGLMHelpers
{
public:

	static inline glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
	{
		glm::mat4 to;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}

	static inline glm::vec3 GetGLMVec(const aiVector3D& vec)
	{
		return glm::vec3(vec.x, vec.y, vec.z);
	}

	static inline glm::quat GetGLMQuat(const aiQuaternion& pOrientation)
	{
		return glm::quat(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
	}
};

struct OBPrimitive {
public:
	uint32_t firstIndex;
	uint32_t indexCount;
	uint32_t firstVertex;
	uint32_t vertexCount;
	std::vector<OBTexture> textures;
	struct Dimensions {
		glm::vec3 min = glm::vec3(FLT_MAX);
		glm::vec3 max = glm::vec3(-FLT_MAX);
		glm::vec3 size;
		glm::vec3 center;
		float radius;
	} dimensions;

	void setDimensions(glm::vec3 min, glm::vec3 max);
};

struct OBVertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec4 color;
	glm::vec4 joint0;
	glm::vec4 weight0;
	glm::vec4 tangent;
	
};
struct APVertex
{
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};

struct APMesh {
	// mesh Data
	vector<APVertex>       vertices;
	vector<unsigned int> indices;
	vector<OBTexture>      textures;

};
struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

struct ObNode {
	ObNode* parent;
	uint32_t index;
	std::vector<ObNode*> children;
	glm::mat4 matrix;
	std::string name;

	int32_t skinIndex = -1;
	glm::vec3 translation{};
	glm::vec3 scale{ 1.0f };
	glm::quat rotation{};
	glm::mat4 localMatrix();
	glm::mat4 getMatrix();
	void update();
	ObNode() {};


};
struct ObSkin {

	std::string name;
	ObNode* skeletonRoot = nullptr;
	std::vector<glm::mat4> inverseBindMatrices;
	std::vector<ObNode*> joints;
};


struct ObMesh {
	std::vector<OBPrimitive*> primitives;
	std::string name;
	struct UniformBlock {
		glm::mat4 matrix;
		glm::mat4 jointMatrix[64]{};
		float jointcount{ 0 };
	} uniformBlock;
};

/*
		glTF ObAnimation channel
	*/
struct ObAnimationChannel {
	enum PathType { TRANSLATION, ROTATION, SCALE };
	PathType path;
	ObNode* node;
	uint32_t samplerIndex;
};

/*
	glTF ObAnimation sampler
*/
struct ObAnimationSampler {
	enum InterpolationType { LINEAR, STEP, CUBICSPLINE };
	InterpolationType interpolation;
	std::vector<float> inputs;
	std::vector<glm::vec4> outputsVec4;
};

struct ObAnimation {
	std::string name;
	std::vector<ObAnimationSampler> samplers;
	std::vector<ObAnimationChannel> channels;
	float start = std::numeric_limits<float>::max();
	float end = std::numeric_limits<float>::min();
};


class Bone {
public:
	Bone(const std::string& name, int ID, const aiNodeAnim* channel);
	void Update(float animationTime);


	glm::mat4 GetLocalTransform() { return m_LocalTransform; }
	std::string GetBoneName() const { return m_Name; }
	int GetBoneID() { return m_ID; }

	int GetPositionIndex(float animationTime);


	int GetRotationIndex(float animationTime);

	int GetScaleIndex(float animationTime);
private:

	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
	glm::mat4 InterpolatePosition(float animationTime);

	glm::mat4 InterpolateRotation(float animationTime);

	glm::mat4 InterpolateScaling(float animationTime);

	std::vector<KeyPosition> m_Positions;
	std::vector<KeyRotation> m_Rotations;
	std::vector<KeyScale> m_Scales;
	int m_NumPositions;
	int m_NumRotations;
	int m_NumScalings;

	glm::mat4 m_LocalTransform;
	std::string m_Name;
	int m_ID;


};
#endif // !OB_MESH_H
