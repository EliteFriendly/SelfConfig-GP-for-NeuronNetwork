#pragma once
#include "IndividualDiffEvolution.h"
#include <vector>
#include<iostream>
#include <algorithm>
#include<thread>
#include "DiffMutation.h"
#include<ctime>
#include <functional>

using namespace std;

class DiffEvolution
{
private:
	function<double(double*)> func;
	double* limitsDimension;//����������� ������ ���
	double F;//�������������� ������
	double Cr;//����������� �����������
	int N;//���������� ���������
	int generations;//���������� ���������
	IndividualDiffEvolution* arrIndividuals;//������ ���������� ������� �������
	IndividualDiffEvolution best;//������ ��������� �������
	DiffMutation mutation;
	string aim;//���� ������
	int ammDimens;//���������� ���������(����)

	IndividualDiffEvolution crossover(IndividualDiffEvolution);
	void surviveCrossover(IndividualDiffEvolution);
	void saveBest();
public:
	DiffEvolution(function<double(double*)> func, double* limitsDimension, int ammDimens, string typeMut, string aim) :
		ammDimens(ammDimens),
		func(func),
		aim(aim)
	{
		DiffEvolution::limitsDimension = new double[ammDimens * 2];

		for (int i = 0; i < ammDimens * 2; i++) {
			DiffEvolution::limitsDimension[i] = limitsDimension[i];
		}

		mutation.setType(typeMut);
		mutation.setLimits(limitsDimension, ammDimens);
	}
	void startSearch(double acc,double F, double Cr, int N,int generation);


	double* getBestCoordinates() {
		return best.getCoordinats();
	}

	double getBest() {
		return func(best.getCoordinats());
	}


};

