#include <stdlib.h>
#include <stdarg.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <map>
#include <cctype>
#include <chrono>

#ifndef MISC_USEFUL_METHODS
std::vector<std::string> SplitStringAtChar(std::string data, char c)
{
	std::vector<std::string> outVec;
	std::stringstream ss(data);
	std::string seg;
	while (std::getline(ss, seg, c))
	{
		outVec.push_back(seg);
	}
	return outVec;
}
#endif // !MISC_USEFUL_METHODS

#ifndef VECTOR_STRUCTS
struct Vec4
{
	float x, y, z, w;
	Vec4()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 0.0f;
	}

	Vec4(float f_inX, float f_inY, float f_inZ, float f_inW)
	{
		x = f_inX;
		y = f_inY;
		z = f_inZ;
		w = f_inW;
	}
};

struct Vec3
{
	float x, y, z;
	Vec3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	Vec3(float f_inX, float f_inY, float f_inZ)
	{
		x = f_inX;
		y = f_inY;
		z = f_inZ;
	}

	Vec3(Vec4 o_inV4)
	{
		x = o_inV4.x;
		y = o_inV4.y;
		z = o_inV4.z;
	}

};

struct Vec2 
{
	float x, y;
	Vec2() 
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vec2(float f_inX, float f_inY) 
	{
		x = f_inX;
		y = f_inY;
	}

	Vec2(Vec3 o_inV3, ...)
	{
		x = o_inV3.x;
		y = o_inV3.y;
	}

	Vec2(Vec4 o_inV4)
	{
		x = o_inV4.x;
		y = o_inV4.y;
	}
};

#endif //VECTOR_STRUCTS

#ifndef OBJ_STRUCTS
struct OBJVertex
{
	Vec3 pos,normal;
	Vec2 uvCoord;
	uint32_t smoothingGroupIndex;

	OBJVertex()	{	}

	OBJVertex(Vec3 p, Vec3 n, Vec2 t)
	{
		pos = p;
		normal = n;
		uvCoord = t;
	}
};
struct OBJColor
{
	Vec3 ambience,diffuse,specular,specHighlight,emissive,transmission;
	float specularExponent = 0,dissolve = 0, refract = 0; 
	int illumination = 0;

	void Print()
	{
		std::cout << "Color Info Not Implemented" << std::endl;
	}
};
struct OBJTexture
{
	std::string ambience, diffuse, specular, alpha, bump, dissolve;

	void Print()
	{
		std::cout << "Texture Info Not Implemented" << std::endl;
	}
};
struct OBJMaterial
{
	std::string name = "";
	OBJColor color;
	OBJTexture texture;

	void Print()
	{
		std::cout << "\n\tMATERIAL" << std::endl;
		std::cout << "Name:\t" << name << std::endl;
		color.Print();
		texture.Print();
	}
};
struct OBJFace
{
	uint32_t posIndex = 0, normIndex = 0, uvIndex = 0;
};
struct OBJMesh
{
	std::string name = "";
	std::vector<OBJVertex> verts;
	std::vector<uint32_t> indices;
	OBJMaterial activeMaterial;

	void Clear()
	{
		verts.clear();
		indices.clear();
	}
};
struct OBJGroup
{
	std::string name = "";
	uint32_t v = 0;
	uint32_t vn = 0;
	uint32_t vt = 0;

	void Print()
	{
		std::cout << "\t\nGROUP" << std::endl;
		std::cout << "Name:\t" << name << std::endl;
		std::cout << "Pos:\t" << v << std::endl;
		std::cout << "Norms:\t" << vn << std::endl;
		std::cout << "UVs:\t" << vt << std::endl;
	}

};
struct OBJData
{
	std::vector<Vec3> positionVec;
	std::vector<Vec3> normalVec;
	std::vector<Vec2> textureVec;
	std::vector<OBJGroup> groups;
	std::vector<std::string> objectNames;
	std::map<std::string, OBJMaterial> materials;
	OBJMesh mesh; //can an obj file have more than one mesh?
	uint32_t faces = 0;

