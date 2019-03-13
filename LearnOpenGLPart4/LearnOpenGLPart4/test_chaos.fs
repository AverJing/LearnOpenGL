#version 330 core
in  vec2  TexCoords;
out vec4  color;
  
uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;  

void main()
{
   vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );
    /*
    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    */
    /*
    float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
    );
    */
    float kernel[9] = float[](
    -1.0 , -1.0 , -1.0 ,
    -1.0 , 8.0 , -1.0 ,
    -1.0 , -1.0 , -1.0  
    );
    vec3 sample[9];
        for(int i = 0; i < 9; i++)
            sample[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
        
        for(int i = 0; i < 9; i++)
            color += vec4(sample[i] * kernel[i], 0.0f);
        color.a = 1.0f;
}