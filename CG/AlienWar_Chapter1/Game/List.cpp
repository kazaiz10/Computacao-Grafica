#include "List.h"
const short TRUE = 1;
const short FALSE = 0;

List::List() {
	this->value = NULL;
	this->Prox = NULL;
	this->ISNULL = NULL;
}

List::List(float value) {
	this->value = value;
	this->Prox = NULL;
	this->ISNULL = 1;
}

void List::insert(float value) {
	if (this->ISNULL == NULL) {
		this->value = value;
		this->ISNULL = 1;
	}
	else if (this->Prox != NULL) this->Prox->insert(value);
	else this->Prox = new List(value);
}

void List::setValue(float value) { this->value = value; }

int List::getsize() {
	if (this != NULL) return 1 + this->Prox->getsize();
	else return 0;
}

int List::getindex(int index) {
	if (this == NULL) return NULL;
	else if (index == 0) return this->value;
	else {
		if (this->Prox != NULL) return this->Prox->getindex(index - 1);
		else return NULL;
	}
}

int List::indexof(float value) {
	if (this == NULL) return -1;
	else {
		for (short index = 0; index < this->getsize(); index++){
			if (this->getindex(index) == value) return index;
		}
		return -1;
	}
}

void List::PrintList() {
	if (this == NULL || this->ISNULL == NULL) printf("[]\n");
	else {
		int size = this->getsize();
		printf("[");
		for (short index = 0; index < size; index++) {
			if (index != size - 1) printf("%i,", this->getindex(index));
			else printf("%i]\n", this->getindex(index));
		}
	}
}

short List::contains(float value) {
	if (this == NULL || this->ISNULL == NULL) return FALSE;
	else {
		if (this->value == value) return TRUE;
		else this->Prox->contains(value);
	}
}

List List::notcontains(List L) {
	if (this == NULL || this->ISNULL == NULL) return NULL;
	else {
		List list;
		for (short index = 0; index < this->getsize(); index++) {
			int element = this->getindex(index);
			if (L.contains(element) != TRUE) list.insert(element);
		}
		return list;
	}
}

List* List::getNode(int index) {
	if (this == NULL || this->ISNULL == NULL) return NULL;
	else if (index == 0) return this;
	else {
		if (this->Prox != NULL) return this->Prox->getNode(index - 1);
		else return NULL;
	}
}

void List::joinList(List L) {
	for (int index = 0; index < L.getsize(); index++) {
		this->insert(L.getindex(index));
	}
}