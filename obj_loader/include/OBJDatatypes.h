#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <map>



struct OBJFace
{
	unsigned int posIndex = 0;
	unsigned int normIndex = 0;
	unsigned int uvIndex = 0;
};

class OBJVertex
{

public:
	OBJVertex() {};
	~OBJVertex() {};
	
	glm::vec3 GetPosition();
	void SetPosition(glm::vec3 a_v3New);

	glm::vec3 GetNormal();
	void SetNormal(glm::vec3 a_v3New);

	glm::vec2 GetTextureCoords();
	void SetTextureCoords(glm::vec2 a_v2New);


	enum Offsets
	{
		POS = 0,
		NORMAL = POS + sizeof(glm::vec3),
		UVCOORD = NORMAL + sizeof(glm::vec3),
		END = UVCOORD + sizeof(glm::vec2)
	};
	bool operator == (const OBJVertex& a_oOther) const;
	bool operator < (const OBJVertex& a_oOther) const;
private:
	glm::vec3 m_pos, m_normal;
	glm::vec2 m_uvCoord;
	unsigned int m_smoothingIndex;

};

struct  OBJColor
{
	float exponent, dis, ref = 0.0f;
	uint8_t illumModel;
};

struct OBJTexture
{
	//std::string amb, dif, spec, alpha, bump, dis;
};

struct OBJMaterial
{
	std::string name;
	OBJColor color;
	OBJTexture texture;
};

struct OBJMesh
{

	OBJMesh() {};
	~OBJMesh() {};
	std::vector<OBJVertex> verts;
	std::vector<unsigned int> indicies;
	OBJMaterial* activeMaterial;
};



struct OBJGroup
{
	std::string name;
	std::vector<OBJMesh*> meshes;
};


class OBJModel
{
public:
	bool AddGroup(OBJGroup* ao_groupIn);
	const std::string GetPath();
	unsigned int GetMeshCount();
	std::vector<OBJMesh*> GetMeshes();
	const glm::mat4& GetWorldMatrix();
	OBJMesh* GetMesh(unsigned int index);
	OBJMaterial* GetMaterial(std::string a_name);
	void AddMaterial(OBJMaterial* a_oInMaterial);

private:
	std::string m_path;
	std::vector<OBJMesh*> m_meshes;
	glm::mat4 m_worldMatrix;
	std::map<std::string, OBJGroup*> m_groupMap;
	std::map<std::string, OBJMaterial*> m_matMap;
};
