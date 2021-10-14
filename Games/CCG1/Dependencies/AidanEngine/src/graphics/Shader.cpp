#include <string>
#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "../utils/fileutils.h"
#include <GL/glew.h>
namespace engine
{
	namespace graphics
	{


		Shader::Shader(const char* vertexpath, const char* fragmentpath)
			: m_VertPath(vertexpath), m_FragPath(fragmentpath), m_RendererID(0)

		{
			ShaderProgramSource source;
			source.VertexSource = read_file(vertexpath).c_str();
			source.FragmentSource = read_file(fragmentpath).c_str();
		
			m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
		}

		Shader::~Shader()
		{
			glDeleteProgram(m_RendererID);
		}

		unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
		{
			unsigned int id = glCreateShader(type);
			const char* src = source.c_str();
			glShaderSource(id, 1, &src, NULL);
			glCompileShader(id);

			int result;
			glGetShaderiv(id, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE)
			{
				int length;
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
				char* message = (char*)alloca(length * sizeof(char));
				glGetShaderInfoLog(id, length, &length, message);
				std::cout << "Failed to Compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "Shader" << std::endl;
				std::cout << message << std::endl;
				glDeleteShader(id);
				return 0;
			}

			return id;
		}
		unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
		{
			unsigned int program = glCreateProgram();
			unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
			unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

			glAttachShader(program, vs);
			glAttachShader(program, fs);
			glLinkProgram(program);
			glValidateProgram(program);

			glDeleteShader(vs);
			glDeleteShader(fs);

			return program;
		}

		void Shader::Bind() const
		{
			glUseProgram(m_RendererID);
		}
		void Shader::Unbind() const
		{
			glUseProgram(0);
		}
		//CHANGE TO CHAR POINTERS
		void Shader::SetUniform1iv(const std::string& name, int count, int* value)
		{
			glUniform1iv(GetUniformLocation(name), count, value);
		}
		void Shader::SetUniform1fv(const std::string& name, int count, float* value)
		{
			glUniform1fv(GetUniformLocation(name), count, value);
		}
		void Shader::SetUniformMat4f(const std::string& name, const maths::mat4 matrix)
		{
			glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix.elements);
		}
		void Shader::SetUniform1i(const std::string& name, int v)
		{
			glUniform1i(GetUniformLocation(name), v);
		}
		void Shader::SetUniform1f(const std::string& name, float v)
		{
			glUniform1f(GetUniformLocation(name), v);
		}
		void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
		{
			glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
		}
		int Shader::GetUniformLocation(const std::string& name)
		{
			if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			{
				return m_UniformLocationCache[name];
			}
			int location = glGetUniformLocation(m_RendererID, name.c_str());
			if (location == -1)
			{
				std::cout << "Warning uniform '" << name << "' doesn't exist!" << std::endl;
			}

			m_UniformLocationCache[name] = location;
			return location;
		}
	}
}