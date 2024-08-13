#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_m.h>
#pragma once
class ListBufferes
{
private:
	GLuint VBO, VAO;
	ListBufferes* Prox;
	int VertexSizeOf;
public:
	float* vertex;
	ListBufferes();
	ListBufferes(float* vertex, int size);
	void add(float* vertex, int size);
	ListBufferes* getIndex(short index);
	GLuint* getVAO();
	GLuint* getVBO();
	float* getVertex();
	int getVertexSizeof();
	void TransGpu(Shader sh);
};