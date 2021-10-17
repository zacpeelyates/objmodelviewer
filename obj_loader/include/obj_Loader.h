#include <glm/glm.hpp>
#include <vector>
#include <string>

//OBJVertex Class
class OBJVertex {

public:
	OBJVertex();
	~OBJVertex();
	
	glm::vec3 GetPosition();
	void SetPosition();

	glm::vec3 GetNormal();
	void SetNormal();

	glm::vec2 GetTextureCoords();
	void SetTextureCoords();


	enum Offsets
	{
		POS = 0,
		NORMAL = POS + sizeof(glm::vec3),
		UVCOORD = NORMAL + sizeof(glm::vec3)
	};
	bool operator == (const OBJVertex& a_oOther) const;
	bool operator < (const OBJVertex& a_oOther) const;
private:
	glm::vec3 pos, normal;
	glm::vec2 uvCoord;
	unsigned int smoothingIndex;

};
//OBJVertex Definitions
