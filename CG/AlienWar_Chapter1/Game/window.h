#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Tree.h"
#include "Player.h"
#include "Enemy.h"
#include <learnopengl/camera.h>
#include "Structes.h"
#include <thread>
#include "StructesList.h"
using namespace std;

float module(float value);
void CreateSh(Shader* sh, short value);
void CreatBank(Enemy* Bank, short value, int texture1, int texture2);
void SetEnemyPos(Enemy* Bank, List coords);
bool CheckCollision(glm::vec3 Pos, StructesList coords, float Raio1, float Raio2);
bool CheckCollision(glm::vec3 Pos, List coords, float Raio1, float Raio2);
void PutStructesPos(Shader* sh, List coords);

struct Character {
	unsigned int TextureID;
	glm::ivec2   Size;
	glm::ivec2   Bearing;
	unsigned int Advance;
};

class window {
private:
	GLFWwindow* scene;
	short WIDTH;
	short HEIGTH;
	int Seed[3];
	double DayDuration;
public:
	window();
	GLFWwindow* getWindow();
	void init();
	void init(char* WindowTittle);
	short getWidth();
	short getHeight();
	int* getSeed();
	double getDayDuration();
	char* getWindowTittle();
	void setWidth(short width);
	void setHeight(short height);
	void setWindowTittle(char* WindowTittle);
	void run(void (*func)(window));
	void setDayDuration(double duration);
};