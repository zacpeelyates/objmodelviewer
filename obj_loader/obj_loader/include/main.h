#include <OBJDatatypes.h>

#ifndef MISC_USEFUL_METHODS
    std::vector<std::string> splitStringAtChar(std::string data, char character);
#endif // !MISC_USEFUL_METHODS

#ifndef OBJ_DATA_PROCESSING
    bool getKeyValuePair(const std::string& line, std::string& outKey, std::string& outValue);
    vec4 getVectorFromValue(const std::string data);
    OBJVerticies getFaceFromVertex(std::string faceData, std::vector<vec4> verts, std::vector<vec4> normals, std::vector<vec2>& UVCs);

#endif // !OBJ_DATA_PROCESSING

#ifndef FILE_INFO
    struct FileInfo {

        //set default values
        std::string filename = "FILE_NOT_FOUND";
        std::vector<OBJMaterial> materials;
        uint32_t verticies = 0;
        uint32_t vertex = 0;
        uint32_t normals = 0;
        uint32_t textures = 0;
        std::string vertF = "";
        std::vector<OBJGroup> groups;
        uint32_t faces = 0;
        std::string faceTypes = "";
        uint32_t triangles = 0;
        uint32_t quads = 0;
        uint32_t triStrips = 0;
        std::streamsize fileSize = 0;
        int memSize = 0;

};

    void printInfo(FileInfo f);
#endif // !FILE_INFO




   