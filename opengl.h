#include <glad/glad.h>


GLuint make_shader(const char *VertexSrc, const char *FragmentSrc, const char *GeometrySrc = NULL);
void pass_to_shader(GLuint Shader, const char *Name, int Value);
void pass_to_shader(GLuint Shader, const char *Name, float Value);
