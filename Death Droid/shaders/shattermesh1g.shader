#version 430 core

//triangle_strip?
layout(triangles) in;
layout(triangle_strip, max_vertices = 255) out;

in vec4 g_Color[3];
in vec3 g_Pos[3];
in vec3 g_Normal[3];
in vec3 g_PreTransPos[3];

out vec4 f_Color;
out vec3 f_Pos;
out vec3 f_Normal;

out vec3 f_PreTransPos;
out float f_Slice;

uniform vec3 u_CamPos;
uniform vec3 u_CamDir;
uniform mat4 u_Translation;
uniform mat4 u_Rotation;

uniform int u_PlaneCount;
uniform samplerBuffer u_TBOSampler;

vec3 getPlanePos(samplerBuffer s, int i)
{
	return texelFetch(s, i * 9).xyz;
}

vec3 getPlaneNorm(samplerBuffer s, int i)
{
	return texelFetch(s, i * 9 + 1).xyz;
}

void main()
{
	if (dot(u_CamPos - g_Pos[0], g_Normal[0]) > 0.0)
	{
		bool someTriangle = true;
		f_Slice = 0.0;

		//could I reduce the number of loops by comparing a given pair once instead of twice?
		//how? does that make sense?
		for (int i = 0; i < u_PlaneCount; i++)
		{
			if (dot(g_PreTransPos[0] - getPlanePos(u_TBOSampler, i), getPlaneNorm(u_TBOSampler, i)) > 0.000001 ||
				dot(g_PreTransPos[1] - getPlanePos(u_TBOSampler, i), getPlaneNorm(u_TBOSampler, i)) > 0.000001 ||
				dot(g_PreTransPos[2] - getPlanePos(u_TBOSampler, i), getPlaneNorm(u_TBOSampler, i)) > 0.000001)
			{
				f_Slice = 1.0;

				if (dot(g_PreTransPos[0] - getPlanePos(u_TBOSampler, i), getPlaneNorm(u_TBOSampler, i)) > 0.000001 &&
					dot(g_PreTransPos[1] - getPlanePos(u_TBOSampler, i), getPlaneNorm(u_TBOSampler, i)) > 0.000001 &&
					dot(g_PreTransPos[2] - getPlanePos(u_TBOSampler, i), getPlaneNorm(u_TBOSampler, i)) > 0.000001)
				{
					someTriangle = false;
				}
			}
		}

		if (someTriangle)
		{
			gl_Position = gl_in[0].gl_Position;
			f_Color = g_Color[0];
			f_Pos = g_Pos[0];
			f_Normal = g_Normal[0];
			f_PreTransPos = g_PreTransPos[0];
			EmitVertex();

			gl_Position = gl_in[1].gl_Position;
			f_Color = g_Color[1];
			f_Pos = g_Pos[1];
			f_Normal = g_Normal[1];
			f_PreTransPos = g_PreTransPos[1];
			EmitVertex();

			gl_Position = gl_in[2].gl_Position;
			f_Color = g_Color[2];
			f_Pos = g_Pos[2];
			f_Normal = g_Normal[2];
			f_PreTransPos = g_PreTransPos[2];
			EmitVertex();
		}
	}
}