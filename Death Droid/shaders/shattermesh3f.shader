#version 430 core

layout(location = 0) out vec4 color;

//layout(binding = 1, offset = 0) uniform atomic_uint fragCount;

in vec4 f_Color;
in vec3 f_Pos;
in vec3 f_Normal;

uniform float u_ResolutionDivide;
uniform vec2 u_WindowSize;
uniform sampler2D u_DepthTexture;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;

vec2 screenToDepthTexture(vec2 pos)
{
	//0-1 range for texture coordinates
	pos.x /= u_WindowSize.x;
	pos.y /= u_WindowSize.y;

	//convert to NDC, divide, and convert back to 0-1 range
	pos = (((2 * pos - 1) / u_ResolutionDivide) + 1) / 2;

	return pos;
}

//bottom left of pixels?
/*vec2 depthTextureToScreen(vec2 pos)
{
	//screenToDepthTexture backwards
	pos = (((2 * pos - 1) * u_ResolutionDivide) + 1) / 2;

	pos.x *= u_WindowSize.x;
	pos.y *= u_WindowSize.y;

	return pos;
}

vec2 depthTextureToScreenMiddle(vec2 pos)
{
	//middle rounding
	pos.x = (int(pos.x * u_WindowSize.x) + 0.5) / u_WindowSize.x;
	pos.y = (int(pos.y * u_WindowSize.y) + 0.5) / u_WindowSize.y;

	//screenToDepthTexture backwards
	pos = (((2 * pos - 1) * u_ResolutionDivide) + 1) / 2;

	pos.x *= u_WindowSize.x;
	pos.y *= u_WindowSize.y;

	return pos;
}*/

void main()
{
	vec2 fragTexCoord = screenToDepthTexture(gl_FragCoord.xy);

	if (distance(gl_FragCoord.z, texture(u_DepthTexture, fragTexCoord).r) > 0.00009 * u_ResolutionDivide)
	{
		discard;
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