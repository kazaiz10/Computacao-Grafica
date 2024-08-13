#include "ListBufferes.h"
#pragma once
class Player
{
private:
	float velocity; //m/s
	glm::mat4 model;
	float scale;
	float angle;
	short FireRate;
	int KeyFront;
	int KeyBack;
	int KeyRight;
	int keyLeft;
	int keyRotateRigth;
	int KeyRotateLeft;
public:
	glm::vec3 Position;
	Player();
	void setVelocity(float velocity);
	void setModel(glm::mat4 model);
	void setPosition(glm::vec3 Position);
	void setFireRate(short FireRate);
	void setKeyFront(int KeyFront);
	void setKeyBack(int KeyBack);
	void setKeyRight(int KeyRight);
	void setKeyLeft(int KeyLeft);
	void setKeyRotateRight(int KeyRotateRight);
	void setKeyRotateLeft(int KeyRotateLeft);
	int getKeyFront();
	int getKeyBack();
	int getKeyRight();
	int getKeyLeft();
	int getKeyRotateRight();
	int getKeyRotateLeft();
	float getVelocity();
	float getAngle();
	glm::mat4 getModel();
	glm::vec3 getPosition();
	void move(glm::vec3 Moving);
	void rotate(float angle, glm::vec3 vector);
	void setModelScale(glm::vec3 vector);
	float* getCoords();
	float getScale();
	short getFireRate();
	void readKeyBoardInput(float* MoveX, float* MoveZ, float deltatim, GLFWwindow* window); //Verefica se as telcas foram precionadas e caso afirmativo altera o valor das variáveis MoveZ e MoveX para a os próximos valores de Z e X que o player vai ocupar
};

