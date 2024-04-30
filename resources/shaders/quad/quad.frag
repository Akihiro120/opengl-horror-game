#version 460 core
out vec4 fragColor;

in vec3 localPos;
  
uniform sampler2D albedoTexture;
in vec2 texCoords;

void main()
{
    vec3 color = texture(albedoTexture, texCoords).rgb;
    fragColor = vec4(color, 1.0f);;
}