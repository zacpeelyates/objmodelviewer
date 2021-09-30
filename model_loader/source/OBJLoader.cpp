#include "OBJLoader.h"
#include "MiscTools.h"
#include <iostream>
#include <sstream>

bool OBJLoader::OBJLoad(FileManager a_oFileManager, bool a_bPrintComments)
{
	OBJData LoadedData;
	std::map<std::string, int32_t> faceIndexMap;
	std::string line;

	while (!a_oFileManager.file.eof())
	{
		if (std::getline(a_oFileManager.file, line)) {

			std::string objStatement;
			std::string value;

			if (OBJGetKeyValuePairFromLine(line, objStatement, value))
			{
				if (objStatement[0] == '#') //comment, done this way bc some files dont put a space after # in comment which leads to UnhandledStatement errors if not handled here
				{
					if (a_bPrintComments)
					{
						std::cout << value << std::endl;
					}
				}
				else if (objStatement == "g" || objStatement == "o") //group or object, blender counts them as the same so i will too 
				{
					OBJGroup g;
					g.name = value;
					LoadedData.groups.push_back(g);
				}
				else if (objStatement == "s")
				{
					if (ParseStringToInt(value)) 
					{
						LoadedData.mesh.verts.back().smoothingGroupIndex = std::stoi(value);
					}
					else if(value == "off")
					{
						std::cout << "Disabling smoothing groups" << std::endl;
					}
				}
				else if (objStatement == "v") //positional data
				{
					if (LoadedData.groups.size() != 0)
					{
						LoadedData.groups.back().vCount++;
					}
					LoadedData.positionVec.push_back(OBJGetVectorFromValue(value));
				}
				else if (objStatement == "vn") //normal data
				{
					if (LoadedData.groups.size() != 0)
					{
						LoadedData.groups[LoadedData.groups.size() - 1].vnCount++;
					}
					LoadedData.normalVec.push_back(OBJGetVectorFromValue(value));
				}
				else if (objStatement == "vt") //texture data
				{
					if (LoadedData.groups.size() != 0)
					{
						LoadedData.groups[LoadedData.groups.size() - 1].vtCount++;
					}
					LoadedData.textureVec.push_back(OBJGetVectorFromValue(value));
				}
				else if (objStatement == "f") //face data
				{
					LoadedData.faces++;
					std::vector<std::string> faceComponents = SplitStringAtChar(value, ' ');
					std::vector<uint16_t> faceIndicies;
					for (auto iter = faceComponents.begin(); iter != faceComponents.end(); ++iter)
					{
						auto iKey = faceIndexMap.find(*iter);
						if (iKey == faceIndexMap.end()) //new vertex
						{
							LoadedData.mesh.verts.push_back(OBJGetFaceFromVertex(*iter, LoadedData));
							faceIndicies.push_back(faceIndexMap[*iter] = (int32_t)LoadedData.mesh.verts.size() - 1);
						}
						else //duplicate vertex
						{
							faceIndicies.push_back((*iKey).second);
						}
					}

					for (int i = 1; i < faceIndicies.size() - 1; ++i) { //change from fan notation to regular triangle notation
						LoadedData.mesh.indices.push_back(faceIndicies[0]);
						LoadedData.mesh.indices.push_back(faceIndicies[i]);
						LoadedData.mesh.indices.push_back(faceIndicies[(size_t)i + 1]);
					}

				}
				else if (objStatement == "usemtl")
				{
					OBJMaterial m;
					if (LoadedData.materials.find(value) == LoadedData.materials.end())
					{

						m.name = value;
						LoadedData.materials.emplace(m.name, m);
					}
					LoadedData.mesh.activeMaterial = m;
				}
				else if (objStatement == "mtllib")
				{
					OBJLoadMaterials(a_oFileManager.GetDirectory() + value, LoadedData, a_bPrintComments);
				}
				else
				{
					std::cout << "Unhandled statement: " << objStatement << std::endl;
				}

			}
		}
	}
	std::cout << "File succesfully parsed!" << std::endl;
	a_oFileManager.Print();
	a_oFileManager.file.close();
	LoadedData.Print();
	LoadedData.Clear();
	return true;
}

