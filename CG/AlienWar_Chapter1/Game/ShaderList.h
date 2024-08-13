#include <shader_m.h>
#include <iostream>
#include <stdio.h>
#pragma once
class ShaderList
{
private:
	
	ShaderList* Prox;
public:
	ShaderList();
	void insert(short value);
	void getindex(short value);
};

