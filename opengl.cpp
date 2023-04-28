#include <stdio.h>
#include "opengl.h"


GLuint make_shader(const char *VertexSrc, const char *FragmentSrc, const char *GeometrySrc/* = NULL*/)
{
	int success;
	char info_log[512];

	GLuint VertexShader   = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(VertexShader, 1, &VertexSrc, NULL);
	glShaderSource(FragmentShader, 1, &FragmentSrc, NULL);

	glCompileShader(VertexShader);
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(VertexShader, sizeof(info_log), NULL, info_log);
		printf("vertex shader error: %s\n", info_log);
		//@ error handling
	}

	glCompileShader(FragmentShader);
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(FragmentShader, sizeof(info_log), NULL, info_log);
		printf("fragment shader error: %s\n", info_log);
		//@ error handling
	}

	GLuint ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);

	if(GeometrySrc)
	{
		GLuint GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(GeometryShader, 1, &GeometrySrc, NULL);
		glCompileShader(GeometryShader);
		glGetShaderiv(GeometryShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(GeometryShader, sizeof(info_log), NULL, info_log);
			printf("geometry shader error: %s\n", info_log);
		//@ error handling
		}
		glAttachShader(ShaderProgram, GeometryShader);
	}

	glLinkProgram(ShaderProgram);

	//@ delete shaders?
//	free(vertexShaderText);
//	free(fragmentShaderText);

	return ShaderProgram;
}

void pass_to_shader(GLuint Shader, const char *Name, int Value)
{
	GLint UniformLocation = glGetUniformLocation(Shader, Name);
	glUniform1i(UniformLocation, Value);
}

void pass_to_shader(GLuint Shader, const char *Name, float Value)
{
	GLint UniformLocation = glGetUniformLocation(Shader, Name);
	glUniform1f(UniformLocation, Value);
}