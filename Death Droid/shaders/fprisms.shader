#version 330 core

layout(location = 0) out vec4 color;

in vec4 f_Color;
in vec3 f_Pos;
in vec3 f_Normal;

uniform int u_LightCount;

uniform vec3 u_LightPosition[100];
uniform vec3 u_LightIntensity[100];

//lighting functions

vec3 ldColor(vec3 fragPos, vec3 norm, vec3 lightPos, vec3 intensity, vec4 color)
{
	//diffuse light computation
	vec3 lightV = normalize(lightPos - fragPos);
	vec3 diffuse = intensity * max(dot(lightV, norm), 0.0);

	//final light addition
	vec3 finalColor = vec3(0.0, 0.0, 0.0);

	finalColor.x = color.x * diffuse.x;
	finalColor.y = color.y * diffuse.y;
	finalColor.z = color.z * diffuse.z;

	return finalColor;
}

void main()
{
	//normal uniform rounding

	vec3 normal = f_Normal;

	if (normal.x < 1.0 && normal.x > -1.0)
	{
		normal.x = 0.0;
	}
	if (normal.y < 1.0 && normal.y > -1.0)
	{
		normal.y = 0.0;
	}
	if (normal.z < 1.0 && normal.z > -1.0)
	{
		normal.z = 0.0;
	}

	normal = normalize(normal);

	//ambient light computation

	float ambient = 0.4;
	vec3 finalColor = vec3(f_Color.x, f_Color.y, f_Color.z) * ambient;

	//diffuse light computation

	for (int i = 0; i < u_LightCount; i++)
	{
		finalColor += ldColor(f_Pos, normal, u_LightPosition[i], u_LightIntensity[i], f_Color);
	}

	finalColor.x = min(finalColor.x, f_Color.x);
	finalColor.y = min(finalColor.y, f_Color.y);
	finalColor.z = min(finalColor.z, f_Color.z);

	color = vec4(finalColor, 1.0);
}