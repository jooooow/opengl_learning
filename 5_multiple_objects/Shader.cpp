#include "Shader.h"
#include <GL/glew.h>
#include <fstream>
#include <string>

Shader::Shader(const std::string& vertex_shader, const std::string& fragment_shader)
{
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, GetShaderSource(vertex_shader));
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, GetShaderSource(fragment_shader));
	rid = glCreateProgram();

	glAttachShader(rid, vs);
	glAttachShader(rid, fs);
	glLinkProgram(rid);
	glValidateProgram(rid);
	glDeleteShader(vs);
	glDeleteShader(fs);
}

Shader::~Shader()
{
	glDeleteProgram(rid);
}

void Shader::Bind()
{
	glUseProgram(rid);
}

void Shader::UnBind()
{
	glUseProgram(0);
}

int Shader::SetUniformFloat1(const std::string& name, float v1)
{
	int location = GetUniformLocation(name);
	if (location == -1)
	{
		return 0;
	}

	glUniform1f(location, v1);

	return 1;
}

int Shader::SetUniformFloat2(const std::string& name, float v1, float v2)
{
	int location = GetUniformLocation(name);
	if (location == -1)
	{
		return 0;
	}

	glUniform2f(location, v1, v2);

	return 1;
}

int Shader::SetUniformFloat3(const std::string& name, float v1, float v2, float v3)
{
	int location = GetUniformLocation(name);
	if (location == -1)
	{
		return 0;
	}

	glUniform3f(location, v1, v2, v3);

	return 1;
}

int Shader::SetUniformFloat4(const std::string& name, float v1, float v2, float v3, float v4)
{
	int location = GetUniformLocation(name);
	if (location == -1)
	{
		return 0;
	}
	
	glUniform4f(location, v1, v2, v3, v4);

	return 1;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& src)
{
	GLuint id = glCreateShader(type);
	const char* code = src.c_str();

	glShaderSource(id, 1, &code, nullptr);
	glCompileShader(id);

	int flag;
	glGetShaderiv(id, GL_COMPILE_STATUS, &flag);
	if (GL_FALSE == flag)
	{
		int len;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
		char* msg = (char*)_malloca(len * sizeof(char));
		glGetShaderInfoLog(id, len, &len, msg);

		printf("compile %s shader failed!\n", type == GL_VERTEX_SHADER ? "vertex" : "fragment");

		glDeleteShader(id);
		return 0;
	}

	return id;
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (uniform_location_cache.find(name) != uniform_location_cache.end())
		return uniform_location_cache[name];

	int location = glGetUniformLocation(rid, name.c_str());
	uniform_location_cache[name] = location;

	return location;
}

std::string Shader::GetShaderSource(std::string filepath)
{
	std::ifstream stream(filepath);
	std::string source = "";
	std::string line;

	while (std::getline(stream, line))
	{
		source += line + "\n";
	}

	return source;
}