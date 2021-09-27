#include <stdlib.h>
#include <main.h>

#include <iostream> 
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>



int main(int argc, char *argv[]){
    
    FileInfo f;
    std::string filepath = "../resources/obj_models/basic_box.obj";
    std::cout << "processing file: " << filepath << std::endl;
    std::fstream file;
    file.open(filepath, std::ios_base::in | std::ios_base::binary);

    if(file.is_open()){

        std::cout << "opened file" << std::endl;
        f.filename = filepath.erase(0, filepath.rfind('/') + 1);
        file.ignore(std::numeric_limits<std::streamsize>::max());
        f.fileSize = file.gcount(); 
        if (f.fileSize == 0) {
            std::cout << "no data found" << std::endl;
            file.close();
        }
        else {
            f.fileSize /= (float)1024; //convert to KB
            file.seekg(0, std::ios_base::beg);
        }
  

        std::string line;
        std::vector<vec2> textureData;
        std::vector<vec4> vertexData;
        std::vector<vec4> normalData; //could combine these into a struct?
        std::vector<OBJVerticies> meshData;
        std::vector<uint32_t> meshIndices; //could merge meshdata and mesh indices into a dictionary/map?
        std::map<std::string, int32_t> faceIndexMap;
        bool triFlag = false;
        bool quadFlag = false;
        bool stripFlag = false;

        while(!file.eof()){

            if(getline(file,line)){

                std::string statement = "";
                std::string value = "";

                if(getKeyValuePair(line,statement,value)){                  

                    if (statement == "v") { //vertex
                        f.vertex++;
                        vec4 v = getVectorFromValue(value);
                        v.w = 1.0f;
                        vertexData.push_back(v);
                    }
                    else if (statement == "vn") { //normal
                        f.normals++;
                        vec4 n = getVectorFromValue(value);
                        n.w = 0.0f;
                        normalData.push_back(n);
                    }
                    else if (statement == "vt") { //texture
                        f.textures++;
                        vec4 v = getVectorFromValue(value);
                        textureData.push_back({ v.x,v.y });
                    }
                    else if (statement == "vp") { //param

                    }
                    else if (statement == "f") { //face
                        f.faces++;
                        std::vector<std::string> components = splitStringAtChar(value, ' '); //more than 3 = quad  more than  4 = triangle strip
                        std::vector<uint32_t> faceIndicies;
                        int compCount = components.size();

                        if (compCount == 3) {
                            f.triangles++;
                            if (!triFlag) {
                                f.faceTypes += "Triangles ";
                                triFlag = true;
                            }
                        }
                        else if (compCount == 4) {
                            f.quads++;
                            if (!quadFlag) {
                                f.faceTypes += "Quads ";
                                quadFlag = true;
                            }
                        }
                        else if (compCount > 4) {
                            f.triStrips++;
                            if (!stripFlag) {
                                f.faceTypes += "TriStrips ";
                                stripFlag = true;
                            }
                            else std::cout << "INCORRECT NUMBER OF FACE COMPONENTS: " << compCount << std::endl;
                        }

                        for (auto iValue = components.begin(); iValue != components.end(); ++iValue) {
                            auto iKey = faceIndexMap.find(*iValue); //check map to see if current value has already been parsed
                            if (iKey == faceIndexMap.end()) { //vertex is new   

                                meshData.push_back(getFaceFromVertex(*iValue, vertexData, normalData, textureData));
                                faceIndicies.push_back(faceIndexMap[*iValue] = (int32_t)meshData.size() - 1); //push index back and add to map
                            }
                            else { //vertex is duplicate
                                faceIndicies.push_back((*iKey).second);
                            }
                        }

                        for (int i = 1; i < faceIndicies.size() - 1; ++i) { //change from fan notation to regular triangle notation
                            meshIndices.push_back(faceIndicies[0]);
                            meshIndices.push_back(faceIndicies[i]);
                            meshIndices.push_back(faceIndicies[(size_t)i + 1]);
                        }

                    }
                    else if (statement == "#") { //comment
                        std::cout << value << std::endl;
                    }
                    else if (statement == "o") { //model name

                    }
                    else if (statement == "usemtl") {
                        OBJMaterial currentMat;
                        currentMat.name = value;
                        f.materials.push_back({ currentMat });
                    }
                    else if (statement == "g") {
                        OBJGroup currentGroup;
                        currentGroup.name = value;
                        f.groups.push_back({ currentGroup });
                    }
                    else std::cout << "UNHANDLED STATEMENT: " << statement << std::endl;

                  
                } 

            }
        }
        //info print out
        
        f.verticies = meshData.size();
        f.memSize = (meshData.size()* sizeof(uint32_t))/(float)1024;

        printInfo(f);
        
        //cleanup
        vertexData.clear();
        normalData.clear();
        textureData.clear();

        file.close();

    }
    else {
        std::cout << "Could not open file" << std::endl;
        return 1;
    }
    return 0; 
}