	void Clear()
	{
		positionVec.clear();
		normalVec.clear();
		textureVec.clear();
		groups.clear();
		materials.clear();
		mesh.Clear();
	}

	void Print()
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
		for (auto &kvp : materials) {
			kvp.second.Print();
		}

		std::cout << "\nObjects:\t" << objectNames.size() << std::endl;
		for (std::string o : objectNames)
		{
			std::cout << o << std::endl;
		}


	}


};
#endif // !OBJ_STRUCTS

#ifndef FILE_MANAGER_CLASS
class FileManager
{
	public:
	std::string path;
	std::fstream &file;
	std::streamsize bytes;
	bool initialized;
	std::chrono::time_point<std::chrono::steady_clock> time;

	FileManager(std::string filepath, std::fstream& fileIn) : file(fileIn)
	{
		time = std::chrono::high_resolution_clock::now();
		path = filepath;
		initialized = false;
		std::cout << "Initializing file: " << path << std::endl;
		file.open(path, std::ios_base::in | std::ios_base::binary);
		if (file.is_open())
		{
			file.ignore(std::numeric_limits<std::streamsize>::max());
			bytes = file.gcount();
			if (bytes == 0)
			{
				std::cout << "File is empty. Closing." << std::endl;
				file.close();
			}
			else
			{
				file.seekg(0, std::ios_base::beg);
				initialized = true;
				std::cout << "File initialized" << std::endl;
			}
		}
		else
		{
			std::cout << "Could not open file." << std::endl;
		}
	}

	const std::string GetName()
	{
		return path.substr(path.rfind('/')+1, path.rfind('.'));
	}

	const std::string GetType()
	{
		return path.substr(path.rfind('.')+1);
	}

	const std::string GetDirectory()
	{
		return path.substr(0, path.rfind('/') + 1);
	}

	long GetTime()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time).count();
	}

	std::streamsize GetFileSize()
	{
		return bytes / 1024;
	}

	void PrintFileInfo()
	{
		std::cout << "\nName:\t" << this->GetName() << std::endl;
		std::cout << "Type:\t"<< this->GetType() << std::endl;
		std::cout << "Size (File):\t" << this->GetFileSize() << "KB" << std::endl;
		std::cout << "Time to Process: " << this->GetTime() <<  "ms" << std::endl;
	}
};
#endif // !FILE_MANAGER_CLASS

#ifndef OBJ_LOADER_CLASS
class OBJLoader
{
	public:
	bool OBJLoad(FileManager fm, bool printComments)
	{
		OBJData LoadedData;	
		std::map<std::string, int32_t> faceIndexMap;
		std::string line;
		
		while (!fm.file.eof())
		{
			if (std::getline(fm.file, line)) {

				std::string objStatement;
				std::string value;

				if (OBJGetKeyValuePair(line, objStatement, value))
				{
					if (objStatement[0] == '#') //comment, done this way bc some files dont put a space after # in comment
					{
						if (printComments)
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
						LoadedData.mesh.verts.back().smoothingGroupIndex = std::stoi(value);
					}
					else if (objStatement == "v") //positional data
					{			
						if (LoadedData.groups.size() != 0)
						{
							LoadedData.groups[LoadedData.groups.size()-1].v++;
						}
						LoadedData.positionVec.push_back(OBJGetVectorFromValue(value));
					}
					else if (objStatement == "vn") //normal data
					{
						if (LoadedData.groups.size() != 0)
						{
							LoadedData.groups[LoadedData.groups.size() - 1].vn++;
						}
						LoadedData.normalVec.push_back(OBJGetVectorFromValue(value));
					}
					else if (objStatement == "vt") //texture data
					{
						if (LoadedData.groups.size() != 0)
						{
							LoadedData.groups[LoadedData.groups.size() - 1].vt++;
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
							LoadedData.materials.emplace(m.name,m);
						}
						LoadedData.mesh.activeMaterial = m;
					}
					else if (objStatement == "mtllib")
					{
						OBJLoadMaterials(fm.GetDirectory()+value,LoadedData,printComments);
					}
					else
					{
						std::cout << "Unhandled statement: " << objStatement << std::endl;
					}
					
				}
			}
		}
		std::cout << "File succesfully parsed!" << std::endl;
		fm.PrintFileInfo();
		fm.file.close();
		LoadedData.Print();
		LoadedData.Clear();
		return true;
	}

