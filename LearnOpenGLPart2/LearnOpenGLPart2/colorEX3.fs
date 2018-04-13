	#version 330 core
	out vec4 FragColor;

	uniform vec3 objectColor;
	uniform vec3 lightColor;

	in vec3 Normal;
	in vec3 FragPos;
	in vec3 LightPos; // extra in variable, since we need the light position in view space we calculate this in the vertex shader

	void main()
	{
		float ambientStrength = 0.1f;
		vec3 ambient = ambientStrength * lightColor; 

		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(LightPos - FragPos);

		float diff = max(dot(norm, lightDir), 0.0f);
		vec3 diffuse = diff * lightColor;

		float specularStrengh = 0.5f;
		vec3 viewDir = normalize(- FragPos); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
		vec3 specular = specularStrengh * spec * lightColor;

		vec3 result = (ambient + diffuse + specular) * objectColor;
		FragColor = vec4(result, 1.0f);
	}