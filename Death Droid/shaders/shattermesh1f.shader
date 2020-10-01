#version 430 core

layout(location = 0) out vec4 color;

//layout(binding = 1, offset = 0) uniform atomic_uint fragCount;

in vec4 f_Color;
in vec3 f_Pos;
in vec3 f_Normal;

in vec3 f_PreTransPos;
in float f_Slice;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;

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
	//atomicCounterIncrement(fragCount);

	if (f_Slice > 0.0)
	{
		for (int i = 0; i < u_PlaneCount; i++)
		{
			if (dot(f_PreTransPos - getPlanePos(u_TBOSampler, i), getPlaneNorm(u_TBOSampler, i)) > 0.000001)
			{
				discard;
			}
		}
	}

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