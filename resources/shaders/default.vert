#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormals;
layout (location = 3) in vec3 aTangents;

// uniforms
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 vertexPosition;
out vec2 texCoords;
out vec3 normals;
out vec3 tangents;
out vec3 fragPosition;
out mat3 TBN;
out vec3 viewPosition;
out vec4 clipSpace;

uniform vec4 plane;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	gl_ClipDistance[0] = dot(model * vec4(aPos, 1.0f), plane);

	clipSpace = projection * view * model * vec4(aPos, 1.0f);

	vertexPosition = aPos;
	texCoords = aTexCoords;
	normals = mat3(transpose(inverse(model))) * aNormals;
	fragPosition = vec3(model * vec4(aPos, 1.0f));
	tangents = mat3(transpose(inverse(model))) * aTangents;
	viewPosition = vec4(view * model * vec4(aPos, 1.0f)).xyz;

	 vec3 T = normalize(vec3(transpose(inverse(model)) * vec4(aTangents, 0.0f)));
	 vec3 N = normalize(vec3(transpose(inverse(model)) * vec4(aNormals, 0.0f)));
	 T = normalize(T - dot(T, N) * N);
	 vec3 B = cross(N, T);
	 TBN = transpose(mat3(T, B, N));

	 //fragPosLightSpace = lightSpaceMatrix * vec4(fragPosition, 1.0f);
}