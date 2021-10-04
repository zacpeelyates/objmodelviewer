////////////////////////////////////////////////////////
// File: OBJDatatypes.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 30/09/21
// Last Edited:  30/09/21
// Brief: Function declarations for OBJDatatypes.cpp
///////////////////////////////////////////////////////
#ifndef __OBJDATATYPES_H__
#define __OBJDATATYPES_H__

#include <glm/glm.hpp>
#include <cstdint>
#include <string>
#include <vector>
#include <map>

struct OBJVertex
{
public:
	OBJVertex();
	OBJVertex(glm::vec3 a_oPosVec, glm::vec3 a_oNormVec, glm::vec2 a_oTextureVec);

	glm::vec3 pos, normal;
	glm::vec2 uvCoord;
	uint32_t smoothingGroupIndex;
};
struct OBJColor
{
	void Print() const;

	float specularExponent = 0.0f;
	float dissolve = 0.0f;
	float refract = 0.0f;
	uint8_t illumination = 0;
	glm::vec3 ambience, diffuse, specular, specHighlight, emissive, transmission;
};
struct OBJTexture
{
	void Print() const;
	std::string ambience, diffuse, specular, alpha, bump, dissolve;
};
struct OBJMaterial
{
	void Print() const;

	std::string name;
	OBJColor color;
	OBJTexture texture;
};
struct OBJFace
{
	uint32_t posIndex = 0;
	uint32_t normIndex = 0;
	uint32_t uvIndex = 0;
};
struct OBJMesh
{
	void Clear();

	std::string name = "";
	std::vector<OBJVertex> verts;
	std::vector<uint32_t> indices;
	OBJMaterial activeMaterial;
};
struct OBJGroup
{
	void Print() const;

	std::string name = "";
	uint32_t vCount = 0;
	uint32_t vnCount = 0;
	uint32_t vtCount = 0;
};
struct OBJData
{
	void Clear();
	void Print() const;

	std::vector<glm::vec3> positionVec;
	std::vector<glm::vec3> normalVec;
	std::vector<glm::vec3> textureVec;
	std::vector<OBJGroup> groups;
	std::map<std::string, OBJMaterial> materials;
	OBJMesh mesh;
	uint32_t faces = 0;
};
#endif // !__OBJDATATYPES_H__