#version 460 core
precision highp float;

layout (location = 0) out vec4 fragColor;

uniform mat4 view;
in vec2 texCoords;

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D   brdfLUT;  
uniform float far_plane;

// texture
uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D trueNormalsTexture;
uniform sampler2D tangentTexture;
uniform sampler2D albedoTexture;
uniform sampler2D roughMetalEmissiveTexture;
uniform sampler2D ssaoTexture;
uniform bool useShadows;
uniform bool useSSAO;
uniform bool useFog;
uniform bool usePBR;

uniform bool renderAsWater;

struct Pointlight
{
	vec3 position;
	vec3 color;

	samplerCube shadowMap;
	bool renderShadows;
	float radius;
};

#define numPointLights 16
uniform Pointlight pointLights[numPointLights];
uniform vec3 viewPosition;
uniform int numOfPointLights;

// pbr functions
#define PI 3.14159265359
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}  

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 0.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);   

float ShadowCalculation(Pointlight light, vec3 viewPos, vec3 normal, vec3 fragPosition)
{
	    // get vector between fragment position and light position
    vec3 fragToLight = fragPosition - light.position;
    // use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(light.shadowMap, fragToLight).r;
    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= far_plane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // now test for shadows
	float shadow = 0.0;
	float bias   = 0.15;
	int samples  = 20;
	float viewDistance = length(viewPos - fragPosition);
	float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;  
	for(int i = 0; i < samples; ++i)
	{
		float closestDepth = texture(light.shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
		closestDepth *= far_plane;   // undo mapping [0;1]
		if(currentDepth - bias > closestDepth)
			shadow += 1.0;
	}
	shadow /= float(samples);  

    return 1.0f - shadow;
}

vec3 makePointLight(Pointlight light, vec3 albedoColor, vec4 normalColor, float roughness, float metallic, vec3 fragPosition, mat3 tbn)
{
	vec3 L = normalize((tbn * light.position) - (tbn * fragPosition));
	vec3 V = normalize((tbn * viewPosition) - (tbn * fragPosition));
	vec3 N = normalColor.rgb;
	vec3 H = normalize(V + L);

	// pbr calculation

	// attenuation
	float distance = length(light.position - fragPosition);
	float attenuation = 1.0f / (distance * distance);
	vec3 radiance = light.color * attenuation * 3.0f;

	vec3 F0 = vec3(0.04); 
	F0      = mix(F0, albedoColor, metallic);
	vec3 F  = fresnelSchlick(max(dot(H, V), 0.0), F0);
	float NDF = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);
	vec3 numerator = NDF * G * F;
	float denominator  = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.0001f;
	vec3 specular = numerator / denominator;

	vec3 kS = F;
	vec3 kD = vec3(1.0f) - kS;

	kD *= 1.0f - metallic;

	float NdotL = max(dot(N, L), 0.0);        

	if(light.renderShadows && useShadows)
	{
		return (kD * albedoColor / PI + specular) * radiance * NdotL * ShadowCalculation(light, V, N, fragPosition);
	} else {
		return (kD * albedoColor / PI + specular) * radiance * NdotL;
	}
}

