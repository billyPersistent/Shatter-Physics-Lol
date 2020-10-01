#version 330 core

layout(location = 0) out vec4 color;

in vec4 f_Color;
in vec3 f_Pos;
in vec3 f_Normal;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;

void main()
{
	//diffuse light computation
	vec3 lightV = normalize(u_LightPos - f_Pos);
	vec3 diffuse = u_LightColor * max(dot(lightV, f_Normal), 0.0);

	//final light addition
	vec3 finalColor = vec3(0.0, 0.0, 0.0);

	finalColor.x = f_Color.x * diffuse.x;
	finalColor.y = f_Color.y * diffuse.y;
	finalColor.z = f_Color.z * diffuse.z;

	color = vec4(finalColor, 1.0f);
}