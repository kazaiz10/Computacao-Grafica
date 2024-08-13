#include "Projectile.h"

Projectile::Projectile() {
	this->damage = 1.0f;
	this->velocity = 0.0f;
	this->Position0 = glm::vec3(0.0f);
	this->angle = 0.0f;
	this->distance = 10.0f;
	this->Prox = NULL;
	this->ShaderProjectile = NULL;
	this->model = glm::mat4(1.0f);
	this->Position = glm::vec3(0.0f);
}

Projectile::Projectile(float velocity, glm::vec3 Pos) {
	this->damage = 1.0f;
	this->velocity = velocity;
	this->Position0 = Pos;
	this->angle = 0.0f;
	this->distance = 10.0f;
	this->Prox = NULL;
	this->ShaderProjectile = new Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs");
	this->model = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.3f)), Pos);
	this->Position = Pos;
}

Projectile::Projectile(float velocity, glm::vec3 Pos, float angle, glm::vec3 VetorRotate) {
	this->damage = 1.0f;
	this->velocity = velocity;
	this->Position0 = Pos;
	this->angle = angle;
	this->distance = 10.0f;
	this->Prox = NULL;
	this->ShaderProjectile = new Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs");
	this->model = glm::translate(glm::mat4(1.0f), Pos);
	this->model = glm::rotate(glm::scale(this->model, glm::vec3(0.3f)), angle, VetorRotate);
	this->Position = Pos;
}

float Projectile::getDamage() { return this->damage; }
float Projectile::getAngle() { return this->angle; }
float Projectile::getDistance() { return this->distance; }
float Projectile::getVelocity() { return this->velocity; }
glm::vec3 Projectile::getPosition0() { return this->Position0; }
glm::vec3 Projectile::getPosition() { return this->Position; }
glm::mat4 Projectile::getModel() { return this->model; }
Shader* Projectile::getShader() { return this->ShaderProjectile; }

void Projectile::setDamage(float Damege) { this->damage = damage; }
void Projectile::setAngle(float Angle) { this->angle = angle; }
void Projectile::setVelocity(float Velocity) { this->velocity = velocity; }
void Projectile::setDistance(float Distance) { this->distance = distance; }
void Projectile::setPosition0(glm::vec3 Pos) { 
	this->Position0 = Pos;
	this->model = glm::translate(this->model, Pos);
}
void Projectile::setModel(glm::mat4 model) { this->model = model; }

Projectile* Projectile::getIndex(int index) {
	if (!this) return NULL;
	else if (index == 0) return this;
	else {
		if (this->Prox != NULL) return this->Prox->getIndex(index - 1);
		else return NULL;
	}
}

void Projectile::newProjectile(float velocity, glm::vec3 Position0, float angle, glm::vec3 VetorRotate) {
	if (this->Prox == NULL) this->Prox = new Projectile(velocity, Position0, angle, VetorRotate);
	else this->Prox->newProjectile(velocity, Position0, angle, VetorRotate);
}

void Projectile::destroyProjectile() {
	if (this->Prox != NULL) *this = *this->Prox;
}

int Projectile::getSize() {
	if (!this) return 0;
	else {
		if (this->Prox != NULL) return 1 + this->Prox->getSize();
		else return 1;
	}
}

void Projectile::rotate(glm::vec3 vetor) { this->model = glm::rotate(this->model, this->angle, vetor); }
void Projectile::move(glm::vec3 Moviment) { 
	this->model = glm::translate(this->model, Moviment);
	float vetormodule = this->VetorModule(Moviment * 0.3f);
	this->Position = this->Position + glm::vec3(sin(this->angle) * vetormodule, 0.0f, cos(this->angle) * vetormodule);
}
void Projectile::MoveAllProject(float DeltaTime, StructesList STList, Enemy* Bank, short EnemyCount) {
	if (!this) return;
	else {
		for (short index = 0; index < EnemyCount; index++) {
			if (Bank[index].getPosition().x + 0.8f >= this->Position.x && this->Position.x + 0.8f >= Bank[index].getPosition().x) {
				if (Bank[index].getPosition().z + 0.8f >= this->Position.z && this->Position.z + 0.8f >= Bank[index].getPosition().z) {
					Bank[index].setHealth(0);
				}
			}
		}
		if (CheckCollision(this->Position, STList, 0.7f, 0.0f) && this->Prox != NULL) *this = *this->Prox;
		else {
			this->move(glm::vec3(0.0f, 0.0f, this->velocity) * DeltaTime);
			if (this->Prox != NULL) this->Prox->MoveAllProject(DeltaTime, STList, Bank, EnemyCount);
		}
	}
}
void Projectile::ApplyShaderModel() {
	if (!this) return;
	else {
		if (this->ShaderProjectile != NULL) this->ShaderProjectile->setMat4("model", this->model);
		if (this->Prox != NULL) this->Prox->ApplyShaderModel();
	}
}

float Projectile::VetorModule(glm::vec3 vetor) {
	return sqrt(vetor.x * vetor.x + vetor.y * vetor.y + vetor.z * vetor.z);
}