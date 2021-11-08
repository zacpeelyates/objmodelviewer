#version 400

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uvCoord;

out vec3 vertPos;
out vec3 vertNormal;
out vec2 vertUV;

uniform mat4 ProjectionViewMatrix;
uniform mat4 ModelMatrix;
vec4 tempVec;

void main()
{
	vertUV = uvCoord;
	vertNormal = normal;
	tempVec = ModelMatrix * vec4(position.xyz,1.0f);
	vertPos = tempVec.xyz;
	gl_Position = ProjectionViewMatrix * ModelMatrix * vec4(position,1.0f);
	
}
