#include "ListBufferes.h"

ListBufferes::ListBufferes() {
	this->vertex = NULL;
	this->VAO = NULL;
	this->VBO = NULL;
	this->Prox = NULL;
	this->VertexSizeOf = NULL;
}

ListBufferes::ListBufferes(float* vertex, int size) {
	this->vertex = vertex;
	this->VAO = NULL;
	this->VBO = NULL;
	this->Prox = NULL;
	this->VertexSizeOf = size;
}

void ListBufferes::add(float* vertex, int size) {
	if (this->vertex == NULL) {
		this->vertex = vertex;
		this->VertexSizeOf = size;
	}
	else if (this->Prox == NULL) this->Prox = new ListBufferes(vertex, size);
	else this->Prox->add(vertex, size);
}

ListBufferes* ListBufferes::getIndex(short index) {
	if (this == NULL) return NULL;
	else if (index == 0) return this;
	else return this->Prox->getIndex(index - 1);
}

GLuint* ListBufferes::getVAO() { return &this->VAO; }
GLuint* ListBufferes::getVBO() { return &this->VBO; }
float* ListBufferes::getVertex() { return this->vertex; }

int ListBufferes::getVertexSizeof() { return this->VertexSizeOf; }

void ListBufferes::TransGpu(Shader sh) {
	glGenVertexArrays(1, this->getVAO());
	glGenBuffers(1, this->getVBO());

	glBindBuffer(GL_ARRAY_BUFFER, *this->getVAO());
	glBufferData(GL_ARRAY_BUFFER, this->VertexSizeOf, this->getVertex(), GL_STATIC_DRAW);

	glBindVertexArray(*this->getVAO());
	
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}
