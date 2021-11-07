#version 400
smooth in vec4 vertColour;
smooth in vec3 vertNormal;
out vec4 outputColour;
void main()
{
 vec4 lightDir = normalize(vec4(0.0f) - vec4(10.0f,10.0f,10.0f,0.0f));
 float nDl = max(0.0f, dot(normalize(vec4(vertNormal.xyz,1.0f)),-lightDir));
 vec4 litColor = vec4(vertColour.xyz * nDl, 1.0);
 outputColour = vertColour + litColor;
}