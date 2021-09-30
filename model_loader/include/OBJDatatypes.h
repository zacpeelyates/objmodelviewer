#ifndef __OBJDATATYPES_H__
#define __OBJDATATYPES_H__

#include "Vecs.h"
#include <cstdint>
#include <string>
#include <vector>
#include <map>

struct OBJVertex
{
public:
	OBJVertex();
	OBJVertex(Vec3 a_oPosVec, Vec3 a_oNormVec, Vec2 a_oTextureVec);

	Vec3 pos, normal;
	Vec2 uvCoord;
	uint32_t smoothingGroupIndex;
};
struct OBJColor
{
	void Print() const;

	float specularExponent = 0.0f;
	float dissolve = 0.0f;
	float refract = 0.0f;
	uint8_t illumination = 0;
	Vec3 ambience, diffuse, specular, specHighlight, emissive, transmission;
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

	std::vector<Vec3> positionVec;
	std::vector<Vec3> normalVec;
	std::vector<Vec2> textureVec;
	std::vector<OBJGroup> groups;
	std::map<std::string, OBJMaterial> materials;
	OBJMesh mesh;
	uint32_t faces = 0;
};
#endif // !__OBJDATATYPES_H__