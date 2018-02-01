layout(location = SCENE_POSITION_ATTRIB_LOCATION)
in vec4 Position;
out vec4 frag_Position;

layout(location = SCENE_TEXCOORD_ATTRIB_LOCATION)
in vec2 TexCoord;
out vec2 frag_TexCoord;

layout(location = SCENE_NORMAL_ATTRIB_LOCATION)
in vec3 Normal;
out vec3 frag_Normal;

uniform mat4 ModelWorld;
uniform mat4 ModelViewProjection;
uniform mat3 Normal_ModelWorld;

out vec3 normalInterp;
out vec3 vecPosition;

void main()
{
    // TODO: Set to MVP * P
	gl_Position = ModelViewProjection * Position;
    
    // TODO: Pass vertex attributes to fragment shader
	frag_TexCoord = TexCoord;
	vec4 vertPos4 =  ModelWorld * Position;
	vecPosition = vec3(vertPos4) / vertPos4.w;
	normalInterp = Normal_ModelWorld * Normal;
}