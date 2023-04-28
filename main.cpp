#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <ft2build.h>
//#include FT_FREETYPE_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "opengl.h"

void draw_triangle(GLuint Shader, color Color);
void draw_quad(float X, float Y, float Width, float Height, color Color, GLuint Shader);
GLuint make_texture(const char *File);
void draw_image(GLuint Texture, GLuint Shader);

void OnWindowResized(GLFWwindow *Window, int Width, int Height)
{
	glViewport(0, 0, Width, Height);
}

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

	glfwSetFramebufferSizeCallback(AppWindow, OnWindowResized);

	if(!gladLoadGL()){
		fprintf(stderr, "error: gladLoadGL()\n");
		return 1;
	}

	GLuint Shader = make_color_shader();

	const char *File = "/home/eero/all/no-gtk-text-editor/test/cmake-test/images/snowy_mountains.jpeg";
	GLuint Texture = make_texture(File);
	GLuint TextureShader = make_texture_shader();
	
	while(glfwWindowShouldClose(AppWindow) != true)
	{
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		draw_image(Texture, TextureShader);
////		color Color = {0.5f, 0.3f, 0.7f, 1.0f};
//		draw_triangle(Shader, {0.0f, 1.0f, 1.0f, 1.0f});
//		draw_quad(0.0f, 0.0f, 0.5f, 0.5f, {1.0f, 0.0f, 0.0f, 1.0f}, Shader);

		glfwSwapBuffers(AppWindow);

		glfwPollEvents();
	}

	return 0;
}

void draw_triangle(GLuint Shader, color Color)
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

void draw_quad(float X, float Y, float Width, float Height, color Color, GLuint Shader)
{
	glUseProgram(Shader);

	float Vertices[] =
	{
		// upper-left:
		X, Y, 0.0f, Color.R, Color.G, Color.B, Color.A,
		// upper-right:
		X + Width, Y, 0.0f, Color.R, Color.G, Color.B, Color.A,
		// lower-right:
		X + Width, Y - Height, 0.0f, Color.R, Color.G, Color.B, Color.A,
		// lower-right:
		X + Width, Y - Height, 0.0f, Color.R, Color.G, Color.B, Color.A,
		// lower-left:
		X, Y - Height, 0.0f, Color.R, Color.G, Color.B, Color.A,
		// upper-left:
		X, Y, 0.0f, Color.R, Color.G, Color.B, Color.A,
	};
	int VerticesSizeBytes = sizeof(Vertices);
	int NumVertices = VerticesSizeBytes / 7 * sizeof(float);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, VerticesSizeBytes, Vertices, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const void *) (0 * sizeof(float)));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const void *) (3 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

GLuint make_texture(const char *File)
{
	int Width, Height, NumChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *Data = stbi_load(File, &Width, &Height, &NumChannels, 0);
	printf("width: %d, height: %d, # channels: %d\n", Width, Height, NumChannels);

	GLuint Texture;
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	// What to do when texture coordinates are outside of the texture:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// What to do when the texture is minified/magnified:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(
		GL_TEXTURE_2D,            // target
		0,                        // level
		GL_RGB,                   // internal format
		Width,                    // width
		Height,                   // height
		0,                        // must be 0
		GL_RGB,                   // format
		GL_UNSIGNED_BYTE,         // type
		Data                      // data
		);

	stbi_image_free(Data);

	return Texture;
}

void draw_image(GLuint Texture, GLuint Shader)
{
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUseProgram(Shader);

	float X = -1.0f;
	float Y = 1.0f;
	float Width = 2.0f;
	float Height = 2.0f;

	float Vertices[] =
	{
		// upper-left:
		X, Y, 0.0f, 0.0f, 1.0f,
		// upper-right:
		X + Width, Y, 0.0f, 1.0f, 1.0f,
		// lower-right:
		X + Width, Y - Height, 0.0f, 1.0f, 0.0f,
		// lower-right:
		X + Width, Y - Height, 0.0f, 1.0f, 0.0f,
		// lower-left:
		X, Y - Height, 0.0f, 0.0f, 0.0f,
		// upper-left:
		X, Y, 0.0f, 0.0f, 1.0f,
	};
	int VerticesSizeBytes = sizeof(Vertices);
	int NumVertices = VerticesSizeBytes / 5 * sizeof(float);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, VerticesSizeBytes, Vertices, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *) (0 * sizeof(float)));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *) (3 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}


