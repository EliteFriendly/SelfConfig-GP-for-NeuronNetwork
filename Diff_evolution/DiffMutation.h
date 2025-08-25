#pragma once
#include "IndividualDiffEvolution.h"
#include<vector>
#include <iostream>

using namespace std;

class DiffMutation
{
private:
	string type;//Тип мутации
	int* selectedInd = nullptr;//Номера выбранных индивидов
	double F;//Масштабирующий фактор
	int ammount;//Количество родителей в донор векторе
	double* limitsDimension = nullptr;//Ограничение на каждую ось
	int ammDimens;//Количество пространств

	void outLimits(IndividualDiffEvolution);//Проверка вышел ли индивид за границы
public:
	void setF(double F) {
		DiffMutation::F = F;
	}

	void setType(string type) {
		DiffMutation::type = type;
		if (type == "rand1") {
			ammount = 3;
		}
		if (type == "rand2") {
			ammount = 5;
		}
		if (type == "best1") {
			ammount = 2;
		}
		if (type == "best2") {
			ammount = 4;
		}
		if (type == "targetToRand1") {
			ammount = 4;//Включая i - тый вектор
		}
		if (type == "targetToBest1") {
			ammount = 3;//Включая i - тый вектор
		}
	}
	void setLimits(double* limits, int ammDimens) {
        DiffMutation::ammDimens = ammDimens;
        limitsDimension = new double[ammDimens * 2];
		for (int i = 0; i < ammDimens * 2; i++) {
			limitsDimension[i] = limits[i];
		}
	}

	IndividualDiffEvolution getDonor(IndividualDiffEvolution *, IndividualDiffEvolution&, int);

	~DiffMutation()
	{
		if(limitsDimension != nullptr){
			delete[] limitsDimension;
		}
		if (selectedInd != nullptr) {
			delete[] selectedInd;
		}

	}
};

