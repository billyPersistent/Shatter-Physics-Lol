#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec4 color;

out vec4 f_Color;
out vec3 f_Pos;
out vec3 f_Normal;

uniform mat4 u_Camera;
uniform mat4 u_Translation;
uniform mat4 u_Rotation;

//the slow down has something to do with the number of vertices, it barely happens on the cube
void main()
{
	f_Color = color;

	f_Pos = (u_Translation * u_Rotation * position).xyz;
	f_Normal = (u_Rotation * normal).xyz;

	gl_Position = u_Camera * u_Translation * u_Rotation * position;
}