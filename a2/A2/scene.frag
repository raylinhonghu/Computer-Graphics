uniform vec3 CameraPos;

uniform vec3 Ambient;
uniform vec3 Diffuse;
uniform vec3 Specular;
uniform float Shininess;

uniform int HasDiffuseMap;
uniform sampler2D DiffuseMap;

const float screenGamma = 2.2;

in vec4 frag_Position;
in vec2 frag_TexCoord;
out vec2 fragTexCoord;
in vec3 frag_Normal;

in vec3 normalInterp;
in vec3 vecPosition;

out vec4 FragColor;

void main()
{
    // TODO: Replace with Phong shading
	
	vec3 normal = normalize(normalInterp);
	vec3 lightDir = normalize(CameraPos - vecPosition);

	float lambertian = max(dot(lightDir,normal), 0.0);
	float specular = 0.0;

	if(lambertian > 0.0) {
		vec3 viewDir = normalize(-vecPosition);

		//blinn phong
		vec3 halfDir = normalize(lightDir + viewDir);
		float specAngle = max(dot(halfDir, normal), 0.0);
		specular = pow(specAngle, Shininess);
	}

	vec3 diffuseMap;

	if(HasDiffuseMap != 0) {
		diffuseMap = texture(DiffuseMap, frag_TexCoord).rgb;
		//FragColor = vec4(diffuseMap, 1.0);
		vec3 colorLinear = 0.05*Ambient + lambertian * diffuseMap + specular * Specular;
		
		vec3 colorGammaCorrected = pow(colorLinear, vec3(1.0/screenGamma));
		FragColor = vec4(colorGammaCorrected, 1.0);
	}
	else {
		diffuseMap = vec3(1.0);
		
		vec3 colorLinear = 0.05*Ambient + lambertian * diffuseMap + specular * Specular;
		
		vec3 colorGammaCorrected = pow(colorLinear, vec3(1.0/screenGamma));
		FragColor = vec4(colorGammaCorrected, 1.0);
	}
}