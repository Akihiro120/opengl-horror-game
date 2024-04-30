#version 460 core
layout (location = 0) out vec4 fragColor;

uniform sampler2D screenTexture;
in vec2 texCoords;

void main()
{
	vec3 screenColor = texture(screenTexture, texCoords).rgb;

	fragColor = vec4(screenColor, 1.0f);
}