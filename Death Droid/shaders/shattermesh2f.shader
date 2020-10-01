#version 430 core

layout(location = 0) out vec4 color;

layout(binding = 1, offset = 0) uniform atomic_uint fragCount;

in vec3 f_PreTransPos;

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
	atomicCounterIncrement(fragCount);

	for (int i = 0; i < u_PlaneCount; i++)
	{
		if (dot(f_PreTransPos - getPlanePos(u_TBOSampler, i), getPlaneNorm(u_TBOSampler, i)) > 0.000001)
		{
			discard;
		}
	}

	color = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}