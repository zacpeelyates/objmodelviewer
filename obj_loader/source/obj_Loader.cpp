
#include "obj_loader.h"
#include "OBJProcessUtils.h"
#include <sstream>
#include <iostream>
#include <glm\glm.hpp>

//declare these here as they're only needed in this cpp file.
glm::vec4 OBJGetVectorFromValue(const std::string a_strValue);
bool OBJGetKeyValuePairFromLine(const std::string& a_rStrLine, std::string& a_rStrOutKey, std::string& a_rStrOutValue);

OBJModel* OBJLoader::OBJProcess(const std::string& a_strFilePath, const bool a_bPrintComments)
{
	OBJModel oLoadedData;
	std::string line,key,value;
	OBJMesh* currentMesh = nullptr;
	std::vector<glm::vec3> vertexData, normalData;
	std::vector<glm::vec2> textureData;
	std::fstream file(a_strFilePath);
	

	while (!file.eof())
	{
		if (std::getline(file, line))
		{
			if (line.size() > 0) 
			{
				if (OBJGetKeyValuePairFromLine(line, key, value)) 
				{
					if (key[0] == '#')
					{
						if (a_bPrintComments) 
						{
							std::cout << value << std::endl;
						}
					}
					else if(key == "g" || key == "o")
					{
						OBJGroup* g = new OBJGroup();					
						g->name = value;
						if (currentMesh != nullptr)
						{
							oLoadedData.GetMeshes().push_back(currentMesh);
						}										
						currentMesh = new OBJMesh();
						if (oLoadedData.AddGroup(g)) std::cout << "added group" << value;
						else std::cout << "failed to add group " << value;
					}
					else if (key == "s")
					{
						if (OBJProcessUtils::ParseStringToInt(value))
						{
							std::cout << "smoothing group " << stoi(value) << std::endl;
						}
						else if (value == "off")
						{
							std::cout << "disabling smoothing groups" << std::endl;
						}
					}
					else if (key == "v")
					{
						vertexData.push_back(OBJGetVectorFromValue(value));
					}
					else if (key == "vn")
					{
						normalData.push_back(OBJGetVectorFromValue(value));
					}
					else if (key == "vt")
					{
						textureData.push_back(OBJGetVectorFromValue(value));
					}
					else if (key == "usemtl")
					{
						OBJMaterial* m = new OBJMaterial; 
						if (oLoadedData.GetMaterial(value) == nullptr)
						{
							m->name = value;
							oLoadedData.AddMaterial(m);
						}
						else 
						{
							m = oLoadedData.GetMaterial(value);
						}
						if(currentMesh == nullptr)
						{
							currentMesh = new OBJMesh();
						}
						currentMesh->activeMaterial = m;
					}
					else if (key == "mtllib") 
					{
						OBJLoadMaterials(OBJProcessUtils::GetFileDirectory(a_strFilePath) + value,oLoadedData,a_bPrintComments);
					}
					else if (key == "f") 
					{										
						if (currentMesh == nullptr)
						{
							currentMesh = new OBJMesh();
						}
							std::vector<std::string> faceComponents = OBJProcessUtils::SplitStringAtChar(value, ' ');
							unsigned int ci = currentMesh->verts.size();
							for(auto iter = faceComponents.begin(); iter !=faceComponents.end(); ++iter)
							{
								OBJFace face = ProcessFace(*iter);
								OBJVertex currentVertex;
								currentVertex.SetPosition(vertexData[face.posIndex-1]);
								if (face.normIndex != 0) currentVertex.SetNormal(normalData[face.normIndex - 1]);
								if (face.uvIndex != 0) currentVertex.SetTextureCoords(textureData[face.uvIndex - 1]);
								currentMesh->verts.push_back(currentVertex);
							}
							for (unsigned int offset = 1; offset < (faceComponents.size() - 1); ++offset)
							{
								currentMesh->indicies.push_back(ci);
								currentMesh->indicies.push_back(ci + offset);
								currentMesh->indicies.push_back(ci + offset);
							}

						}
					}
				}
			}
		return &oLoadedData;
		}
		return nullptr;
	}



bool OBJLoader::OBJLoadMaterials(const std::string& a_strFilePath, OBJModel& a_roLoadedData, const bool a_bPrintComments)
{
	std::fstream file(a_strFilePath);
	file.open(a_strFilePath, std::ios_base::in | std::ios_base::binary);
	if (file.is_open())
	{
		std::string line; 
		while (!file.eof())
		{
			if (line.size() > 0) {
				std::string key;
				std::string value;
				OBJMaterial* currentMaterial = nullptr;

				if (OBJGetKeyValuePairFromLine(line, key, value))
				{
					if (key[0] == '#')
					{
						if (a_bPrintComments)
						{
							std::cout << value << std::endl;
						}
					}
					else if (key == "newmtl")
					{
						if (a_roLoadedData.GetMaterial(value) == nullptr)
						{
							currentMaterial = new OBJMaterial();
							currentMaterial->name = value;
							a_roLoadedData.AddMaterial(currentMaterial);
						}
					}
				}
			}
		}
		return true;
	}
	return false;
}


OBJFace OBJLoader::ProcessFace(std::string a_strFaceData)
{
	//contstructs an OBJ Face from input string, then return an OBJVertex containing relevant face information
	std::vector<std::string> vertIndicies = OBJProcessUtils::SplitStringAtChar(a_strFaceData, '/');
	OBJFace face = { 0,0,0 };
	face.posIndex = std::stoi(vertIndicies[0]);
	if (vertIndicies.size() > 1) {
		if (vertIndicies[1].size() > 0)
		{
			face.uvIndex = std::stoi(vertIndicies[1]);
		}
		if (vertIndicies.size() > 2)
		{
			face.normIndex = std::stoi(vertIndicies[2]);
		}
	}
	return face;
}


glm::vec4 OBJGetVectorFromValue(const std::string ac_strValue) 
{
	//parses given string into a vec4 -- this vector is implicity cast to the correct size when assigned (dangerous,scary)
	std::stringstream ss(ac_strValue);
	glm::vec4 outVertex;
	int i = 0;
	for (std::string s; ss >> s; ++i)
	{
		((float*)(&outVertex))[i] = stof(s);
	}
	return outVertex;
}

bool OBJGetKeyValuePairFromLine(const std::string& ac_rStrLine, std::string& a_rStrOutKey, std::string& a_rStrOutValue) 
{
	//Splits a line from OBJ file into the key (or OBJStatement) and the associated value (the rest of the file)
	if (ac_rStrLine.empty()) return false;

	//get key
	size_t keyFirst = ac_rStrLine.find_first_not_of("  \t\r\n");
	if (keyFirst == std::string::npos) return false; //key was not valid
	size_t keyLast = ac_rStrLine.find_first_of("   \t\r\n", keyFirst); //start search from keyfirst
	a_rStrOutKey = ac_rStrLine.substr(keyFirst, keyLast - keyFirst);

	//get value 
	size_t valueFirst = ac_rStrLine.find_first_not_of("  \t\r\n", keyLast); //start search from where we left off 
	if (valueFirst == std::string::npos) return false; //value was not valid
	size_t valueLast = ac_rStrLine.find_last_not_of("  \t\r\n") + 1; //add one to get last character as method used is exclusive
	a_rStrOutValue = ac_rStrLine.substr(valueFirst, valueLast - valueFirst);

	return true;
}
