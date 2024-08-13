#include "Tree.h"

//Criação de um Node da árvore:
struct Tree* CreateNode(int data) {
	Tree* pinheiro = new struct Tree;
	pinheiro->data = data;
	pinheiro->Rigth = NULL;
	pinheiro->Left = NULL;
	return pinheiro;
}

//Criação de um Node vazio da árvore:
struct Tree* CreateNode() {
	Tree* pinheiro = new struct Tree;
	pinheiro->data = NULL;
	pinheiro->Rigth = NULL;
	pinheiro->Left = NULL;
	return pinheiro;
}

//Inserir um número na árvore:
void insertStructure(Tree* t, int value) {
	if (!t) t = CreateNode(value);
	if (value > t->data) {
		if (t->Rigth == NULL) t->Rigth = CreateNode(value);
		else insertStructure(t->Rigth, value);
	}
	if (value < t->data) {
		if (t->Left == NULL) t->Left = CreateNode(value);
		else insertStructure(t->Left, value);
	}
}

//Retorna o Node Pai do elemento
Tree* getFather(Tree* t, int element) {
	if (t == NULL) return NULL;
	else if (t->Rigth != NULL && t->Rigth->data == element) return t;
	else if (t->Left != NULL && t->Left->data == element) return t;
	else {
		if (element > t->data) {
			if (t->Rigth != NULL) return getFather(t->Rigth, element);
			else return NULL;
		}
		else if (element < t->data) {
			if (t->Left != NULL) return getFather(t->Left, element);
			else return NULL;
		}
		else return NULL;
	}
}

//Função responsável por procurar um elemento na árvore.
//Retorna 1 se encontrar retorna 0 se não encontrar
int Search(Tree* t, int element) {
	if (t == NULL) return 0;
	else if (t->data == element) return 1;
	else {
		if (element > t->data) return Search(t->Rigth, element);
		else return Search(t->Left, element);
	}
}

//Criar uma árvore com structure nodes aleatórios:
Tree* CreateMap(short structure) {
	srand((unsigned)structure * time(0));
	int value = rand() % 500;
	Tree* acacia = CreateNode(value);
	for (int i = 1; i < structure; i++) {
		while (Search(acacia, value) == 1) {
			value = rand() % 500;
		}
		insertStructure(acacia, value);
	}
	return acacia;
}

//Criar uma árvore com structure nodes aleatórios:
Tree* CreateMap(short structure, int HeightMAX) {
	srand((unsigned)structure * time(0));
	int value = rand() % 500;
	Tree* acacia = CreateNode(value);
	for (int i = 1; i < structure; i++) {
		while (Search(acacia, value) == 1 || NodeLevel(acacia, value) > HeightMAX) {
			value = rand() % 500;
		}
		insertStructure(acacia, value);
	}
	return acacia;
}

//Retorna o maior valor entre dois valores do tipo short
int max(short value1, short value2) {
	if (value1 > value2) return value1;
	else return value2;
}

//Retorna a altura da árvore binária:
short height(Tree* tree) {
	if (tree == NULL) return -1;
	else return 1 + max(height(tree->Rigth), height(tree->Left));
}

//Imprime um nivel especifico da árvore:
void PrintLevel(Tree* tree, int level) {
	if (level == 0) printf("%i ", tree->data);
	else {
		if (tree->Rigth != NULL) PrintLevel(tree->Rigth, level - 1);
		if (tree->Left != NULL) PrintLevel(tree->Left, level - 1);
	}
}

//Retorna o número de Nodes por nivel:
int NodesBYLevel(Tree* tree, int level) {
	if (level == 0) return 1;
	else {
		if (tree->Rigth != NULL && tree->Left != NULL) return NodesBYLevel(tree->Rigth, level - 1) + NodesBYLevel(tree->Left, level - 1);
		else if (tree->Rigth != NULL) return NodesBYLevel(tree->Rigth, level - 1);
		else if (tree->Left != NULL) return NodesBYLevel(tree->Left, level - 1);
		else return 0;
	}
}

//Salvar todos os elementos do nivel "level" na lista "list":
void SaveNodeBYLevel(Tree* tree, int level, List* list) {
	if (level == 0) list->insert(tree->data);
	else {
		if (tree->Rigth != NULL && tree->Left != NULL) {
			SaveNodeBYLevel(tree->Rigth, level - 1, list);
			SaveNodeBYLevel(tree->Left, level - 1, list);
		}
		else if (tree->Rigth != NULL) SaveNodeBYLevel(tree->Rigth, level - 1, list);
		else if (tree->Left != NULL) SaveNodeBYLevel(tree->Left, level - 1, list);
	}
}

//Salvar todos os elementos de uma árvore numa lista:
List TreeTOList(Tree* tree) {
	List list;
	for (int altura = 0; altura < height(tree) + 1; altura++) {
		SaveNodeBYLevel(tree, altura, &list);
	}
	return list;
}


