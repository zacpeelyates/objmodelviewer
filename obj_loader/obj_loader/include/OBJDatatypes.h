#include <vector>
#include <string>

#ifndef OBJ_STATMENTS_ENUMS
enum OBJSTATEMENT
{
    //enum containing all supported OBJ statements, so that they may be switch-cased or iterated over

    V = 0, //spatial vertex
    VT, //texture vertex
    VN, //normal vertex
    VP, //parameter vertex
    CSTYPE, //freeform type
    DEG, //freeform degree
    BMAT, //freeform matrix
    STEP, //freeform step
    P, //polygonal points
    L, //polygonal line
    F, //polygonal face
    CURV, //freeform curve
    CURV2, //freeform curve (2D)
    SURF, //freeform surface
    PARAM, //freeform parameter
    TRIM, //freeform trim curve
    HOLE, //freeform hole curve
    SCRV, //freeform special curve
    SP, //freeform special point
    END, //freeform end-of-statement
    CON, //freeform surface connection
    G, //group state
    S, //smoothing group state
    MG, //merging group state
    O, //object name
    BEVEL, //bevel interpolation
    C_INTERP, //color interpolation
    D_INTERP, //dissolve interpolation
    LOD, //level-of-detail
    USEMTL, //active material
    MTLLIB, //material lib path
    SHADOW_OBJ, //shadow object
    TRACE_OBJ, //tracer object
    CTECH, //curve subdivision 
    STECH, //surface subdivision
    COMMENT,
    UNKNOWN
}; //this isnt actually used yet
#endif // !OBJ_STATMENTS_ENUMS


#ifndef DATA_TYPE_STRUCTS
struct vec2 { float x, y; };
struct vec3 { float x, y, z; };
struct vec4 { float x, y, z, w; };

struct OBJVerticies { vec4 vertex, normal; vec2 uvCoord; };
struct OBJFace { OBJVerticies first, second, third, quad; }; //face may be triangle or quad
struct OBJLine { std::vector<OBJVerticies> segments; };

struct OBJMaterial {
    std::string name;
    //TODO: add other material stuff
};

struct OBJGroup {
    std::string name;
    //TODO: add other group stuff
};



#endif // !DATA_TYPE_STRUCTS

