#include "ListBufferes.h"
#pragma once
class Structes
{
public:
	glm::mat4 model;
	glm::vec3 Position;
	Shader* ShaderStruct;
	short ID;
	Structes();
	glm::mat4 getModel();
	glm::vec3 getPosition();
	short getID();
	void setModel(glm::mat4 model);
	void setPosition(glm::vec3 Pos);
	void setID(short id);
	void move(glm::vec3 Pos);
	Shader* getShader();
};

