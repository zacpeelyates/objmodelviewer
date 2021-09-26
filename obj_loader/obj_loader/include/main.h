#include <OBJDatatypes.h>

#ifndef MISC_USEFUL_METHODS
    std::vector<std::string> splitStringAtChar(std::string data, char character);
#endif // !MISC_USEFUL_METHODS

#ifndef OBJ_DATA_PROCESSING
    bool getKeyValuePair(const std::string& line, std::string& outKey, std::string& outValue);
    vec4 getVectorFromValue(const std::string data);
    OBJVerticies getFaceFromVertex(std::string faceData, std::vector<vec4> verts, std::vector<vec4> normals, std::vector<vec2>& UVCs);

#endif // !OBJ_DATA_PROCESSING

   