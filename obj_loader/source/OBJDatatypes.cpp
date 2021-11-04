#include <glm\glm.hpp>
#include "OBJDatatypes.h"

bool OBJModel::AddGroup(OBJGroup* ao_groupIn)
{
	if (m_groupMap.find(ao_groupIn->name) == m_groupMap.end()) 
	{
		m_groupMap.emplace(ao_groupIn->name, ao_groupIn);
		return true;
	}
	return false;

}

const std::string OBJModel::GetPath()
{
	return m_path;
}

unsigned int OBJModel::GetMeshCount()
{
	return m_meshes.size();
}

std::vector<OBJMesh*> OBJModel::GetMeshes()
{
	return m_meshes;
}


OBJMesh* OBJModel::GetMesh(unsigned int a_uiIndex)
{
	unsigned int meshCount = m_meshes.size();
	if (meshCount > 0 && a_uiIndex < meshCount)
	{
		return m_meshes[a_uiIndex];
	}
	return nullptr;
}

OBJMaterial* OBJModel::GetMaterial(std::string a_name)
{
	auto mat = m_matMap.find(a_name);
	if (mat == m_matMap.end()) return nullptr;
	return mat->second;
}

void OBJModel::AddMaterial(OBJMaterial* a_poInMaterial)
{
	if (m_matMap.find(a_poInMaterial->name) == m_matMap.end())
	{
		m_matMap.emplace(a_poInMaterial->name, a_poInMaterial);
	}
}

glm::vec3 OBJVertex::GetPosition()
{
	return m_pos;
}

glm::vec3 OBJVertex::GetNormal()
{
	return m_normal;
}

glm::vec2 OBJVertex::GetTextureCoords()
{
	return m_uvCoord;
}

void OBJVertex::SetPosition(glm::vec3 a_v3New)
{
	m_pos = a_v3New;
}

void OBJVertex::SetNormal(glm::vec3 a_v3New)
{
	m_normal = a_v3New;
}

void OBJVertex::SetTextureCoords(glm::vec2 a_v2New)
{
	m_uvCoord = a_v2New;
}


bool OBJVertex::operator==(const OBJVertex& a_oOther) const
{
	return memcmp(this,&a_oOther,sizeof(OBJVertex)) == 0;
}

bool OBJVertex::operator<(const OBJVertex& a_oOther) const
{
	return memcmp(this, &a_oOther, sizeof(OBJVertex)) < 0;
}
