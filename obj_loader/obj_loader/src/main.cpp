#include <stdlib.h>
#include <main.h>

#include <iostream> 
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>



int main(int argc, char *argv[]){

    std::string filename = "../resources/obj_models/C1102056.obj";
    std::cout << "processing file: " << filename << std::endl;
    std::fstream file;
    file.open(filename, std::ios_base::in | std::ios_base::binary);

    if(file.is_open()){

        std::cout << "opened file" << std::endl;

        std::string line;
        std::vector<vec2> textureData;
        std::vector<vec4> vertexData;
        std::vector<vec4> normalData; //could combine these into a struct?
        std::vector<OBJVerticies> meshData;
        std::vector<uint32_t> meshIndices; //could merge meshdata and mesh indices into a dictionary/map?
        std::map<std::string, int32_t> faceIndexMap;

        while(!file.eof()){

            if(getline(file,line)){

                std::string key = "";
                std::string value = "";

                if(getKeyValuePair(line,key,value)){

                    

                    if(key == "v"){ //vertex
                        vec4 v = getVectorFromValue(value);
                        v.w = 1.0f;
                        vertexData.push_back(v);
                    }
                    else if(key == "vn"){ //normal
                        vec4 n = getVectorFromValue(value);
                        n.w = 0.0f;
                        normalData.push_back(n);
                    }
                    else if(key == "vt"){ //texture
                        vec4 v = getVectorFromValue(value);
                        textureData.push_back({v.x,v.y});
                    }
                    else if(key == "vp"){ //param
                       
                    }
                    else if(key == "f"){ //face
                        std::vector<std::string> components = splitStringAtChar(value,' ');
                        std::vector<uint32_t> indicies;

                        for (auto iValue = components.begin(); iValue != components.end(); ++iValue) {
                            auto iKey = faceIndexMap.find(*iValue); //check map to see if current value has already been parsed
                            if (iKey == faceIndexMap.end()) { //vertex is new   

                                meshData.push_back(getFaceFromVertex(*iValue, vertexData, normalData, textureData));
                                indicies.push_back(faceIndexMap[*iValue] = (int32_t)meshData.size() - 1); //push index back and add to map
                            }
                            else { //vertex is duplicate
                                indicies.push_back((*iKey).second);
                            }
                        }

                        for (int i = 1; i < indicies.size() - 1; ++i) { //change from fan notation to regular triangle notation
                            meshIndices.push_back(indicies[0]);
                            meshIndices.push_back(indicies[i]);
                            meshIndices.push_back(indicies[(size_t)i + 1]);
                        }

                    }
                    else if(key == "#"){ //comment
                        std::cout << value << std::endl;
                    }
                  
                } 

            }
        }
        std::cout << "verts processed in mesh: " << meshData.size() << std::endl;
        
        vertexData.clear();
        normalData.clear();
        textureData.clear();

        file.close();

    } else return 1;
    return 0; 
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