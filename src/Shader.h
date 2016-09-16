#ifndef SHADER_H
#define SHADER_H
#include <SFML/OpenGL.hpp>
class Shader
{
	public:
		Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
		~Shader();
		inline void use() { glUseProgram(this->program); }

	private:
		GLuint program;		// The program ID
};
#endif
