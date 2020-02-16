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
    float shadow;
    float closestDepth;

    float bias = 0.0004; // 0.005 -> detached shadows, 0.00005 -> shadow calculation inaccuracy/shadow acne
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    // perform perspective divide
    vec3 projCoords = FragPosLightSpace.xyz/ FragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    ////////////////////////////
    // Gaussian blur sampling //
    ////////////////////////////
    closestDepth = texture(shadowMap, projCoords.xy * texelSize).r;           
    shadow += projCoords.z - bias > closestDepth ? 4.0 : 0.0;

    closestDepth = texture(shadowMap, projCoords.xy + vec2(0,-1) * texelSize).r;           
    shadow += projCoords.z - bias > closestDepth ? 2.0 : 0.0;

    closestDepth = texture(shadowMap, projCoords.xy + vec2(0,1) * texelSize).r;           
    shadow += projCoords.z - bias > closestDepth ? 2.0 : 0.0;

    closestDepth = texture(shadowMap, projCoords.xy + vec2(1,0) * texelSize).r;           
    shadow += projCoords.z - bias > closestDepth ? 2.0 : 0.0;

    closestDepth = texture(shadowMap, projCoords.xy + vec2(-1,0) * texelSize).r;           
    shadow += projCoords.z - bias > closestDepth ? 2.0 : 0.0;

    closestDepth = texture(shadowMap, projCoords.xy + vec2(-1,-1) * texelSize).r;           
    shadow += projCoords.z - bias > closestDepth ? 1.0 : 0.0;

    closestDepth = texture(shadowMap, projCoords.xy + vec2(-1,1) * texelSize).r;           
    shadow += projCoords.z - bias > closestDepth ? 1.0 : 0.0;

    closestDepth = texture(shadowMap, projCoords.xy + vec2(1,1) * texelSize).r;           
    shadow += projCoords.z - bias > closestDepth ? 1.0 : 0.0;

    closestDepth = texture(shadowMap, projCoords.xy + vec2(1,-1) * texelSize).r;           
    shadow += projCoords.z - bias > closestDepth ? 1.0 : 0.0;

    shadow /= 16.0;
    return shadow;
}

void main()
{
	vec4 texColor = texture(material.diffuse, TexCoords);
	if(texColor.a < 0.9)
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