	bool OBJLoadMaterials(const std::string&path, OBJData &data, const bool &showComments)
	{
		std::fstream file;
		FileManager fm(path,file);
		if (fm.initialized) 
		{
			std::cout << path << " Loaded!" << std::endl;
			while (!fm.file.eof()) {
				std::string line;
				while (std::getline(fm.file, line))
				{
					std::string mtlStatement;
					std::string value;
					OBJMaterial current;
					if (OBJGetKeyValuePair(line, mtlStatement, value))
					{

						if (mtlStatement[0] == '#') //comment
						{
							if (showComments)
							{
								std::cout << value << std::endl;
							}
						}
						else if (mtlStatement == "newmtl")
						{						
							if (data.materials.find(value) == data.materials.end())
							{

								current.name = value;
								data.materials.emplace(current.name, current);
							}
							data.mesh.activeMaterial = current;
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
						else if (mtlStatement == "D")
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
	}

	OBJVertex OBJGetFaceFromVertex(std::string faceData, OBJData& data)
	{
		std::vector<std::string> vertIndicies = SplitStringAtChar(faceData, '/');
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
		current.pos = data.positionVec[size_t(face.posIndex) - 1];
		if (face.normIndex != 0)
		{
			current.normal = data.normalVec[size_t(face.normIndex) - 1];
		}
		if (face.uvIndex != 0)
		{
			current.uvCoord = data.textureVec[size_t(face.uvIndex) - 1];
		}
		return current;
	}

	Vec4 OBJGetVectorFromValue(const std::string data)
	{
		std::stringstream ss(data);
		Vec4 outVertex;
		int i = 0;
		for (std::string s; ss >> s; ++i)
		{ 
			((float*)(&outVertex))[i] = stof(s);
		}
		return outVertex;
	}

	bool OBJGetKeyValuePair(const std::string& line, std::string &outKey, std::string &outValue)
	{
		if (line.empty()) return false;

		//get key
		size_t keyFirst = line.find_first_not_of("  \t\r\n");
		if (keyFirst == std::string::npos) return false; //key was not valid
		size_t keyLast = line.find_first_of("   \t\r\n", keyFirst); //start search from keyfirst
		outKey = line.substr(keyFirst, keyLast - keyFirst);

		//get value range 
		size_t valueFirst = line.find_first_not_of("  \t\r\n", keyLast); //start search from where we left off 
		if (valueFirst == std::string::npos) return false; //value was not valid
		size_t valueLast = line.find_last_not_of("  \t\r\n") + 1; //add one to get last character as method used is exclusive
		outValue = line.substr(valueFirst, valueLast - valueFirst);

		return true;
	}
};
#endif // !OBJ_LOADER_CLASS

#ifndef MAIN_METHOD
int main(int argc, char* argv[])
{
	std::string inputPath;
	std::cout << "Enter Filename: ";
	std::getline(std::cin,inputPath);
	std::fstream filestream;
	FileManager fileManager(inputPath, filestream);
	if (fileManager.initialized) {
		if (fileManager.GetType() == "obj")
		{
			OBJLoader objLoader;
			char charIn = ' ';
			std::cout << "Print comments? (Y / any other key): ";
			std::cin >> charIn;
			bool showComments = std::tolower(charIn) == 'y';
			objLoader.OBJLoad(fileManager, showComments);
		}
		else
		{
			std::cout << "Unsupported Filetype! Exiting." << std::endl;
			return 1;
		}
	}
	else
	{
		std::cout << "File failed to initialize! Exiting." << std::endl;
		return 1;
	}
	return 0;
}
#endif // !MAIN_METHOD

