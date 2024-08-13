#include "Structes.h"

Structes::Structes() {
	this->model = glm::mat4(1.0f);
	this->Position = glm::vec3(0.0f);
	this->ID = NULL;
	this->ShaderStruct = new Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs");
}

glm::mat4 Structes::getModel() { return this->model; }
glm::vec3 Structes::getPosition() { return this->Position; }
Shader* Structes::getShader() { return this->ShaderStruct; }
short Structes::getID() { return this->ID; }

void Structes::setID(short id) { this->ID = id; }
void Structes::setPosition(glm::vec3 Pos) { this->Position = Pos; }
void Structes::setModel(glm::mat4 model) { this->model = model; }

void Structes::move(glm::vec3 Pos) {
	this->model = glm::translate(this->model, Pos);
	this->Position = this->Position + Pos;
}