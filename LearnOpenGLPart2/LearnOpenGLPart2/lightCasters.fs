	#version 330 core
	out vec4 FragColor;

	struct Material{
			sampler2D diffuse;
			sampler2D specular;
			float shininess;
	};
	struct Light{
			vec3 position;
			vec3 direction;

			vec3 ambient;
			vec3 diffuse;
			vec3 specular;			

			float constant;
			float linear;
			float quadratic;
			float cutOff;
			float outerCutOff;
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
		vec3 lightDir = normalize(-light.direction);		

		float theta = dot(lightDir, normalize(-light.direction));
		float epsilon = (light.cutOff - light.outerCutOff);
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

		float distance = length(light.direction - FragPos);
		float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic * distance * distance);

		//ambient 
		vec3 ambient = vec3(texture(material.diffuse, texCoord)) * light.ambient; 
		ambient *= attenuation;
		//diffuse			
		vec3 norm = normalize(Normal);
		//vec3 lightDir = normalize(light.position - FragPos);
		
		float diff = max(dot(norm, lightDir), 0.0f);
		vec3 diffuse = vec3(texture(material.diffuse, texCoord)) * diff * light.diffuse;
		diffuse *= intensity;
		diffuse *= attenuation;
		//specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		vec3 specular = vec3(texture(material.specular, texCoord)) * spec * light.specular;
		specular *= intensity;
		specular *= attenuation;
		vec3 result = (ambient + diffuse + specular);
		FragColor = vec4(result, 1.0f);	

	}