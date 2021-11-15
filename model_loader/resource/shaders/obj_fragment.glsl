#version 400
smooth in vec3 vertPos;
smooth in vec3 vertNormal;

out vec4 outputColor;
uniform vec3 camPos;

uniform vec3 kA;
uniform vec3 kD;
uniform vec3 kS;

uniform float nS;

vec3 iA = vec3(0.25f);
vec3 iD = vec3(1.0f);
vec3 iS = vec3(1.0f);


vec3 lightDir = normalize(vec3(0.0f) - vec3(10.0f,8.0f,10.0f));


void main()
{
	vec3 Ambient = kA * iA;
	float nDl = max(0.0f,dot(normalize(vertNormal),-lightDir));
	vec3 Diffuse = kD * iD * nDl;

	vec3 R = reflect(lightDir, normalize(vertNormal));
	vec3 E = normalize(camPos - vertPos);

	float SpecTerm = 1.0f;// pow(min(0.0f,dot(R,E)),nS);
	vec3 Specular = kS * iS * SpecTerm;
	outputColor = vec4((Ambient + Diffuse + Specular)*0.75, 1.0f);

}

