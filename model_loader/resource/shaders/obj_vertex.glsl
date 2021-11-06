#version 400
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 uvCoord;
smooth out vec4 vertColour;
uniform mat4 ProjectionViewMatrix;
void main()
{
vertColour = vec4(0.5f,0.5f,0.5f,1.0f);
gl_Position = ProjectionViewMatrix * position;
}
