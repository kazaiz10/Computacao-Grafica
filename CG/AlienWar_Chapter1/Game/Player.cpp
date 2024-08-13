#include "Player.h"
Player::Player() {
	this->velocity = NULL;
	this->Position = glm::vec3(0.0f);
	this->model = glm::mat4(1.0f);
	this->scale = 1.0f;
	this->angle = 0.0f;
	this->FireRate = 5;
	this->KeyFront = GLFW_KEY_W;
	this->KeyBack = GLFW_KEY_S;
	this->KeyRight = GLFW_KEY_A;
	this->keyLeft = GLFW_KEY_D;
	this->keyRotateRigth = GLFW_KEY_RIGHT;
	this->KeyRotateLeft = GLFW_KEY_LEFT;
}

void Player::setVelocity(float velocity) { this->velocity = velocity; }
void Player::setModel(glm::mat4 model) { this->model = model; }
void Player::setPosition(glm::vec3 Position) { this->Position = Position; }
void Player::setFireRate(short FireRate) { this->FireRate = FireRate; }
void Player::setKeyFront(int KeyFront) { this->KeyFront = KeyFront; }
void Player::setKeyBack(int KeyBack) { this->KeyBack = KeyBack; }
void Player::setKeyRight(int KeyRight) { this->KeyRight = KeyRight; }
void Player::setKeyLeft(int KeyLeft) { this->keyLeft = KeyLeft; }
void Player::setKeyRotateRight(int KeyRotateRight) { this->keyRotateRigth = KeyRotateRight; }
void Player::setKeyRotateLeft(int KeyRotateLeft) { this->KeyRotateLeft = KeyRotateLeft; }

int Player::getKeyFront() { return this->KeyFront; }
int Player::getKeyBack() { return this->KeyBack; }
int Player::getKeyRight() { return this->KeyRight; }
int Player::getKeyLeft() { return this->keyLeft; }
int Player::getKeyRotateRight() { return this->keyRotateRigth; }
int Player::getKeyRotateLeft() { return this->KeyRotateLeft; }
float Player::getVelocity() { return this->velocity / this->scale; }
glm::mat4 Player::getModel() { return this->model; }
glm::vec3 Player::getPosition() { return this->Position; }
float Player::getScale() { return this->scale; }
float Player::getAngle() { return this->angle; }
short Player::getFireRate() { return this->FireRate; }
void Player::setModelScale(glm::vec3 vector) { 
	this->model = glm::scale(this->model, vector);
	this->scale = vector.x;
}

void Player::move(glm::vec3 Moving) {
	this->model = glm::rotate(this->model, -this->getAngle(), glm::vec3(0.0f, 1.0f, 0.0f));
	this->model = glm::translate(this->model, Moving);
	this->model = glm::rotate(this->model, this->getAngle(), glm::vec3(0.0f, 1.0f, 0.0f));
	this->Position = this->Position + (Moving * this->scale);
}

float* Player::getCoords() {
	float coords[3] = { this->Position.x, this->Position.y, this->Position.z };
	return coords;
}

void Player::rotate(float angle, glm::vec3 vetor) {
	this->model = glm::rotate(this->model, angle, vetor);
	this->angle += angle;
}

void Player::readKeyBoardInput(float* MoveX, float* MoveZ, float deltatime, GLFWwindow* window){
	float moviment = this->getVelocity() * deltatime;
	if (glfwGetKey(window, this->KeyFront) == GLFW_PRESS)	*MoveZ = moviment;
	if (glfwGetKey(window, this->KeyBack) == GLFW_PRESS)	*MoveZ = -moviment;
	if (glfwGetKey(window, this->KeyRight) == GLFW_PRESS)	*MoveX = moviment;
	if (glfwGetKey(window, this->keyLeft) == GLFW_PRESS)	*MoveX = -moviment;
	if (glfwGetKey(window, this->keyRotateRigth) == GLFW_PRESS)	this->rotate(0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
	if (glfwGetKey(window, this->KeyRotateLeft) == GLFW_PRESS)	this->rotate(-0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
}