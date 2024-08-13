#include <stdio.h>
#pragma once
class List
{
private:
	float value;
	short ISNULL;
	List* Prox;
public:
	void insert(float value);
	List();
	List(float value);
	void setValue(float value);
	int getsize();
	int getindex(int index);
	int indexof(float value);
	void PrintList(); // Faz print a toda a lista
	short contains(float value); // Retorna 0 -> se o elemento n�o existir na lista, 1 -> se o elemento existir na lista
	List notcontains(List list); // Retorna uma lista com todos os elementos que n�o est�o na lista list
	List* getNode(int index); // Retorna no node da lista que esta na posi��o index
	void joinList(List l);
};