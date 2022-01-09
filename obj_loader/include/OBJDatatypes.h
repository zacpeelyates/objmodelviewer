#ifndef OBJDATATYPES_H
#define OBJDATATYPES_H

#include "glm.hpp"
#include <map>
#include <string>
#include <vector>



struct OBJFace
{
	unsigned int posIndex = 0;
	unsigned int normIndex = 0;
	unsigned int uvIndex = 0;
};

class OBJVertex
{

public:
	OBJVertex();
	~OBJVertex();
	
	glm::vec3 GetPosition();
	void SetPosition(glm::vec3 a_v3New);
	void TranslatePosition(glm::vec3 a_v3Delta);

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
inline OBJVertex::OBJVertex() {};
inline OBJVertex::~OBJVertex() {};


struct OBJMaterial
{
public:
	std::string name;
	glm::vec3 GetAmbience(), GetDiffuse(), GetSpecular(), GetEmissive();
	void SetAmbience(glm::vec3 a_inv3), SetDiffuse(glm::vec3 a_inv3), SetSpecular(glm::vec3 a_inv3), SetEmissive(glm::vec3 a_inv3);

	float GetDensity(), GetDissolve(), GetSpecularExponent(), GetTransparency();
	void SetDensity(float a_fIn), SetDissolve(float a_fIn), SetSpecularExponent(float a_fIn), SetTransparency(float a_fIn);

	uint8_t GetIlluminationModel();
	void SetIlluminationModel(uint8_t a_inUint);

	enum TextureTypes
	{
		DiffuseTexture = 0,
		SpecularTexture,
		NormalTexture,

		TextureTypes_Count
	};

	std::string textureFileNames[TextureTypes_Count];
	unsigned int textureIDs[TextureTypes_Count];

private:
	glm::vec3 ambience, diffuse, specular, emissive;
	float density, dissolve, specularExponent,transparency;
	uint8_t illumModel;

};

struct OBJMesh
{
	OBJMesh();
	~OBJMesh();
	glm::vec3 calculateFaceNormal(const unsigned int& a_indexA, const unsigned int& a_indexB, const unsigned int& a_indexC);
	void CalculateUnassignedFaceNormals();
	std::vector<OBJVertex> m_verts;
	std::vector<unsigned int> m_indicies;
	OBJMaterial* m_activeMaterial = nullptr;
};
inline OBJMesh::OBJMesh() {};
inline OBJMesh::~OBJMesh() {}




struct OBJGroup
{
	std::string name;
	std::vector<OBJMesh*> meshes;
};


class OBJModel
{
public:
	OBJModel() : m_worldMatrix(glm::mat4(1.0f)) {};
	~OBJModel() {};
	bool AddGroup(OBJGroup* ao_groupIn);
	bool AddMaterial(OBJMaterial* a_oInMaterial);
	void AddMesh(OBJMesh* a_InMesh);
	unsigned int GetMeshCount();
	OBJMesh* GetMesh(unsigned int index);
	OBJMaterial* GetMaterial(std::string a_name);
	OBJMaterial* GetMaterial(unsigned int index);
	unsigned int GetMaterialCount();
    glm::mat4 GetWorldMatrix();

	void SetWorldMatrix(glm::mat4 a_inMatrix);

private:
	std::vector<OBJMesh*> m_meshes;
	std::map<std::string, OBJGroup*> m_groupMap;
	std::map<std::string, OBJMaterial*> m_matMap;
	glm::mat4 m_worldMatrix;
};
#endif // OBJDATATYPES_H
