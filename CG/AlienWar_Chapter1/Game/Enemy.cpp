#include "Enemy.h"
Enemy::Enemy() {
	this->model = glm::mat4(1.0f);
	this->Position = glm::vec3(0.0f);
	this->velocity = 0.0f;
	this->health = 1.0f;
	this->scale = 1.0f;
	this->name = "Enemy";
	this->id = 0;
}

Enemy::Enemy(float velocity) {
	this->model = glm::mat4(1.0f);
	this->Position = glm::vec3(0.0f);
	this->velocity = velocity;
	this->health = 1.0f;
	this->scale = 1.0f;
	this->name = "Enemy";
	this->id = 0;
}

Enemy::Enemy(float velocity, float health) {
	this->model = glm::mat4(1.0f);
	this->Position = glm::vec3(0.0f);
	this->velocity = velocity;
	this->health = health;
	this->scale = 1.0f;
	this->name = "Enemy";
	this->id = 0;
}

Enemy::Enemy(string name, float velocity, float health) {
	this->model = glm::mat4(1.0f);
	this->Position = glm::vec3(0.0f);
	this->velocity = velocity;
	this->health = health;
	this->scale = 1.0f;
	this->name = name;
	this->id = 0;
}

glm::vec3 Enemy::getPosition() { return this->Position; }
glm::mat4 Enemy::getModel() { return this->model; }
string Enemy::getName() { return this->name; }
float Enemy::getHealth() { return this->health; }
float Enemy::getVelocity() { return this->velocity / this->scale; }
float Enemy::getScale() { return this->scale; }
short Enemy::getID() { return this->id; }
unsigned int Enemy::getTexture() { return this->texture; }

void Enemy::setModel(glm::mat4 model) { this->model = model; }
void Enemy::setName(string name) { this->name = name; }
void Enemy::setHealth(float health) { this->health = health; }
void Enemy::setVelocity(float velocity) { this->velocity = velocity; }
void Enemy::setTexture(int texture) { this->texture = texture; }
void Enemy::setScale(float scale) { 
	this->scale = scale;
	this->model = glm::scale(this->model, glm::vec3(scale));
}
void Enemy::setID(short id) { this->id = id; }
void Enemy::move(glm::vec3 moving) {
	this->model = glm::translate(this->model, moving);
	this->Position = this->Position + (moving + this->scale);
}