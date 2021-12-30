#pragma once

#include <iostream>
#include <unordered_map>

class Shader
{
public:
	Shader(const std::string& vertex_shader, const std::string& fragment_shader);
	~Shader();

	void Bind();
	void UnBind();

	int SetUniformFloat1(const std::string& name, float v1);
	int SetUniformFloat2(const std::string& name, float v1, float v2);
	int SetUniformFloat3(const std::string& name, float v1, float v2, float v3);
	int SetUniformFloat4(const std::string& name, float v1, float v2, float v3, float v4);

private:
	unsigned int rid;
	std::unordered_map<std::string, int> uniform_location_cache;

	std::string GetShaderSource(std::string filepath);
	unsigned int CompileShader(unsigned int type, const std::string& src);
	int GetUniformLocation(const std::string& name);
};

