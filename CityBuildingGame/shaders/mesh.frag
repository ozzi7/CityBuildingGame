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

in vec3 Normal;
in vec2 TexCoords;

uniform Material material;
uniform Light light;

void main()
{
	vec4 texel = texture(material.diffuse, TexCoords);

	vec4 texColor = texture(material.diffuse, TexCoords);

    vec3 ambient = light.ambient * texColor.rgb;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texColor.rgb;   
    
	vec3 result = ambient + diffuse;
    FragColor = vec4(result, texColor.a);
}