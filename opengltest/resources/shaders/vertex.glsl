#version 400
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 colour;
smooth out vec4 vertColour;
void main()
{
vertColour = colour;
g