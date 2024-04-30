#version 460 core
layout (location = 0) out vec4 fragColor;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;
in vec2 texCoords;

uniform float gamma;
uniform float exposure;
uniform float contrast;
uniform float bloomStrength = 0.04f;
uniform bool useBloom;
uniform bool usePostProcessing;

vec3 saturateVec3(vec3 color)
{
	float r = clamp(color.r, 0.0f, 1.0f);
	float g = clamp(color.g, 0.0f, 1.0f);
	float b = clamp(color.b, 0.0f, 1.0f);

	return vec3(r, g, b);
}

vec3 acesTonemapping(vec3 x)
{
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;
  return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

void main()
{
	vec3 screenColor = texture(screenTexture, texCoords).rgb;

		// bloom
	if(useBloom)
	{
		vec3 bloomColor = texture(bloomTexture, texCoords).rgb;
		screenColor.rgb = mix(screenColor.rgb, bloomColor, bloomStrength);
	}

	// tone mapping - reinhard
	//screenColor = screenColor / (screenColor + vec3(1.0f));

	// tone mapping - aces
	if(usePostProcessing)
	{
		screenColor = acesTonemapping(screenColor);
	}

		// exposure
		screenColor = vec3(1.0f) - exp(-screenColor * exposure);

		// gamma
		screenColor = pow(screenColor, vec3(1.0f / gamma));

	fragColor = vec4(screenColor, 1.0f);

	if(usePostProcessing)
	{
		// contrast
		fragColor.rgb = (fragColor.rgb - 0.5f) * (1.0f + contrast) + 0.5f;
	}
}	