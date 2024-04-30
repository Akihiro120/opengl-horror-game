#version 460 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gTangents;
layout (location = 3) out vec4 gAlbedo;
layout (location = 4) out vec4 gRoughMetalEmissive;
layout (location = 5) out vec4 gViewPosition;
layout (location = 6) out vec4 gTrueNormals;


// inputs
in vec3 vertexPosition;
in vec2 texCoords;
in vec3 normals;
in vec3 tangents;
in vec3 fragPosition;
in mat3 TBN;
in vec3 viewPosition;

// textures
uniform float tiling;
uniform bool useAlbedoTexture;
uniform bool useNormalTexture;
uniform sampler2D albedoTexture;
uniform sampler2D normalTexture;
uniform sampler2D microTexture;
uniform sampler2D dudvTexture;

uniform bool useEmissive;
uniform float emissiveStrength;

uniform vec3 baseColor;
uniform float roughness;
uniform float metallic;
uniform bool useParallax;

in vec4 clipSpace;
uniform bool renderAsWater;
uniform float time;

vec3 microVariation(sampler2D textureColor)
{
	float microTiling = 0.5f;
	float microVariationTiling = 0.0f;
	float microVariationAlphaTiling = 0.05f;

	vec3 microTint = vec3(1.0f);
	vec3 microVariationTint = vec3(0.01f, 0.01f, 0.01f);//vec3(1.0f, 0.95f, 0.5f);

	// micro 
	vec3 microColor = texture(textureColor, texCoords * microTiling).rgb * microTint;
	vec3 microColorVariation = texture(textureColor, texCoords * microVariationTiling).g * microVariationTint;
	vec3 microVariationBlend = texture(microTexture, texCoords * microVariationAlphaTiling).rgb;

	// variation
	float m1 = texture(microTexture, texCoords * 0.005f).r;
	float m2 = texture(microTexture, texCoords * 0.001f).r;
	float m105 = m1 + 0.5f;
	float m205 = m2 + 0.5f;
	float m12 = m1 * m2;

	float micro = mix(0.5f, 0.0f, m12);
	vec3 color = mix(microColor, microColorVariation, microVariationBlend.r);

	return micro * color;
}

vec3 microVariationNormals(sampler2D textureColor)
{
	float microTiling = 0.5f;
	float microVariationTiling = 0.0f;
	float microVariationAlphaTiling = 0.075f;

	vec3 microTint = vec3(1.0f);
	vec3 microVariationTint = vec3(0.0f, 0.0f, 1.0f);//vec3(1.0f, 0.95f, 0.5f);

	// micro 
	vec3 microColor = texture(textureColor, texCoords * microTiling).rgb * microTint;
	vec3 microColorVariation = texture(textureColor, texCoords * microVariationTiling).g * microVariationTint;
	vec3 microVariationBlend = texture(microTexture, texCoords * microVariationAlphaTiling).rgb;

	// variation
	float m1 = texture(microTexture, texCoords * 0.008f).r;
	float m2 = texture(microTexture, texCoords * 0.001f).r;
	float m105 = m1 + 0.5f;
	float m205 = m2 + 0.5f;
	float m12 = m1 * m2;

	vec3 micro = mix(vec3(0.0f, 0.0f, 1.0f), vec3(1.0f), m12);
	vec3 color = mix(microColor, microColorVariation, microVariationBlend.r);

	return micro * color;
}

void main()
{
	gViewPosition = vec4(viewPosition, 1.0f);
	gPosition = vec4(fragPosition, 1.0f);
	vec3 normalColor = vec3(0.0f);

	vec2 parallaxTexCoords = texCoords;
	if(useNormalTexture)
	{
		if(renderAsWater)
		{
			normalColor = texture(normalTexture, vec2(texCoords.x + (time * 0.015f), texCoords.y) * tiling).rgb;
			normalColor += texture(normalTexture, vec2(texCoords.x, texCoords.y + (time * 0.02f)) * tiling).rgb;
		} else {
		normalColor = texture(normalTexture, vec2(texCoords.x, texCoords.y) * tiling).rgb;
		}
		normalColor = normalColor * 2.0f - 1.0f;
		normalColor = normalize(normalColor);
	} else {
		normalColor = normalize(normals);
	}
	gNormal.rgb = normalColor;
	gNormal.a = useNormalTexture ? 1.0f : 0.0f;
	gTrueNormals = vec4(normalize(normals), 1.0f);
	gTangents = vec4(normalize(tangents), 1.0f);

	if(renderAsWater)
	{
		vec2 distortion1 = (texture(dudvTexture, vec2(texCoords.x + (time * 0.015f), texCoords.y) * tiling).rg * 2.0f - 1.0f) * 0.02f;
		vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;
		vec2 reflectCoords = vec2(ndc.x, -ndc.y);
		reflectCoords += distortion1;

		vec4 albedo = vec4(texture(albedoTexture, reflectCoords).rgb, 1.0f);
		gAlbedo = useAlbedoTexture ? albedo : vec4(baseColor, 1.0f);
	} else {
		vec4 albedo = useParallax ? vec4(microVariation(albedoTexture), 1.0f) : vec4(texture(albedoTexture, texCoords * tiling).rgb, 1.0f);
		gAlbedo = useAlbedoTexture ? albedo : vec4(baseColor, 1.0f);
	}

	gRoughMetalEmissive.r = roughness;
	gRoughMetalEmissive.g = metallic;
	gRoughMetalEmissive.b = useEmissive ? 1.0f : 0.0f;
	gRoughMetalEmissive.a = 1.0f;
}