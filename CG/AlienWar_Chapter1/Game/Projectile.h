#include "ListBufferes.h"
#include "StructesList.h"
#include "window.h"
#pragma once
class Projectile
{
private:
	float damage;
	float velocity; //Velocidade do Projetil (m/s)
	glm::vec3 Position0; //Posição inicial
	glm::vec3 Position; //Posição atual do projetil
	float angle; //Ângulo de disparo
	float distance; //Distânica máxima até que o por
	Projectile* Prox; //Próximo projétil
	Shader* ShaderProjectile;
	glm::mat4 model;
	float VetorModule(glm::vec3 vetor);
public:
	Projectile();
	Projectile(float velocity, glm::vec3 Position0);
	Projectile(float velocity, glm::vec3 Position0, float angle, glm::vec3 VetorRotate);
	float getDamage();
	float getVelocity();
	glm::vec3 getPosition0();
	glm::vec3 getPosition();
	float getAngle();
	float getDistance();
	Shader* getShader();
	Projectile* getIndex(int index);
	glm::mat4 getModel();
	int getSize();
	void setDamage(float damage);
	void setVelocity(float velocity);
	void setPosition0(glm::vec3 Pos);
	void setAngle(float angle);
	void setDistance(float distance);
	void setModel(glm::mat4 model);
	void newProjectile(float velocity, glm::vec3 Position0, float angle, glm::vec3 VetorRotate); //Adiciona uma novo projetil
	void destroyProjectile(); //Destroi o projetil mais antigo
	void move(glm::vec3 Moviment);
	void MoveAllProject(float DeltaTime, StructesList STList, Enemy* Bank, short EnemyCount); //Faz a movimentação de todos os projeteis.
	void rotate(glm::vec3 vetor);
	void ApplyShaderModel(); //Aplica em todos os shaders de todos os projeiteis o modelos correspondentes.
};

