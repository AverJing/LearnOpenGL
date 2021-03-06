#version 330 core
layout (location = 0) in vec3 aPos;

//out vec4 color;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    gl_Position =  projection * model * vec4(aPos.xy, 0.0f, 1.0f);
    //color = vec4(aPos, 1.0f);
    //gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
}