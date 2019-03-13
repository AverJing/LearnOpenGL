#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

uniform vec3 cameraPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_reflection1;
uniform samplerCube skybox;


void main()
{            
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    vec4 diffuse_color = texture(texture_diffuse1, TexCoords);
    vec4 spec_color = texture(texture_specular1, TexCoords);

    float reflect_intensity = texture(texture_reflection1, TexCoords).r;
    vec4 reflect_color;
    if(reflect_intensity > 0.01) // Only sample reflections when above a certain treshold
        reflect_color = texture(skybox, R) * reflect_intensity;

    FragColor = vec4(diffuse_color + spec_color + reflect_color);
}