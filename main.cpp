#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <ft2build.h>
//#include FT_FREETYPE_H

#include "opengl.h"

GLuint make_color_shader();

int main()
{
	if (glfwInit() == GLFW_FALSE) {
		fprintf(stderr, "error: glfwInit()\n");
		return 1;
	}

//	FT_Library Lib;
//	if (FT_Init_FreeType(&Lib) != FT_Err_Ok) {
//		fprintf(stderr, "error: FT_Init_FreeType()\n");
//		return 1;
//	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int WindowWidth = 300;
	int WindowHeight = 300;
	const char *WindowTitle = "The App Window";
	GLFWwindow *AppWindow = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle, NULL, NULL);

	glfwMakeContextCurrent(AppWindow);

	if(!gladLoadGL()){
		fprintf(stderr, "error: gladLoadGL()\n");
		return 1;
	}

	GLuint Shader = make_color_shader();
	
	while(glfwWindowShouldClose(AppWindow) != true)
	{
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

//		color Color = {0.5f, 0.3f, 0.7f, 1.0f};
		render_triangle(Shader, {0.0f, 1.0f, 1.0f, 1.0f});

		glfwSwapBuffers(AppWindow);

		glfwPollEvents();
	}

	return 0;
}

void render_triangle(GLuint Shader, color Color)
{
	glUseProgram(Shader);

	float Vertices[] =
	{
		+0.0f, +0.5f, 0.0f, Color.R, Color.G, Color.B, Color.A,
		-0.5f, -0.5f, 0.0f, Color.R, Color.G, Color.B, Color.A,
		+0.5f, -0.5f, 0.0f, Color.R, Color.G, Color.B, Color.A,
	};
	int NumBytesVertex = 7 * sizeof(float);
	int NumBytesVertices = sizeof(Vertices);
	int NumVertices = NumBytesVertices / NumBytesVertex;

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, NumBytesVertices, Vertices, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const void *) (0 * sizeof(float)));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const void *) (3 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}


