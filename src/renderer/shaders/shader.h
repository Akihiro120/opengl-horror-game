#pragma once
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const char* vertexShader, const char* fragmentShader, const char* geometryShader = nullptr);

	void bindProgram();
	void cleanUp();

	// uniforms
	void setMat4(const char* uniform, glm::mat4 matrix);
	void setInts(const char* uniform, int value);
	void setVec2(const char* uniform, glm::vec2 value);
	void setVec3(const char* uniform, glm::vec3 value);
	void setVec4(const char* uniform, glm::vec4 value);
	void setFloats(const char* uniform, float value);

private:
	void shaderErrorLog(unsigned int shader, const char* type);

	unsigned int m_ShaderProgram;
	unsigned int m_VertexShader;
	unsigned int m_GeometryShader;
	unsigned int m_FragmentShader;
};