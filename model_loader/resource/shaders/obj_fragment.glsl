#version 400
smooth in vec3 vertPos;
smooth in vec3 vertNormal;
smooth in vec2 vertUV;
smooth in vec3 lightPos;

out vec4 outputColor;

uniform vec3 camPos;

//material
uniform vec3 kA;
uniform vec3 kD;
uniform vec3 kS;

uniform float nS;

//lighting
vec3 iA = vec3(0.25f);
vec3 iD = vec3(1.0f);
vec3 iS = vec3(1.0f);


//texture
uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;
uniform sampler2D NormalTexture;


void main()
{
	vec3 lightDir = normalize(vec3(0.0f) - vec3(10.0f,8.0f,10.0f));
	//texture
	vec4 normalData = texture(NormalTexture,vertUV);
	vec4 specularData = texture(SpecularTexture,vertUV);
	vec4 diffuseData = texture(DiffuseTexture,vertUV);
	//lighting 
	vec3 Ambient = kA * iA;
	float nDl = max(0.0f,dot(normalize(vertNormal),-lightDir));
	vec3 Diffuse = kD * iD * nDl * normalData.rgb;

	vec3 R = reflect(lightDir, normalize(vertNormal));
	vec3 E = normalize(camPos - vertPos);

	float SpecTerm = pow(max(0.0f,dot(R,E)),nS);
	vec3 Specular = kS * iS * SpecTerm;
	outputColor = vec4((Ambient + Diffuse + Specular), 1.0f);

}

