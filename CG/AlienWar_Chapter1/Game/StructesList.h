#include "Structes.h"
#include "List.h"
#pragma once
class StructesList : public Structes
{
private:
	StructesList* Prox;
public:
	StructesList();
	StructesList(glm::vec3 Pos, glm::mat4 Model, short ID);
	void insert(glm::vec3 Pos, glm::mat4 Model, short ID);
	StructesList* getIndex(short index);
	short getSize();
	void insert(List Coords, List Tree);
	void insert(List Coords);
	void ApplyShader();
};

