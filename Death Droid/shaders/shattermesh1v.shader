#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec4 color;

out vec4 g_Color;
out vec3 g_Pos;
out vec3 g_Normal;

out vec3 g_PreTransPos;

uniform mat4 u_Camera;
uniform mat4 u_Translation;
uniform mat4 u_Rotation;

//the slow down has something to do with the number of vertices, it barely happens on the cube
void main()
{
	g_Color = color;
	
	g_Pos = (u_Translation * u_Rotation * position).xyz;
	g_Normal = (u_Rotation * normal).xyz;//geometry shader?
	
	g_PreTransPos = position.xyz;

	gl_Position = u_Camera * u_Translation * u_Rotation * position;
}