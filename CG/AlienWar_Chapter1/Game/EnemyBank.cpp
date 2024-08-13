#include "EnemyBank.h"

EnemyBank::EnemyBank() {
	this->enemy = new Enemy();
	this->Prox = NULL;
}

void EnemyBank::insert(short value) {
	if (this->Prox == NULL) {
		if (value != 0) {
			this->Prox = new EnemyBank();
			this->Prox->insert(value - 1);
		}
	}
}

Enemy* EnemyBank::getIndex(short index) {
	if (index == 0) return this->enemy;
	else {
		if (this->Prox != NULL) return this->Prox->getIndex(index - 1);
		else return this->enemy;
	}
}