void main()
{
	// texture
	vec3 albedoColor = texture(albedoTexture, texCoords).rgb;
	vec4 normalColor = texture(normalTexture, texCoords);
	vec3 tangentColor = texture(tangentTexture, texCoords).rgb;
	vec3 fragPosition = texture(positionTexture, texCoords).rgb;
	vec4 RMES = texture(roughMetalEmissiveTexture, texCoords);
	float ssaoColor = texture(ssaoTexture, texCoords).r;
	vec3 trueNormals = texture(trueNormalsTexture, texCoords).rgb;

	vec3 T = tangentColor.rgb;
	vec3 N = trueNormals.rgb;
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	mat3 tbn = normalColor.a == 1.0f ? transpose(mat3(T, B, N)) : mat3(1.0f);

	/// light calculations - possible optimizations: do not draw lights when out of range
	vec3 result = vec3(0.0f);
	vec3 color = vec3(0.0f);
	if(usePBR)
	{
		for(int i = 0; i < numOfPointLights; i++)
		{       
			result += makePointLight(pointLights[i], albedoColor, normalColor, RMES.r, RMES.g, fragPosition, tbn);
		}

		vec3 V = normalize((tbn * viewPosition) - (tbn * fragPosition));
		vec3 R = reflect(-V, normalColor.rgb);

		// ambient lighting (we now use IBL as the ambient term)
		vec3 F0 = vec3(0.04); 
		F0 = mix(F0, albedoColor, RMES.g);
		vec3 F = fresnelSchlickRoughness(max(dot(normalColor.rgb, V), 0.0), F0, RMES.r);
    
		vec3 kS = F;
		vec3 kD = 1.0 - kS;
		kD *= 1.0 - RMES.g;	  
    
		vec3 irradiance = texture(irradianceMap, normalColor.rgb).rgb * 0.075f;
		vec3 diffuse      = irradiance * albedoColor;
    
		// sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
		const float MAX_REFLECTION_LOD = 4.0;
		vec3 prefilteredColor = textureLod(prefilterMap, R, RMES.r * MAX_REFLECTION_LOD).rgb * 0.1;    
		vec2 brdf  = texture(brdfLUT, vec2(max(dot(normalColor.rgb, V), 0.0), RMES.r)).rg;
		vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

		vec3 ambient = (kD * diffuse + specular) * (useSSAO ? ssaoColor : 1.0f);

		// attenuation
		color = ambient + result;
	} else 
	{
		for(int i = 0; i < numOfPointLights; i++)
		{    
			float ambientStrength = 0.1f;
			vec3 ambient = (ambientStrength * (pointLights[i].color / 10.0f) * albedoColor) * (useSSAO ? ssaoColor : 1.0f);;

			vec3 lightDir = normalize(tbn * pointLights[i].position - tbn * fragPosition);
			float diff = max(dot(normalColor.rgb, lightDir), 0.0f);
			vec3 diffuse = diff * (pointLights[i].color / 10.0f) * albedoColor;

			float specularStrength = RMES.g;
			vec3 viewDir = normalize(tbn * viewPosition - tbn * fragPosition);
			vec3 reflectDir = reflect(-lightDir, normalColor.rgb);
			vec3 halfwayDir = normalize(lightDir + viewDir);

			float spec = pow(max(dot(normalColor.rgb, halfwayDir), 0.0f), 32.0f);
			vec3 specular = specularStrength * spec * (pointLights[i].color / 10.0f);

			// atteunation
			float constant = 1.0f;
			float linear = 0.14f;
			float quadratic = 0.07f;

			float distance = length(pointLights[i].position - fragPosition);
			float attenuation = 1.0f / (constant + linear * distance + quadratic * (distance * distance));

			ambient *= attenuation;
			diffuse *= attenuation;
			specular *= attenuation;

			result += (ambient + ((diffuse + specular) * (useShadows ? ShadowCalculation(pointLights[i], viewDir, normalColor.rgb, fragPosition) : 1.0f)));
		}
		color = result;
	}

	if(RMES.b == 1.0f)
	{
		fragColor = vec4(albedoColor * RMES.a, 1.0f);
	} else {
		fragColor = vec4(color, 1.0f);
	}

	// apply fog
	if(useFog)
	{
		float gradient = 1.5f;
		float density = 0.009798f;
		vec3 fogColor = vec3(255.0f / 255.0f, 191.0f / 255.0f, 41.0f / 255.0f) * vec3(0.005);

		float distance = length((view * vec4(fragPosition, 1.0f)).rgb);
		float visibility = exp(-pow((distance * density), gradient));
		visibility = clamp(visibility, 0.0f, 1.0f);
		fragColor.rgb = mix(fogColor, fragColor.rgb, visibility);
	}
}