#version 450 core
layout (location = 0) in vec3 aPos;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4( aPos.x, aPos.y, 0.0, 1.0 );
    TexCoords = vec2( aPos.x * 0.5 + 0.5, aPos.y * 0.5 + 0.5 );
}