bool OBJLoader::OBJLoadMaterials(const std::string& a_strFilePath, OBJData& a_oLoadedData, const bool ac_bPrintComments)
{
	std::fstream file;
	FileManager fm(a_strFilePath, file);
	if (fm.initialized)
	{
		std::cout << a_strFilePath << " Loaded!" << std::endl;
		while (!fm.file.eof()) {
			std::string line;
			while (std::getline(fm.file, line))
			{
				std::string mtlStatement;
				std::string value;
				OBJMaterial current;
				if (OBJGetKeyValuePairFromLine(line, mtlStatement, value))
				{

					if (mtlStatement[0] == '#') //comment
					{
						if (ac_bPrintComments)
						{
							std::cout << value << std::endl;
						}
					}
					else if (mtlStatement == "newmtl")
					{
						if (a_oLoadedData.materials.find(value) == a_oLoadedData.materials.end())
						{

							current.name = value;
							a_oLoadedData.materials.emplace(current.name, current);
						}
						a_oLoadedData.mesh.activeMaterial = current;
					}
					else if (mtlStatement == "Ns")
					{
						current.color.specularExponent = std::stof(value);
					}
					else if (mtlStatement == "Ka")
					{
						current.color.ambience = OBJGetVectorFromValue(value);
					}
					else if (mtlStatement == "Kd")
					{
						current.color.diffuse = OBJGetVectorFromValue(value);
					}
					else if (mtlStatement == "Ks")
					{
						current.color.specHighlight = OBJGetVectorFromValue(value);
					}
					else if (mtlStatement == "Ke")
					{
						current.color.emissive = OBJGetVectorFromValue(value);
					}
					else if (mtlStatement == "Tf")
					{
						current.color.transmission = OBJGetVectorFromValue(value);
					}
					else if (mtlStatement == "Ni")
					{
						current.color.refract = std::stof(value);
					}
					else if (mtlStatement == "d")
					{
						current.color.dissolve = std::stof(value);
					}
					else if (mtlStatement == "illum")
					{
						current.color.illumination = std::stoi(value);
					}
					else if (mtlStatement == "map_Ka")
					{
						current.texture.ambience = value;
						//TODO PROCESS TEXTURES
					}
					else if (mtlStatement == "map_Kd")
					{
						current.texture.diffuse = value;
					}
					else if (mtlStatement == "map_Ks")
					{
						current.texture.specular = value;
					}
					else if (mtlStatement == "map_d")
					{
						current.texture.dissolve = value;
					}
					else if (mtlStatement == "bump")
					{
						current.texture.bump = value;
					}

					else
					{
						std::cout << "Unhandled statement: " << mtlStatement << std::endl;
					}
				}
			}

		}
		std::cout << "Material file successfully parsed!" << std::endl;
	}
	else
	{
		return false;
	}
	return true;
}

OBJVertex OBJLoader::OBJGetFaceFromVertex(std::string a_strFaceData, OBJData& a_oLoadedData)
{
	std::vector<std::string> vertIndicies = SplitStringAtChar(a_strFaceData, '/');
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

	OBJVertex current;
	current.pos = a_oLoadedData.positionVec[size_t(face.posIndex) - 1];
	if (face.normIndex != 0)
	{
		current.normal = a_oLoadedData.normalVec[size_t(face.normIndex) - 1];
	}
	if (face.uvIndex != 0)
	{
		current.uvCoord = a_oLoadedData.textureVec[size_t(face.uvIndex) - 1];
	}
	return current;
}

Vec4 OBJLoader::OBJGetVectorFromValue(const std::string ac_strValue)
{
	std::stringstream ss(ac_strValue);
	Vec4 outVertex;
	int i = 0;
	for (std::string s; ss >> s; ++i)
	{
		((float*)(&outVertex))[i] = stof(s);
	}
	return outVertex;
}

bool OBJLoader::OBJGetKeyValuePairFromLine(const std::string& ac_rStrLine, std::string& a_rStrOutKey, std::string& a_rStrOutValue)
{
	if (ac_rStrLine.empty()) return false;

	//get key
	size_t keyFirst = ac_rStrLine.find_first_not_of("  \t\r\n");
	if (keyFirst == std::string::npos) return false; //key was not valid
	size_t keyLast = ac_rStrLine.find_first_of("   \t\r\n", keyFirst); //start search from keyfirst
	a_rStrOutKey = ac_rStrLine.substr(keyFirst, keyLast - keyFirst);

	//get value range 
	size_t valueFirst = ac_rStrLine.find_first_not_of("  \t\r\n", keyLast); //start search from where we left off 
	if (valueFirst == std::string::npos) return false; //value was not valid
	size_t valueLast = ac_rStrLine.find_last_not_of("  \t\r\n") + 1; //add one to get last character as method used is exclusive
	a_rStrOutValue = ac_rStrLine.substr(valueFirst, valueLast - valueFirst);

	return true;
}
