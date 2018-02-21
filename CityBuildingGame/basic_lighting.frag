#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoord;

uniform mat4 model;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform sampler2D texture1;

void main()
{
	vec3 normal = normalize(transpose(inverse(mat3(model))) * Normal);
    vec3 surfacePos = vec3(model * vec4(FragPos, 1));
    vec4 surfaceColor = texture(texture1, TexCoord);
    vec3 surfaceToLight = normalize(lightPos - surfacePos);
    vec3 surfaceToCamera = normalize(viewPos - surfacePos);
    
    //ambient
    vec3 ambient = 0.05 * surfaceColor.rgb * lightColor;

    //diffuse
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * lightColor;
    
    //specular
    //float specularCoefficient = 0.0;
    //if(diffuseCoefficient > 0.0)
    //    specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
    //vec3 specular = specularCoefficient * materialSpecularColor * lightColor;
    
    //attenuation
    float distanceToLight = length(lightPos - surfacePos);
    float attenuation = 1.0 / (1.0 + 0.2 * pow(distanceToLight, 2));

    //linear color (color before gamma correction)
    vec3 linearColor = ambient + attenuation*diffuse;//(diffuse + specular);
    
    //final color (after gamma correction)
    vec3 gamma = vec3(1.0/2.2);
    FragColor = vec4(pow(linearColor, gamma), surfaceColor.a);
}