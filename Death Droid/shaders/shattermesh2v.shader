#version 430 core

layout(location = 0) in vec4 position;

out vec3 f_PreTransPos;

uniform mat4 u_Camera;
uniform mat4 u_Translation;
uniform mat4 u_Rotation;

uniform float u_ResolutionDivide;

void main()
{
	f_PreTransPos = position.xyz;

	gl_Position = u_Camera * u_Translation * u_Rotation * position;
	gl_Position.xy /= u_ResolutionDivide;
}