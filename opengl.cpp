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

GLuint make_texture_shader()
{
	const char *VertexShader = R"(
		#version 330 core
		
		layout(location = 0) in vec3 Position;
		layout(location = 1) in vec2 vTex;
		
		out vec2 fTex;
		
		void main()
		{
			gl_Position = vec4(Position, 1.0);
			fTex = vTex;
		}
	)";
	const char *FragmentShader = R"(
		#version 330 core
		
		in vec2 fTex;
		out vec4 Color;
		
		uniform sampler2D s;
		
		void main()
		{
			Color = texture(s, fTex);
		}
	)";
	return make_shader(VertexShader, FragmentShader);
}

GLuint make_SDF_shader()
{
	const char *VertexShader = R"(
		#version 330 core
		
		layout(location = 0) in vec3 Position;
		layout(location = 1) in vec2 vTex;
		
		out vec2 fTex;
		
		void main()
		{
			gl_Position = vec4(Position, 1.0);
			fTex = vTex;
		}
	)";
	const char *FragmentShader = R"(
		#version 330 core

		in vec2 fTex;
		out vec4 Color;

		uniform sampler2D s;

		const float smoothing = 1.0 / 16.0;

		vec4 get_smooth(vec3 Color, float Distance)
		{
			return vec4(Color, smoothstep(0.5 - smoothing, 0.5 + smoothing, Distance));
		}

		vec4 get_smooth_with_background(vec4 Color, vec4 Background, float Distance)
		{
			float T = smoothstep(0.5 - smoothing, 0.5 + smoothing, Distance);
			return mix(Background, Color, T);
		}

		vec4 get_crisp(vec3 Color, float Distance)
		{
			float Alpha = (Distance < 0.5) ? 0.0 : 1.0;
			return vec4(Color, Alpha);
		}

		vec4 get_sdf(float Distance)
		{
			return vec4(1.0, 1.0, 1.0, Distance);
		}

		void main()
		{
			float Distance = texture(s, fTex).r;

//			Color = get_sdf(Distance);
//			Color = get_crisp(vec3(1.0, 1.0, 1.0), Distance);
//			Color = get_smooth(vec3(1.0, 1.0, 1.0), Distance);
			Color = get_smooth_with_background(vec4(1.0, 0.0, 0.0, 1.0), vec4(0.0, 0.0, 1.0, 1.0), Distance);
		}
	)";
	return make_shader(VertexShader, FragmentShader);
}

GLuint make_text_shader()
{
	const char *VertexShader = R"(
		#version 330 core
	
		layout(location = 0) in vec3 Position;
		layout(location = 1) in vec2 vTex;
	
		out vec2 fTex;
	
		void main()
		{
			gl_Position = vec4(Position, 1.0);
			fTex = vTex;
		}
	)";

	const char *FragmentShader = R"(
		#version 330 core
		
		in vec2 fTex;
		
		out vec4 Color;
		
		uniform sampler2D s;
		
		void main()
		{
			Color = vec4(1.0, 1.0, 1.0, texture(s, fTex).r);
//			Color = vec4(texture(s, fTex).r, 1.0, 1.0, 1.0);
		}
	)";

	return make_shader(VertexShader, FragmentShader);
}