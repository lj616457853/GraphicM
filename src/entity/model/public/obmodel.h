#ifndef OB_MODEL_H
#define OB_MODEL_H
#include<obmesh.h>

#include <tinygltf/tiny_gltf.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

enum FileLoadingFlags {
	None = 0x00000000,
	PreTransformVertices = 0x00000001,
	PreMultiplyVertexColors = 0x00000002,
	FlipY = 0x00000004,
	DontLoadImages = 0x00000008
};

class ObModel {
	std::string name;

	std::vector<OBTexture> textures;

	std::vector<ObAnimation> animations;


	std::vector<ObNode*> nodes;


	std::vector<ObNode*> linearNodes;

	std::vector<ObSkin*> skins;

	std::vector<uint32_t> indexs;

	std::vector<OBVertex> vertexs;

	struct Dimensions {
		glm::vec3 min = glm::vec3(FLT_MAX);
		glm::vec3 max = glm::vec3(-FLT_MAX);
		glm::vec3 size;
		glm::vec3 center;
		float radius;
	} dimensions;
	std::string path;

	ObModel();
	
	void getNodeDimensions(ObNode* node, glm::vec3& min, glm::vec3& max);


	void getSceneDimensions();

	void updateAnimation(uint32_t index, float time);

	void loadFromFile(std::string filename, float scale = 1.0f, uint32_t fileLoadingFlags = FileLoadingFlags::None);

	void loadFromFileGltf(std::string filename, float scale = 1.0f, uint32_t fileLoadingFlags = FileLoadingFlags::None);


	void loadFromFileAmip(std::string filename, float scale = 1.0f, uint32_t fileLoadingFlags = FileLoadingFlags::None);

	
	void loadNodeFromThy(ObNode* parent, const tinygltf::Node& node, uint32_t nodeIndex, const tinygltf::Model& model, std::vector<uint32_t>& indexBuffer, std::vector<OBVertex>& vertexBuffer, float globalscale);
	
	
	void loadSkinsThy(tinygltf::Model& gltfModel);

	void loadImagesThy(tinygltf::Model& gltfModel);
	
	void loadMaterialsThy(tinygltf::Model& gltfModel);
	
	void loadAnimationsThy(tinygltf::Model& gltfModel);
	
	void loadNodeFromAmip(ObNode* parent, aiNode& node,uint32_t nodeIndex,const aiScene & sene,float globalscale);

	void loadSkinsAmi(aiScene& gltfModel);
	void loadImagesAmi(aiScene& gltfModel);
	void loadMaterialsAmi(aiScene& gltfModel);
	void loadAnimationsAmi(aiScene& gltfModel);
	ObNode* findNode(ObNode* parent, uint32_t index);



	ObNode* nodeFromIndex(uint32_t index);

};

class APModel {
public:
	vector<OBTexture> textures_loaded;

	vector<APMesh>    meshes;
	std::string directory;
	bool gammaCorrection;
	//Animator* current_animator = nullptr;
	std::string modelpath;
	APModel(std::string  path, bool gamma = false);
	APModel() = default;
	~APModel();
	auto& GetBoneInfoMap() { return m_BoneInfoMap; }
	int& GetBoneCount() { return m_BoneCounter; }
	bool nodeload=false;
private:
	
	std::map<std::string, BoneInfo> m_BoneInfoMap;
	int m_BoneCounter = 0;
	void loadModel(std::string const& path);
	void processNode(aiNode* node, const aiScene* scene);
	void SetVertexBoneDataToDefault(APVertex& vertex);
	APMesh processMesh(aiMesh* mesh, const aiScene* scene);
	void SetVertexBoneData(APVertex& vertex, int boneID, float weight);

	void ExtractBoneWeightForVertices(std::vector<APVertex>& vertices, aiMesh* mesh, const aiScene* scene);

	unsigned char* TextureFromFile(OBTexture* texture,const char* path, const std::string& directory, bool gamma = false);

	vector<OBTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};


class AmpAnimation {
public:
	AmpAnimation() = default;

	AmpAnimation(const std::string& animationPath, APModel* model);
	
	~AmpAnimation();

	Bone* FindBone(const std::string& name);

	inline float GetTicksPerSecond() { return m_TicksPerSecond; }
	inline float GetDuration() { return m_Duration; }
	inline const AssimpNodeData& GetRootNode() { return m_RootNode; }
	inline const std::map<std::string, BoneInfo>& GetBoneIDMap()
	{
		return m_BoneInfoMap;
	}

private:
	void ReadMissingBones(const aiAnimation* animation, APModel& model);


	void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);

	float m_Duration;
	int m_TicksPerSecond;
	std::vector<Bone> m_Bones;
	AssimpNodeData m_RootNode;
	std::map<std::string, BoneInfo> m_BoneInfoMap;

};


class Animator {
public:
	Animator(AmpAnimation* animation);

	Animator() {};

	~Animator() {};
	void UpdateAnimation(float dt);

	void PlayAnimation(AmpAnimation* pAnimation);

	void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

	std::vector<glm::mat4> GetFinalBoneMatrices()
	{
		return m_FinalBoneMatrices;
	}

private:
	std::vector<glm::mat4> m_FinalBoneMatrices;
	AmpAnimation* m_CurrentAnimation;
	float m_CurrentTime;
	float m_DeltaTime;


};

#endif // !OB_MODEL_H
