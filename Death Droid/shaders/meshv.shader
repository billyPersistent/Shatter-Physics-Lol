#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec4 color;

out vec4 f_Color;
out vec3 f_Pos;
out vec3 f_Normal;

uniform mat4 u_Camera;
uniform mat4 u_Translation;
uniform mat4 u_Rotation;

void main()
{
	f_Color = color;

	vec4 pos4 = u_Translation * u_Rotation * position;
	vec4 norm4 = u_Rotation * normal;

	f_Pos = vec3(pos4.x, pos4.y, pos4.z);
	f_Normal = vec3(norm4.x, norm4.y, norm4.z);

	gl_Position = u_Camera * u_Translation * u_Rotation * position;//reverse dis?
	//f_Pos = vec3(gl_Position.x, gl_Position.y, gl_Position.z);
}