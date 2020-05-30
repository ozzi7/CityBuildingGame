#version 450 core
out vec4 FragColor;

struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;
in float AlternativeTexture; // 0 = normal texture, 1.0 = secondary texture, etc.

uniform Light light;
uniform sampler2D shadowMap;
uniform vec2 texelSizeShadowMap;
layout(binding = 0) uniform sampler2D Texture0;
layout(binding = 1) uniform sampler2D Texture1;

float ShadowCalculation()
{
    float shadow;
    float closestDepth;

    float bias = 0.0004; // 0.005 -> detached shadows, 0.00005 -> shadow calculation inaccuracy/shadow acne

    // perform perspective divide
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    ////////////////////////////
    // Gaussian blur sampling //
    ////////////////////////////
    closestDepth = texture(shadowMap, projCoords.xy * texelSizeShadowMap).r;
    shadow += projCoords.z - bias > closestDepth ? 0.2 : 0.0;

    closestDepth = texture(shadowMap, projCoords.xy + vec2(0,-1) * texelSizeShadowMap).r;
    shadow += projCoords.z - bias > closestDepth ? 0.1236 : 0.0;

    closestDepth = texture(shadowMap, projCoords.xy + vec2(0,1) * texelSizeShadowMap).r;
    shadow += projCoords.z - bias > closestDepth ? 0.1236 : 0.0;

    closestDepth = texture(shadowMap, projCoords.xy + vec2(1,0) * texelSizeShadowMap).r;
    shadow += projCoords.z - bias > closestDepth ? 0.1236 : 0.0;

    closestDepth = texture(shadowMap, projCoords.xy + vec2(-1,0) * texelSizeShadowMap).r;
    shadow += projCoords.z - bias > closestDepth ? 0.1236 : 0.0;

    closestDepth = texture(shadowMap, projCoords.xy + vec2(-1,-1) * texelSizeShadowMap).r;
    shadow += projCoords.z - bias > closestDepth ? 0.0764 : 0.0;

    closestDepth = texture(shadowMap, projCoords.xy + vec2(-1,1) * texelSizeShadowMap).r;
    shadow += projCoords.z - bias > closestDepth ? 0.0764 : 0.0;

    closestDepth = texture(shadowMap, projCoords.xy + vec2(1,1) * texelSizeShadowMap).r;
    shadow += projCoords.z - bias > closestDepth ? 0.0764 : 0.0;

    closestDepth = texture(shadowMap, projCoords.xy + vec2(1,-1) * texelSizeShadowMap).r;
    shadow += projCoords.z - bias > closestDepth ? 0.0764 : 0.0;

    return shadow;
}

void main()
{
    vec4 texColor;
    if (AlternativeTexture == 0.0)
	    texColor = texture(Texture0, TexCoords);
    if (AlternativeTexture == 1.0)
        texColor = texture(Texture1, TexCoords);
	if (texColor.a < 0.5)
		discard;
		
    vec3 ambient = light.ambient * texColor.rgb;
	vec3 diffuse = light.diffuse * dot(Normal, light.direction) * texColor.rgb;   
	vec3 result = diffuse * (1 - ShadowCalculation()) + ambient;
    FragColor = vec4(result, 1);
}
