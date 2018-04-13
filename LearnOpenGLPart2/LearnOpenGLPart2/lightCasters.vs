	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aNormal;
	layout (location = 2) in vec2 aTexCoord;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	out vec3 Normal;
	out vec3 FragPos;
	out vec2 texCoord;

	void main()
	{
		FragPos = vec3(model * vec4(aPos, 1.0f));
		Normal = mat3(transpose(inverse(model))) * aNormal;

		//gl_Position = projection * view * model * vec4(aPos, 1.0f);
		gl_Position = projection * view * vec4(FragPos, 1.0f);
		texCoord = aTexCoord;
	}