void printInfo(FileInfo f) {

    if (f.vertex > 0) f.vertF += "Contains data for vertex";
    if (f.normals > 0) f.vertF += " and normals";
    if (f.textures > 0) f.vertF += " and textures"; //this is the worst way of doing anything ever 

    std::cout << "\nFilename:\t" << f.filename << std::endl;
    std::cout << "Materials:\t" << f.materials.size() << std::endl;
    for (int i = 0; i < f.materials.size(); ++i) {
        std::cout << "MATERIAL " << i+1 << ":\t" << f.materials[i].name << std::endl;
    }
    std::cout << "Verticies:\t" << f.verticies << std::endl;
    std::cout << "Vertex Count:\t" << f.vertex << std::endl;
    std::cout << "Normal Count:\t" << f.normals << std::endl;
    std::cout << "Texture Count:\t" << f.textures << std::endl;
    std::cout << "Vertex Format:\t" << f.vertF << std::endl;
    std::cout << "Group Count:\t" << f.groups.size() << std::endl;
    for (int i = 0; i < f.groups.size(); ++i) {
        std::cout << "GROUP " << i+1 << ":\t" << f.groups[i].name << std::endl;
    }
    std::cout << "Face Count:\t" << f.faces << std::endl;
    std::cout << "Face Types:\t" << f.faceTypes << std::endl;
    std::cout << "Triangle Count:\t" << f.triangles << std::endl;
    std::cout << "Quad Count:\t" << f.quads << std::endl;
    std::cout << "TriStrip Count:\t" << f.triStrips << std::endl;
    std::cout << "File Size:\t" << f.fileSize << "KB" << std::endl;
    std::cout << "Memory Size:\t" << f.memSize <<  "KB" << std::endl;

}

OBJVerticies getFaceFromVertex(std::string faceData, std::vector<vec4> verts, std::vector<vec4> normals, std::vector<vec2>& UVCs){
    //this can probably be refactored tbh idk why we use this struct of ints instead of just ints
    std::vector<std::string> vertIndicies = splitStringAtChar(faceData,'/');
    //define triplet struct
    typedef struct OBJFaceTriplet {int32_t v,vn,vt;} OBJFaceTriplet;
    OBJFaceTriplet ft = {0,0,0};
    //store present data in triplet
    ft.v = stoi(vertIndicies[0]); //store vertex data
    if(vertIndicies.size() > 1) { //additional data is present
        if(!vertIndicies[1].empty()) ft.vt = stoi(vertIndicies[1]); //store uvcoord data
        if(vertIndicies.size() > 2)  ft.vn = stoi(vertIndicies[2]); //store normal data
    }    

    OBJVerticies current; 
    current.vertex = verts[size_t(ft.v)-1];
    if(ft.vn != 0) current.normal = normals[size_t(ft.vn)-1];
    if(ft.vt != 0) current.uvCoord = UVCs[size_t(ft.vt)-1];
    return current;

}

std::vector<std::string> splitStringAtChar(std::string s, char c){
    std::vector<std::string> outVec;
    std::stringstream ss(s);
    std::string seg;
    while(getline(ss,seg,c)) outVec.push_back(seg);
    return outVec;
}

bool getKeyValuePair(const std::string& line, std::string& outKey, std::string& outValue){

    if(line.empty()) return false;
    std::cout << line << std::endl;

    //get key
    size_t keyFirst = line.find_first_not_of("  \t\r\n");
    if(keyFirst == std::string::npos) return false; //key was not valid
    size_t keyLast = line.find_first_of("   \t\r\n",keyFirst); //start search from keyfirst
    outKey = line.substr(keyFirst, keyLast - keyFirst);

    //get value range 
    size_t valueFirst = line.find_first_not_of("  \t\r\n", keyLast); //start search from where we left off 
    if(valueFirst == std::string::npos) return false; //value was not valid
    size_t valueLast = line.find_last_not_of("  \t\r\n")+1; //add one to get last character as method used is exclusive
    outValue = line.substr(valueFirst, valueLast - valueFirst);

    std::cout << "KVP: " << outKey << "," << outValue << std::endl;

    return true;

}

vec4 getVectorFromValue(const std::string data){

    std::stringstream ss(data);
    vec4 outVec = {0.0f,0.0f,0.0f,0.0f};

    int i = 0;
    for(std::string s; ss >> s; ++i) {((float*)(&outVec))[i] = stof(s);}

    return outVec;
}