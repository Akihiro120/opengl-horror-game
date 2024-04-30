#version 460 core
layout (location = 0) out vec4 fragColor;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;
in vec2 texCoords;

uniform float bloomStrength;

void main()
{
	vec3 screenColor = texture(screenTexture, texCoords).rgb;
	vec3 bloomColor = texture(bloomTexture, texCoords).rgb;

	vec3 finalColor = mix(screenColor.rgb, bloomColor.rgb, bloomStrength);
	fragColor = vec4(finalColor, 1.0f);
}