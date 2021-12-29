#version 400
smooth in vec3 vertPos;
smooth in vec3 vertNormal;
smooth in vec2 vertUV;

out vec4 outputColor;

uniform vec3 camPos;
uniform vec3 kA;
uniform vec3 kD;
uniform vec3 kS;
uniform vec3 lightPos;

uniform float nS;

vec3 iA = vec3(0.25f);
vec3 iD = vec3(1.0f);
vec3 iS = vec3(1.0f);



//texture
uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;
uniform sampler2D NormalTexture;


void main()
{
	vec3 lightDir = normalize(vec3(0.0f) - lightPos);
	//texture
	vec4 normalData = texture(NormalTexture,vertUV);
	vec4 specularData = texture(SpecularTexture,vertUV);
	vec4 diffuseData = texture(DiffuseTexture,vertUV);
	//lighting 
	vec3 modifiedNormal = vertNormal * normalData.rgb;
	vec3 Ambient = kA * iA;
	float nDl = max(0.0f,dot(normalize(modifiedNormal),-lightDir));
	vec3 Diffuse = kD * iD * nDl * diffuseData.rgb;

	vec3 R = reflect(lightDir, normalize(modifiedNormal));
	vec3 E = normalize(camPos - vertPos);

	float SpecTerm = pow(max(0.0f,dot(R,E)),nS);
	vec3 Specular = kS * iS * SpecTerm * specularData.rgb;
	outputColor = vec4((Ambient + Diffuse + Specular), 1.0f);

}

