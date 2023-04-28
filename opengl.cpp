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

GLuint make_color_shader()
{
	const char *VertexShader = R"(
		#version 330 core
		
		layout(location = 0) in vec3 Position;
		layout(location = 1) in vec4 vColor;
		
		out vec4 fColor;
		
		void main()
		{
			gl_Position = vec4(Position, 1.0);
			fColor = vColor;
		}
	)";
	const char *FragmentShader = R"(
		#version 330 core
		
		in vec4 fColor;
		out vec4 Color;
		
		void main()
		{
			Color = fColor;
//			Color = vec4(0.0, 0.0, 1.0, 1.0);
		}
	)";
	return make_shader(VertexShader, FragmentShader);
}