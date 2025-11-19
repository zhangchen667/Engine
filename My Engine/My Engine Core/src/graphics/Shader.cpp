#include"Shader.h"
namespace myarcane {
	namespace graphics
	{
		Shader::Shader(const char* vertPath, const char* fragPath) :m_VertPath(vertPath), m_FragPath(fragPath) {
			m_ShaderID = load();
		}
		Shader::~Shader() {
			glDeleteProgram(m_ShaderID);
		}
		GLuint Shader::load() {
			//初始化着色器
			GLuint program = glCreateProgram();
			GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
			GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

			//读取着色器代码
			std::string vertSourceString = FileUtils::readFile(m_VertPath);
			std::string fragSourceString = FileUtils::readFile(m_FragPath);
			const char* vertSource = vertSourceString.c_str();
			const char* fragSource = fragSourceString.c_str();

			//编译顶点着色器
			glShaderSource(vertex, 1, &vertSource, NULL);
			glCompileShader(vertex);
			GLint result;

			//检查顶点着色器编译错误
			glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE) {
				GLint length;
				glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length);
				std::vector<char> error(length);
				glGetShaderInfoLog(vertex, length, &length, &error[0]);
				std::cout << "Failed to Compile Vertex Shader" << std::endl << &error[0] << std::endl;
				glDeleteShader(vertex);
				return 0;
			}

			//编译片段着色器
			glShaderSource(fragment, 1, &fragSource, NULL);
			glCompileShader(fragment);

			//检查片段着色器编译错误
			glGetShaderiv(fragment, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE) {
				GLint length;
				glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &length);
				std::vector<char> error(length);
				glGetShaderInfoLog(fragment, length, &length, &error[0]);
				std::cout << "Failed to Compile Fragment Shader" << std::endl << &error[0] << std::endl;
				glDeleteShader(fragment);
				return 0;
			}

			//链接着色器程序
			glAttachShader(program, vertex);
			glAttachShader(program, fragment);
			glLinkProgram(program);
			glValidateProgram(program);

			//使用完后删除着色器
			glDeleteShader(vertex);
			glDeleteShader(fragment);
			//返回着色器程序ID
			return program;
		}

		GLint Shader::getUniformLocation(const GLchar* name) {
			return glGetUniformLocation(m_ShaderID, name);
		}

		void Shader::setUniform1f(const GLchar* name, GLfloat value) {
			glUniform1f(getUniformLocation(name), value);
		}
		void Shader::setUniform1i(const GLchar* name, int value) {
			glUniform1i(getUniformLocation(name), value);
		}

		void Shader::setUniform2f(const GLchar* name, const glm::vec2& vector) {
			glUniform2f(getUniformLocation(name), vector.x, vector.y);
		}

		void Shader::setUniform3f(const GLchar* name, const glm::vec3& vector) {
			glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
		}

		void Shader::setUniform4f(const GLchar* name, const glm::vec4& vector) {
			glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
		}

		void Shader::setUniformMat4(const GLchar* name, const glm::mat4& matrix) {
			glUniformMatrix4fv(glGetUniformLocation(m_ShaderID, name), 1, GL_FALSE, glm::value_ptr(matrix));
		}
		void Shader::enable()const {
			glUseProgram(m_ShaderID);
		}
		void Shader::disable()const {
			glUseProgram(0);
		}
	}
}