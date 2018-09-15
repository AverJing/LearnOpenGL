#version 330 core
layout (location = 0) in vec4 vertex; //<vec2 position, vec2 texCoords>
layout (location = 1) in vec3 color;
layout (location = 2) in mat4 model;
layout (location = 6) in int index;

out vec2 TexCoords;
out vec3 InColor;
flat out int Index;

uniform mat4 projection;

void main(){
    TexCoords = vertex.zw;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
    InColor = color;
    Index = index;
}