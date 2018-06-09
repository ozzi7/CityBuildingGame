#version 450 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;

// texture samplers
uniform sampler2D texture1;

struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
};

uniform Light light;
uniform vec3 viewPos;

void main()
{
	vec4 texColor = texture(texture1, TexCoord);

    vec3 ambient = light.ambient * texColor.rgb;
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texColor.rgb;  
    
	vec3 result = ambient + diffuse;
    FragColor = vec4(result, texColor.a);
}