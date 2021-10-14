#pragma once
#include <string>
#include <unordered_map>
#include "../maths/maths.h"
namespace engine {
	namespace graphics {

		struct ShaderProgramSource
		{
			std::string VertexSource;
			std::string FragmentSource;
		};
		class Shader
		{
		private:
			unsigned int m_RendererID;
			const char *m_VertPath;
			const char *m_FragPath;
			std::unordered_map<std::string, int> m_UniformLocationCache;
		public:
			Shader(const char* vertexpath, const char* fragmentpath);
			~Shader();

			void Bind() const;
			void Unbind() const;

			//Set Uniforms
			void SetUniform1iv(const std::string& name, int count, int* value);
			void SetUniform1fv(const std::string& name, int count, float* value);
			void SetUniformMat4f(const std::string& name, const maths::mat4 matrix);
			void SetUniform1i(const std::string& name, int v);
			void SetUniform1f(const std::string& name, float v);
			void SetUniform2f(const std::string& name, float v0, float v1);
			void SetUniform3f(const std::string& name, float v0, float v1, float v2);
			void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		private:
			int GetUniformLocation(const std::string & name);
			unsigned int CompileShader(unsigned int type, const std::string & source);
			unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		};
	}
}

