#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 Pos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    Pos = aPos;
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}