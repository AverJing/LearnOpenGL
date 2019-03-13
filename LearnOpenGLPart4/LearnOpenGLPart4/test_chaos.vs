#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoords;

uniform float time;

//uniform mat4 projection;

void main()
{
    gl_Position = vec4(aPos, 0.0f, 1.0f); 
    
    if(true)
    {
        float strength = 0.3;
        vec2 pos = vec2(aTexCoord.x + sin(time) * strength, aTexCoord.y + cos(time) * strength);        
        TexCoords = pos;
    }
} 