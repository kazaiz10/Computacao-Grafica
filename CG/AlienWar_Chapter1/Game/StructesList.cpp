#include "StructesList.h"
StructesList::StructesList() {
	this->model = glm::mat4(1.0f);
	this->Position = glm::vec3(0.0f);
	this->ID = NULL;
	this->Prox = NULL;
}

StructesList::StructesList(glm::vec3 Pos, glm::mat4 Model, short ID) {
	this->model = glm::translate(glm::mat4(1.0f), Pos);
	this->Position = Pos;
	this->ID = ID;
	this->Prox = NULL;
}

void StructesList::insert(glm::vec3 Pos, glm::mat4 Model, short ID) {
	if (this->ID == NULL) {
		this->model = glm::translate(glm::mat4(1.0f), Pos);
		this->Position = Pos;
		this->ID = ID;
	}
	else {
		if (this->Prox != NULL) this->Prox->insert(Pos, Model, ID);
		else this->Prox = new StructesList(Pos, Model, ID);
	}
}

StructesList* StructesList::getIndex(short index) {
	if (!this) return NULL;
	else if (index == 0) return this;
	else {
		if (this->Prox == NULL) return NULL;
		else this->Prox->getIndex(index - 1);
	}
}

short StructesList::getSize() {
	if (!this) return 0;
	else {
		if (this->Prox == NULL) return 1;
		else return 1 + this->Prox->getSize();
	}
}

void StructesList::insert(List Coords, List Tree) {
	for (short index = 0; index < Coords.getsize() ; index = index + 2) {
		if (Tree.getindex(index / 2) != NULL)	this->insert(glm::vec3(Coords.getindex(index + 1), 0.0f, Coords.getindex(index)), glm::mat4(1.0f), Tree.getindex(index / 2));
		else	this->insert(glm::vec3(Coords.getindex(index + 1), 0.0f, Coords.getindex(index)), glm::mat4(1.0f), 1000 + index);
	}
}

void StructesList::insert(List Coords) {
	for (short index = 0; index < Coords.getsize() / 2; index = index + 2) {
		printf("Hello insert\n");
		this->insert(glm::vec3(Coords.getindex(index + 1), 0.0f, Coords.getindex(index)), glm::mat4(1.0f), 1000 + index);
	}
}

void StructesList::ApplyShader() {
	if (!this) return;
	else {
		this->getShader()->setMat4("model", glm::translate(glm::mat4(1.0f), this->getPosition()));
		if (this->Prox != NULL) this->Prox->ApplyShader();
	}
}