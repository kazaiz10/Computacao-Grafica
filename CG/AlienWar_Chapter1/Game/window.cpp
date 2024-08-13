#include "window.h"
float module(float value) {
    if (value > 0) return value;
    else return -value;
}

void CreateSh(Shader* sh, short value) {
    for (short index = 0; index < value; index++) {
        sh[index] = Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs");
    }
}

void CreatBank(Enemy* Bank, short value, int texture1, int texture2) {
    for (short index = 0; index < value; index++) {
        Bank[index] = Enemy();
        Bank[index].setID(index);
        if (index % 2 == 0) Bank[index].setScale(0.5f);
        else Bank[index].setScale(1.0f);
        if (index > 5) Bank[index].setTexture(texture1);
        else Bank[index].setTexture(texture2);
    }
}

//Coloca os inimigos nas posições certas:
//Enemy* Bank -> Array de inimigos;
//List coords -> Lista com as coordenadas onde iram ficar os inimigos
void SetEnemyPos(Enemy* Bank, List coords) {
    for (short index = 0; index < coords.getsize() / 2; index++) {
        Bank[index].move((glm::vec3(coords.getindex((2 * index) + 1), 0.0f, coords.getindex(2 * index))) * Bank[index].getScale());
    }
}

//Colocar as estruturas na prosições certas:
//Shader* sh -> array com os Shader de modelo das estruturas;
//List coords -> lista com as coordenadsa das estruturas
void PutStructesPos(Shader* sh, List coords) {
    for (short index = 1; index < coords.getsize(); index = index + 2) {
        sh[index / 2].setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(coords.getindex(index + 1), 0.0f, coords.getindex(index))));
    }
}

//Função responsável por fazer a verificar se houve alguma colisão ou não:
//glm::vec3 Pos -> vetor que representa a prosição futura do jgoador;
//StructesList coords -> lista com as estruturas
bool CheckCollision(glm::vec3 Pos, StructesList coords, float Raio1, float Raio2) {
    for (int index = 0; index < coords.getSize(); index++) {
        glm::vec3 Position = coords.getIndex(index)->getPosition();
        bool CollisionX = Pos.x + Raio1 >= Position.x && Position.x + Raio1 >= Pos.x;
        bool CollisionZ = Pos.z + Raio1 >= Position.z && Position.z + Raio1 >= Pos.z;
        if (CollisionX && CollisionZ) return true;
    }
    return false;
}

bool CheckCollision(glm::vec3 Pos, List coords, float Raio1, float Raio2) {
    for (int index = 0; index < coords.getsize(); index = index + 1) {
        bool CollisionX = Pos.x + Raio1 >= coords.getindex(index + 1) && coords.getindex(index + 1) + Raio1 >= Pos.x;
        bool CollisionZ = Pos.z + Raio1 >= coords.getindex(index) && coords.getindex(index) + Raio1 >= Pos.z;
        if (CollisionX && CollisionZ) return true;
    }
    return false;
}

float square(float value) { return value * value; }

float distance(glm::vec3 u, glm::vec3 v) {
    return sqrt(square(v.x - u.x) + square(v.y - u.y) + square(v.z - u.z));
}

void AjustWindow(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

window::window() {
    this->WIDTH = 900;
    this->HEIGTH = 900;
    this->scene = NULL;
    this->Seed[0] = CreateSeed()[0];
    this->Seed[1] = CreateSeed()[1];
    this->Seed[2] = CreateSeed()[2];
    this->DayDuration = 10;
}

GLFWwindow* window::getWindow() { return this->scene; }
int* window::getSeed() { return this->Seed; }
short window::getHeight() { return this->HEIGTH; }
short window::getWidth() { return this->WIDTH; }
double window::getDayDuration() { return this->DayDuration; }

void window::setHeight(short height) { this->HEIGTH = height; }
void window::setWidth(short width) { this->WIDTH = width; }
void window::setDayDuration(double Duration) { this->DayDuration = Duration; }

void window::init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    this->scene = glfwCreateWindow(this->WIDTH, this->HEIGTH, "window", NULL, NULL);
    glfwMakeContextCurrent(this->scene);
    glfwSetFramebufferSizeCallback(this->scene, AjustWindow);
}

void window::init(char* WindowTittle) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    this->scene = glfwCreateWindow(this->WIDTH, this->HEIGTH, WindowTittle, NULL, NULL);
    glfwMakeContextCurrent(this->scene);
}

void window::run(void (*func)(window)) {
    func(*this);
}