#pragma once
#include <iostream>
#include<vector>
#include<functional>
#include "MutationGP.h"
#include"CrossoverGP.h"
#include"Tree.h"
#include "SelectionGP.h"
#include "FormingGP.h"
#include <thread>
#include <fstream>

class AdaptiveGeneticProgramming
{
private:
	int treeDepth;
	Tree bestIndividual;
	int numIndividuals;
	int numGeneration;
	Tree* arrayIndividuals = nullptr;
	Tree* arrayChildren = nullptr;
	double socialCard = 0.1;//Минимальная вероятность выбора

	int ammInputs;//Количество осей или входов
	int size;//Количество точек
	int numberFile = 1;//Используется для именовании файла

	int ammSel = 5;
	SelectionGP** selection = nullptr;
	/*
	0-ранговая
	1-пропорциональная
	2-4 турнирная(3,5,10)
	*/
	double* selProbabilities = nullptr;
	int* chosenSel = nullptr;
	ofstream fSel;

	int ammCross = 4;
	CrossoverGP** crossover = nullptr;
	/*
	0-пустое
	1-стандарт
	2-одноточечное
	3-равномерное
	*/
	double* crossProbabilities = nullptr;
	int* chosenCross = nullptr;
	ofstream fCross;

	int ammMut = 4;
	MutationGP** mutation = nullptr;
	/*
	0-дерево
	1-слабая
	2-средняя
	3-сильная
	*/
	double* mutProbabilities = nullptr;
	int* chosenMut = nullptr;
	ofstream fMut;

	FormingGP forming;

	void findBest();

	int probabilityChoice(double* arrProb, int size) {
		mt19937 gen(rand());

		double sum = arrProb[0];
		int number = 0;
		double probability = gen() % 1000000 / 1000000.0;
		while (probability > sum) {
			number++;
			if (number == size) {
				return number - 1;
			}
			sum += arrProb[number];
		}
		return number;
	}

	void setSelectionsArrays();

	Tree createChild(int);

	void threadsFitnessCalc(double** x,int ammThread);

	int findWinner(int* arr, int ammPlayer,double* arrFitness);

	void changeProbabilities(double* arrProba, int numWinner, int ammPlayers);
	
	void recalcProbabilities();

	void saveProbabilities() {
		for (int i = 0; i < ammSel; i++) {
			fSel << selProbabilities[i]<<'\t';
		}
		fSel << endl;
		for (int i = 0; i < ammMut; i++) {
			fCross << crossProbabilities[i] << '\t';
			fMut << mutProbabilities[i] << '\t';
		}
		fCross << endl;
		fMut << endl;
	}

public:
	AdaptiveGeneticProgramming(int treeDepth) : treeDepth(treeDepth){
		
		selection = new SelectionGP*[5];
		selection[0] = new RankedSelection;
		selection[1] = new ProportionalSelection;
		selection[2] = new TournamentSelection(3);
		selection[3] = new TournamentSelection(5);
		selection[4] = new TournamentSelection(10);

		crossover = new CrossoverGP * [4];
		crossover[0] = new EmptyCrossover;
		crossover[1] = new StandartCrossover;
		crossover[2] = new OnepointCrossover;
		crossover[3] = new UniformCrossover;

		mutation = new MutationGP * [4];
		mutation[0] = new TreeMutation(treeDepth - 1);
		mutation[1] = new PointMutation("Weak");
		mutation[2] = new PointMutation("Average");
		mutation[3] = new PointMutation("Strong");


		selProbabilities = new double[5];
		crossProbabilities = new double[4];
		mutProbabilities = new double[4];

		for (int i = 0; i < 5; i++) {
			selProbabilities[i] = 1.0 / 5;
		}
		for (int i = 0; i < 4; i++) {
			crossProbabilities[i] = 0.3;
			mutProbabilities[i] = 1.0 / 4;
		}
		crossProbabilities[0] = 0.1;//Ибо там пустой кроссовер
	}
	void startTrain(double** x, int ammInputs, int size, int numIndividuals, int numGeneration);
	Tree getBest() {
		return bestIndividual;
	}
	double getError(double** x, double *y, int size) {
		double sum = 0;//Среднеквадратичная ошибка
		for (int i = 0; i < size; i++) {
			sum += pow(bestIndividual.getValue(x[i])[0] - y[i], 2);
		}
		sum = pow(sum , 0.5) / double(size);
		return sum;

	}

	void numFile(int num) {
		numberFile = num;
	}

	~AdaptiveGeneticProgramming() {
		if (arrayIndividuals != nullptr) {
			delete[] arrayIndividuals;
		}
		if (arrayChildren != nullptr) {
			delete[] arrayChildren;
		}

	}
};

