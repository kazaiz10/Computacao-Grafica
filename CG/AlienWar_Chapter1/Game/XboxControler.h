#pragma comment(lib,"XInput.lib")
#pragma comment(lib,"Xinput9_1_0.lib")
#include <iostream>
#include <Windows.h>
#include <Xinput.h>
#include "Player.h"
#include "Projectile.h"
#pragma once
class XboxControler
{
private:
	short Connect; // 0 -> Nenhum comando connectado 1 -> Pelo menos um comando conecetado
	int Sensitivity;
	XINPUT_STATE state;
	DWORD dwResult;
public:
	XboxControler();
	XboxControler(int Sensitivity);
	short getConnectState();
	int getSensitivity();
	void init();
	void setSensitivity(int Sensitivity);
	void start(float* varX, float* varZ, float* Zoom,float* DeltaTime, Player* player, Projectile* Pro, float deltatime, float* Firetime);
};