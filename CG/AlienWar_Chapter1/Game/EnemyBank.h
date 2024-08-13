#include "Enemy.h"
#pragma once
class EnemyBank
{
private:
	Enemy* enemy;
	EnemyBank* Prox;
public:
	EnemyBank();
	void insert(short value);
	Enemy* getIndex(short index);
};

