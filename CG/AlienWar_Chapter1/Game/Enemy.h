#include "ListBufferes.h"
#include <shader_m.h>
#include <iostream>
#pragma once
using namespace std;
class Enemy
{
private:
	glm::mat4 model;
	glm::vec3 Position;
	float velocity;
	float health;
	float scale;
	string name;
	short id;
	unsigned int texture;
public:
	Enemy();
	Enemy(float velocity);
	Enemy(float velocity, float health);
	Enemy(string name, float velocity, float health);
	unsigned int getTexture();
	float getVelocity();
	float getHealth();
	float getScale();
	short getID();
	glm::vec3 getPosition();
	string getName();
	glm::mat4 getModel();
	void setVelocity(float velocity);
	void setHealth(float health);
	void setScale(float scale);
	void setModel(glm::mat4 model);
	void setName(string name);
	void setID(short ID);
	void setTexture(int texture);
	void move(glm::vec3 moving);
};