//Imprime a árvore por nivel:
void PrintTreeBYLevel(Tree* tree) {
	for (short level = 0; level <= height(tree); level++) {
		printf("%i -> ", level);
		PrintLevel(tree, level);
		printf("\n");
	}
}

//Retorna a base (base) elevada ao expoenete (expo). Só funciona para expo > 0:
int expoente(short base, short expo) {
	if (expo == 0) return 1;
	else return base * expoente(base, expo - 1);
}

//Retorna o número de Colunas necessárias para criação da Matriz/Tabela:
short ColumnNeed(short TreeHeight) {
	if (TreeHeight >= 1) return expoente(2, TreeHeight) + ColumnNeed(TreeHeight - 1);
	else return 1;
}

//Retorna as cordernadas x e y de um elemento na matrix.
//O output é um array de duas coordenadas [x,y].
//Se o elemento não existir na matrix devolve as coordenadas [-1, -1].
short* Coords(short altura, short comprimento, short delta, short** matrix, short element) {
	short coords[2] = { -1, -1 };
	for (int column = delta - 1; column < comprimento; column++) {
		if (matrix[altura][column] == element) {
			coords[0] = column;
			coords[1] = altura;
			return coords;
		}
	}
	return coords;
}

//Returna a variação que existe na altura "altura":
short CalcDeltaX(short altura) {
	short MaxNode = ColumnNeed(altura);
	return MaxNode - ((MaxNode - 1) / 2);
}

//Responsável por converte a árvore para uma matriz.
//A árvore fica guardadar sobre a forma de matriz na variável "matrix" passada como argumento:
void CreateTable(Tree* tree, short** matrix) {
	short altura = height(tree);
	short comprimento = ColumnNeed(altura);
	matrix[0][(comprimento - 1) / 2] = tree->data;
	for (int height = 1; height <= altura; height++) {
		List ListNode;
		SaveNodeBYLevel(tree, height, &ListNode);
		short DeltaX = CalcDeltaX(altura - height);
		for (int element = 0; element < ListNode.getsize(); element++) {
			short ListElement = ListNode.getindex(element);
			int x = Coords(height - 1, comprimento, DeltaX, matrix, getFather(tree, ListElement)->data)[0];
			if (ListNode.getindex(element) > getFather(tree, ListElement)->data)
				matrix[height][x + DeltaX] = ListElement;
			else matrix[height][x - DeltaX] = ListElement;
		}
	}
}

//Responsável por imprimir a tabela/matriz
void PrintTable(Tree* tree, short** matrix) {
	for (int row = 0; row <= height(tree); row++) {
		for (int column = 0; column < ColumnNeed(height(tree)); column++) {
			if (matrix[row][column] == NULL || matrix[row][column] < 0) printf("  ");
			else printf("%i", matrix[row][column]);
		}
		printf("\n");
	}
}

//Retorna uma Lista com todas as coordenadas ocupadas da matrix:
List getMatrixCoords(Tree* tree, short** matrix) {
	short altura = height(tree);
	short largura = ColumnNeed(altura);
	List list;
	for (short row = 0; row <= altura; row++) {
		for (short column = 0; column < largura; column++) {
			if (matrix[row][column] != 0) {
				list.insert(row);
				list.insert(column);
			}
		}
	}
	return list;
}

//Retorna uma Lista com todas as coordenadas ocupadas da matrix:
List getMatrixCoords(Tree* tree) {
	short** table = (short**)calloc(height(tree) + 1, sizeof(short*));
	for (int altura = 0; altura <= height(tree); altura++) {
		short* column = (short*)calloc(ColumnNeed(height(tree)), sizeof(short));
		table[altura] = column;
	}
	CreateTable(tree, table);
	return getMatrixCoords(tree, table);
}

//Retorna uma Lista com todas as coordenadas ocupadas da matrix que não tenham valores contidos na lista tree2:
List getMatrixCoords(Tree* tree1, List tree2, short** matrix) {
	short altura = height(tree1);
	short largura = ColumnNeed(altura);
	List list;
	for (short row = 0; row <= altura; row++) {
		for (short column = 0; column < largura; column++) {
			if (matrix[row][column] != 0 && tree2.contains(matrix[row][column]) == 0) {
				list.insert(row);
				list.insert(column);
			}
		}
	}
	return list;
}

//Retorna uma Lista com todas as coordenadas ocupadas da matrix que não tenham valores contidos na lista tree2 :
List getMatrixCoords(Tree* tree1, List tree2) {
	short** table = (short**)calloc(height(tree1) + 1, sizeof(short*));
	for (int altura = 0; altura <= height(tree1); altura++) {
		short* column = (short*)calloc(ColumnNeed(height(tree1)), sizeof(short));
		table[altura] = column;
	}
	CreateTable(tree1, table);
	return getMatrixCoords(tree1, tree2, table);
}

