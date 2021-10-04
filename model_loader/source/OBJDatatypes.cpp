//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: OBJDatatypes.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 30/09/21
// Last Edited:  30/09/21
// Brief: Collection of function implementations used in structs that store various types of data used in OBJ Files
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "OBJDatatypes.h"
#include <iostream>

OBJVertex::OBJVertex()
{
	smoothingGroupIndex = 0;
}
OBJVertex::OBJVertex(glm::vec3 a_oPosVec, glm::vec3 a_oNormVec, glm::vec2 a_oTextureVec)
{
	this->pos = a_oPosVec;
	this->normal = a_oNormVec;
	this->uvCoord = a_oTextureVec;
	smoothingGroupIndex = 0;
}
;

//TODO: Put commented print info into a std::vector so that we can just iterate through it.
void OBJColor::Print() const
{
	//std::cout << "Ambience RGB:" << "\n\tR:" << ambience.x << "\n\tG:" << ambience.y << "\n\tB:" << ambience.z << std::endl;
	//std::cout << "Diffuse RGB:" << "\n\tR:" << diffuse.x << "\n\tG:" << diffuse.y << "\n\tB:" << diffuse.z << std::endl;
	//std::cout << "Specular RGB:" << "\n\tR:" << specular.x << "\n\tG:" << specular.y << "\n\tB:" << specular.z << std::endl;
	//std::cout << "Highlight RGB:" << "\n\tR:" << specHighlight.x << "\n\tG:" << specHighlight.y << "\n\tB:" << specHighlight.z << std::endl;
	//std::cout << "Emissive RGB:" << "\n\tR:" << emissive.x << "\n\tG:" << emissive.y << "\n\tB:" << emissive.z << std::endl;
	//std::cout << "Transmission RGB:" << "\n\tR:" << transmission.x << "\n\tG:" << transmission.y << "\n\tB:" << transmission.z << std::endl;
}

void OBJTexture::Print() const
{
	//std::cout << "Ambience Map:" << ambience << std::endl;
	//std::cout << "Diffuse Map:" << diffuse << std::endl;
	//std::cout << "Specular Map:" << specular << std::endl;
	//std::cout << "Alpha Map:" << alpha << std::endl;
	//std::cout << "Bump Map:" << bump << std::endl;
	//std::cout << "Dissolve Map:" << dissolve << std::endl;
}

void OBJMaterial::Print() const
{
	std::cout << "\n\tMATERIAL" << std::endl;
	std::cout << "Name:\t" << name << std::endl;
	color.Print();
	texture.Print();
}

void OBJGroup::Print() const
{
	std::cout << "\n\tGROUP" << std::endl;
	std::cout << "Name:\t" << name << std::endl;
	std::cout << "Position Verticies:\t" << vCount << std::endl;
	std::cout << "Normal Verticies:\t" << vnCount << std::endl;
	std::cout << "Texture Coordinates:\t" << vtCount << std::endl;

}

void OBJData::Print() const
{
	std::cout << "\nSize (Memory):\t" << (mesh.verts.size() * sizeof(OBJVertex)) / 1024 << "KB" << std::endl;
	if (positionVec.size() != 0)
	{
		std::cout << "Position Verticies:\t" << positionVec.size() << std::endl;
	}
	else
	{
		std::cout << "File is MISSING Position Data!" << std::endl;;
	}
	if (normalVec.size() != 0)
	{
		std::cout << "Normal Verticies:\t" << normalVec.size() << std::endl;
	}
	else
	{
		std::cout << "File does not contain normal data" << std::endl;
		//todo: generate our own normals at some point
	}
	if (textureVec.size() != 0)
	{
		std::cout << "Texture Coordinates:\t" << textureVec.size() << std::endl;
	}
	else
	{
		std::cout << "File does not contain texture data" << std::endl;
	}
	std::cout << "Verts in mesh: " << mesh.verts.size() << std::endl;
	std::cout << "Faces:\t" << faces << std::endl;
	std::cout << "Triangles:\t" << positionVec.size() / 3 << std::endl;

	std::cout << "\nGroups:\t" << groups.size() << std::endl;
	for (OBJGroup g : groups) {
		g.Print();
	}

	std::cout << "\nMaterials:\t" << materials.size() << std::endl;
	for (auto& kvp : materials) {
		kvp.second.Print();
	}
}

void OBJMesh::Clear()
{
	verts.clear();
	indices.clear();
}

void OBJData::Clear()
{
	positionVec.clear();
	normalVec.clear();
	textureVec.clear();
	groups.clear();
	materials.clear();
	mesh.Clear();
}


