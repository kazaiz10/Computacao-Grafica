#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include "List.h"
#pragma once
struct Tree {
	int data = NULL;
	struct Tree* Rigth = NULL;
	struct Tree* Left = NULL;
};

struct Tree* CreateNode(int data);
struct Tree* CreateNode();
short height(Tree* tree);
int Search(Tree* t, int element);
void insertStructure(Tree* t, int value);
Tree* CreateMap(short structure);
Tree* CreateMap(short structure, int HeightMAX);
void PrintTreeBYLevel(Tree* tree);
short ColumnNeed(short TreeHeight);
void CreateTable(Tree* tree, short** matrix);
int NodesBYLevel(Tree* tree, int level);
void SaveNodeBYLevel(Tree* tree, int level, List* list);
Tree* getFather(Tree* t, int element);
void PrintTable(Tree* tree, short** matrix);
List getMatrixCoords(Tree* tree, short** matrix);
List getMatrixCoords(Tree* tree);
List getMatrixCoords(Tree* tree1, List tree2);
List getMatrixCoords(Tree* tree1, List tree2, short** matrix);
List TreeTOList(Tree* tree);
short NodeLevel(Tree* tree, short element);
short CountNodes(Tree* tree);
void PrintTreeByLevel(Tree* tree);
List CreateMiddle(short elements);
short* CreateSeed();
List getChunkCoords(Tree* top, List middle, Tree* bottom);
void AddEmeny(Tree* tree, short TreeHeight);
List MartixCoordsTOMapCoords(List coords, std::string str);
List EnemyCoordsList(Tree* top, Tree* bot);