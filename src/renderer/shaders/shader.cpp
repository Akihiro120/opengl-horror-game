#include "shader.h"
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexShader, const char* fragmentShader, const char* geometryShader)
{
	// read files
	std::ifstream vertexFile;
	std::ifstream fragmentFile;
	std::ifstream geometryFile;

	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	const char* vertexShaderSource;
	const char* fragmentShaderSource;
	const char* geometryShaderSource;

	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	geometryFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vertexFile.open(vertexShader);
		fragmentFile.open(fragmentShader);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vertexFile.rdbuf();
		fShaderStream << fragmentFile.rdbuf();

		vertexFile.close();
		fragmentFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		if (geometryShader != nullptr)
		{
			geometryFile.open(geometryShader);
			std::stringstream gShaderStream;
			gShaderStream << geometryFile.rdbuf();
			geometryFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Failed to Read File Path" << std::endl;
	}

	vertexShaderSource = vertexCode.c_str();
	fragmentShaderSource = fragmentCode.c_str();
	geometryShaderSource = geometryCode.c_str();

	// create the vertex shader
	m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_VertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(m_VertexShader);
	shaderErrorLog(m_VertexShader, "Vertex");


	// create the fragment shader
	m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_FragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(m_FragmentShader);
	shaderErrorLog(m_FragmentShader, "Fragment");

	// create the geometry shader
	if (geometryShader != nullptr)
	{
		m_GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(m_GeometryShader, 1, &geometryShaderSource, nullptr);
		glCompileShader(m_GeometryShader);
		shaderErrorLog(m_GeometryShader, "Geometry");
	}

	// create the shader program
	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, m_VertexShader);
	glAttachShader(m_ShaderProgram, m_FragmentShader);
	if (geometryShader != nullptr)
	{
		glAttachShader(m_ShaderProgram, m_GeometryShader);
	}
	glLinkProgram(m_ShaderProgram);
}

void Shader::bindProgram()
{
	// bind the shader program as the current
	glUseProgram(m_ShaderProgram);
}

void Shader::shaderErrorLog(unsigned int shader, const char* type)
{
	int success;
	char infoLog[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	glGetShaderInfoLog(shader, 512, NULL, infoLog);
	if (!success)
	{
		if (type == "Vertex") std::cout << "Failed to Compile Vertex Shader: \n" << infoLog << std::endl;
		if (type == "Fragment") std::cout << "Failed to Compile Fragment Shader: \n" << infoLog << std::endl;
		if (type == "Geometry") std::cout << "Failed to Compile Geometry Shader: \n" << infoLog << std::endl;
	}
}

void Shader::cleanUp()
{
	// destroy the vertex and fragment shader upon destruction
	glDeleteShader(m_VertexShader);
	glDeleteShader(m_FragmentShader);
}

void Shader::setMat4(const char* uniform, glm::mat4 matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgram, uniform), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setInts(const char* uniform, int value)
{
	glUniform1i(glGetUniformLocation(m_ShaderProgram, uniform), value);
}

void Shader::setVec4(const char* uniform, glm::vec4 value)
{
	glUniform4fv(glGetUniformLocation(m_ShaderProgram, uniform), 1, &value[0]);
}

void Shader::setVec3(const char* uniform, glm::vec3 value)
{
	glUniform3fv(glGetUniformLocation(m_ShaderProgram, uniform), 1, &value[0]);
}

void Shader::setVec2(const char* uniform, glm::vec2 value)
{
	glUniform2fv(glGetUniformLocation(m_ShaderProgram, uniform), 1, &value[0]);
}

void Shader::setFloats(const char* uniform, float value)
{
	glUniform1f(glGetUniformLocation(m_ShaderProgram, uniform), value);
}