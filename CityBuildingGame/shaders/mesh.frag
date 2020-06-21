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
uniform vec3 cameraPosition;
uniform sampler2D shadowMap;
uniform vec2 texelSizeShadowMap;
uniform bool hasSpecular = false;

layout(binding = 0) uniform sampler2D Texture0;
layout(binding = 1) uniform sampler2D Texture1;
layout(binding = 2) uniform sampler2D Texture2;

layout(binding = 10) uniform sampler2D SpecularTexture;

float ShadowCalculation()
{
    float shadow;
    float closestDepth;

    float bias = 0.0001; // 0.005 -> detached shadows, 0.00005 -> shadow calculation inaccuracy/shadow acne

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
    vec3 specular = {0, 0, 0};
    vec3 result;
    if (AlternativeTexture == 0.0)
	    texColor = texture(Texture0, TexCoords);
    if (AlternativeTexture == 1.0)
        texColor = texture(Texture1, TexCoords);
    if (AlternativeTexture == 2.0)
        texColor = texture(Texture2, TexCoords);
	if (texColor.a < 0.5)
		discard;

	vec3 diffuse = light.diffuse * clamp(dot(Normal, light.direction),  0.0, 1.0); 

    if (hasSpecular) {
        float specularStrength = texture(SpecularTexture, TexCoords).r * 5;
        vec3 specularDirection = reflect(-light.direction, Normal);
        vec3 viewDirection = normalize(cameraPosition - FragPos);
        float spec = pow(max(dot(viewDirection, specularDirection), 0.0), 2);
        vec3 specular = specularStrength * spec * light.diffuse;
    } 
    result = (((diffuse + specular) * (1.0 - ShadowCalculation())) + light.ambient ) * texColor.rgb;

    FragColor = vec4(result, 1);
}
