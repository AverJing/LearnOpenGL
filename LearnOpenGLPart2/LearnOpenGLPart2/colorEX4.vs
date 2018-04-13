	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aNormal;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
	
	uniform vec3 viewPos;
	uniform vec3 lightPos; // we now define the uniform in the vertex shader and pass the 'view space' lightpos to the fragment shader. lightPos is currently in world space.

	uniform vec3 objectColor;
	uniform vec3 lightColor;

	out vec3 result;

	void main()
	{
	    gl_Position = projection * view * model * vec4(aPos, 1.0f);
		vec3 FragPos = vec3(model * vec4(aPos, 1.0f));
		vec3 Normal = mat3(transpose(inverse(model))) * aNormal;

		float ambientStrength = 0.1f;
		vec3 ambient = ambientStrength * lightColor; 

		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(lightPos - FragPos);

		float diff = max(dot(norm, lightDir), 0.0f);
		vec3 diffuse = diff * lightColor;

		float specularStrengh = 0.5f;
		vec3 viewDir = normalize(viewPos - FragPos); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
		vec3 specular = specularStrengh * spec * lightColor;

		result = (ambient + diffuse + specular) * objectColor;
	}