//Retorna em que altura o elemento "element" será adiciona na árvore
//Se o elemento "element" já estiver na tree retorna uma valor negativo.
short NodeLevel(Tree* tree, short element) {
	if (!tree || tree->data == NULL) return -1;
	else {
		if (element > tree->data) {
			if (tree->Rigth == NULL) return 1;
			else return 1 + NodeLevel(tree->Rigth, element);
		}
		else if (element < tree->data) {
			if (tree->Left == NULL) return 1;
			else return 1 + NodeLevel(tree->Left, element);
		}
		else return -9990;
	}
}

//Retorna o valor total de nodes presentes na árvore:
short CountNodes(Tree* tree) {
	if (!tree) return 0;
	else return 1 + CountNodes(tree->Rigth) + CountNodes(tree->Left);
}

//Imprime a árvore por altura/nivel:
void PrintTreeByLevel(Tree* tree) {
	for (int h = 0; h < height(tree); h++) {
		List list;
		SaveNodeBYLevel(tree, h, &list);
		for (int index = 0; index < list.getsize(); index++) {
			printf("%i ", list.getindex(index));
		}
		printf("\n");
	}
}

short contains(short* array, short element, short size) {
	for (int i = 0; i < size; i++) {
		if (array[i] == element) return 1;
	}
	return 0;
}

//Delvolve uma lista com as coordenadas das estruturas que iram ficar no meio:
List CreateMiddle(short elements) {
	List lista = List();
	srand(time(0));
	//short* x = (short*) calloc(elements, sizeof(short));
	for (int i = 0; i < elements; i++) {
		srand(time(0) + i);
		short value = rand() % 7;
		if (value % 2 == 0) value = -value;
		//x[i] = value;
		lista.insert(cos(value) * 3);
		lista.insert(value);
		//printf("X -> %i Z -> %f\n", value, cos(value) * 3);
	}
	return lista;
}

//Retorne de seed value:
short* CreateSeed() {
	srand(time(0));
	short seed[3];
	seed[0] = rand() % 12 + 1;
	srand(seed[0]);
	seed[1] = rand() % 4 + 1;
	srand(seed[1] * time(0));
	seed[2] = rand() % 12 + 1;
	return seed;
}

//Retorna uma Lista com as coordenadas de todas as estruturas dentro de um Chunck
List getChunkCoords(Tree* t, List middle, Tree* bot) {
	List top = getMatrixCoords(t);
	List bottom = getMatrixCoords(bot);
	List coods;
	for (int index = 0; index < top.getsize(); index = index + 2) {
		coods.insert(top.getindex(index) + 7.0f);
		coods.insert(top.getindex(index + 1) - 7.0f);
	}
	for (int index = 0; index < middle.getsize(); index++) {
		coods.insert(middle.getindex(index));
	}
	for (int index = 0; index < bottom.getsize(); index = index + 2) {
		coods.insert(bottom.getindex(index) - 7.0f);
		coods.insert(bottom.getindex(index + 1) - 7.0f);
	}
	return coods;
}

//Adiciona Enemigos a Árvore:
void AddEmeny(Tree* Structs, short TreeHeight) {
	while (CountNodes(Structs) < ColumnNeed(TreeHeight) - 2){
		srand((unsigned)4 * time(0));
		int value = rand() % 500;
		while (Search(Structs, value) == 1 || NodeLevel(Structs, value) > 3) {
			value = rand() % 500;
		}
		insertStructure(Structs, value);
	}
}


List MartixCoordsTOMapCoords(List coords, std::string str) {
	List list;
	if (str == "top") {
		for (short index = 0; index < coords.getsize(); index = index + 2) {
			list.insert(coords.getindex(index) + 7.0f);
			list.insert(coords.getindex(index + 1) - 7.0f);
		}
	}
	else if (str == "bot") {
		for (short index = 0; index < coords.getsize(); index = index + 2) {
			list.insert(coords.getindex(index) - 7.0f);
			list.insert(coords.getindex(index + 1) - 7.0f);
		}
	}
	return list;
}

List EnemyCoordsList(Tree* top, Tree* bot) {
	List valuesTOP = TreeTOList(top);
	List valuesBOT = TreeTOList(bot);
	AddEmeny(top, 3);
	AddEmeny(bot, 3);
	List EnemyTop = MartixCoordsTOMapCoords(getMatrixCoords(top, valuesTOP), "top");
	List EnemyBot = MartixCoordsTOMapCoords(getMatrixCoords(bot, valuesBOT), "bot");
	EnemyTop.joinList(EnemyBot);
	return EnemyTop;
}