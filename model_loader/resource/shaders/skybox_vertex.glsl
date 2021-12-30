#version 400

layout (location = 0) in vec3 position;

out vec3 UVCoords;

uniform mat4 ProjectionViewMatrix;

void main()
{
	UVCoords = vec3(position.xy,-position.z); //cubemap uses left hand coordinate system, openGL uses right, need to flip Z axis
	gl_Position = (ProjectionViewMatrix * vec4(UVCoords,1.0)).xyww;
}
