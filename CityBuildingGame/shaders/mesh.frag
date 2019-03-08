#version 450 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    //vec3 specular;    
    //float shininess;
}; 

struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

uniform Material material;
uniform Light light;
uniform sampler2D shadowMap;

float ShadowCalculation()
{
	// perform perspective divide
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, FragPosLightSpace.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main()
{
	vec4 texColor = texture(material.diffuse, TexCoords);
	if(texColor.a < 0.5)
		discard;
		
    vec3 ambient = light.ambient * texColor.rgb;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texColor.rgb;   
    float shadow = ShadowCalculation();
	vec3 result = diffuse * (1 - shadow) + ambient;
    FragColor = vec4(result, texColor.a);
	//FragColor = vec4(vec3(1.0-shadow), texColor.a);
}
