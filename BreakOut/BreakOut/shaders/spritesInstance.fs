#version 330 core
in vec2 TexCoords;
flat in int Index;
in vec3 InColor;

out vec4 color;

uniform sampler2D image[2];

//https://stackoverflow.com/questions/45167538/error-sampler-arrays-indexed-with-non-constant-expressions-are-forbidden-in-gl
//in 330, index must be constant, not a variable
void main(){
    if(Index == 0){
         color = vec4(InColor, 1.0) * texture(image[0], TexCoords);
    }else{
         color = vec4(InColor, 1.0) * texture(image[1], TexCoords);
    }
    //It is strange, fellow code is wrong. But up code is true.
   //color = vec4(InColor, 1.0) * texture(image[Index], TexCoords);
   //color = vec4(0.0f);
}