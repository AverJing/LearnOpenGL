	#version 330 core
	out vec4 FragColor;

	struct Material{
			sampler2D diffuse;
			sampler2D specular;
			float shininess;
	};
	struct Light{
			vec3 position;

			vec3 ambient;
			vec3 diffuse;
			vec3 specular;			
	};

	//uniform vec3 objectColor;
	uniform vec3 viewPos;
	uniform Material material;
	uniform Light light;

	in vec2 texCoord;
	in vec3 Normal;
	in vec3 FragPos;

	void main()
	{
		//ambient 
		vec3 ambient = vec3(texture(material.diffuse, texCoord)) * light.ambient; 
		//diffuse
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(light.position - FragPos);
		float diff = max(dot(norm, lightDir), 0.0f);
		vec3 diffuse = vec3(texture(material.diffuse, texCoord)) * diff * light.diffuse;
		//specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		vec3 specular = (vec3(texture(material.specular, texCoord))) * spec * light.specular;
		//vec3 specular = (vec3(1.0) - vec3(texture(material.specular, texCoord))) * spec * light.specular;// here we inverse the sampled specular color. Black becomes white and white becomes black.

		vec3 result = (ambient + diffuse + specular);
		FragColor = vec4(result, 1.0f);
	}