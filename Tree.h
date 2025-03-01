#pragma once
#include <functional>
#include <vector>
#include <iostream>
#include "Diff_evolution/DiffEvolution.h"
#include "Neuron.h"

/*
Заходя суда помни!
номер слоя по x
количество нейронов по y
+------------------------------------>x
| [0;0] [1;0] [2;0] [3;0] [4;0] [5;0]
| [0;1] [...] [] [] [] []
| [] [] [] [] [] []
|
|
|
|
vy


*/





using namespace std;

class Tree
{
private:
	
	int numberFunc;//Номер функции который используется в узле
	int numVertices = 0;//Количество вершин где нужно настраивать коэффициенты
	int numNodes;//Количество узлов ниже
	int layerLevel;//На каком уровне относительно начала находится узел
	int size;//Количество данных

	int numInput = NULL;//Номер входа
	int ammInputs;//Количество входов
	int ammOutputs;//Количество выходов

	double fitness=-9999999;//Ну тут понятно

	bool lastVertice;//Последняя ли вершина
	bool inputBranch;//Является ли узел ветвью входа
	bool mainNode;//Начальный ли узел

	int* ammNeuron = nullptr;//Количество узлов в слое
	int ammLayers = 0;//Количество слоев
	Neuron** network = nullptr;
	Neuron* output = nullptr;

	Tree* left = nullptr;
	Tree* right = nullptr;


	vector<string> strBinaryFunc = { "+",">" };//Символьный вывод функции

	int amFuncActive = 16;
	function <double(double)> funcActivation[16] = {
		[](double x) { return 0; } ,//1
		[](double x) {return sin(x); },//2
		[](double x) {if (x < -1) return -1.0; if (x > 1) return 1.0; else return x; },//3
		[](double x) {return 2 / (1 + exp(x)) - 1; },//4
		//5 пропущена
		[](double x) {return exp(x); },//6
		[](double x) {return abs(x); },//7
		[](double x) {return 1 - exp(x); },//8
		[](double x) {return x; },//9
		[](double x) {return pow(x,2); },//10
		[](double x) {return pow(x,3); },//11
		[](double x) { if (x == 0) return 0.0; return pow(x,-1); },//12
		[](double x) {return 1; },//13
		[](double x) {return 1 / (1 + exp(-x)); },//14
		[](double x) {return exp(-(x * x) / 2); },//15
		[](double x) {if (x < -1 / 2) return -1.0; if (x > 1 / 2) return 1.0; else return x + 1 / 2; }//16
	};


	


public:
	Tree() {}
	Tree(int d, int ammInput, bool inputBranch);
	Tree(const Tree &copy) 
	{
		ammOutputs = copy.ammOutputs;
		ammLayers = copy.ammLayers;
		mainNode = copy.mainNode;
		inputBranch = copy.inputBranch;
		size = copy.size;
		numberFunc = copy.numberFunc;
		layerLevel = copy.layerLevel;
		lastVertice = copy.lastVertice;
		numInput = copy.numInput;
		numVertices = copy.numVertices;
		numNodes = copy.numNodes;
		fitness = copy.fitness;
		ammInputs = copy.ammInputs;

		if (ammNeuron != nullptr) {
			delete[] ammNeuron;
		}

		if (copy.output != nullptr) {
			if (output != nullptr)
				delete output;
			output = new Neuron[ammInputs];
			for (int i = 0; i < ammInputs; i++) {
				output[i] = copy.output[i];
			}
		}
		if (copy.ammNeuron != nullptr) {
			ammNeuron = new int[ammLayers];
			for (int i = 0; i < ammLayers; i++) {
				ammNeuron[i] = copy.ammNeuron[i];
			}
		}
		if (copy.network != nullptr) {
			network = new Neuron * [ammLayers];
			for (int i = 0; i < ammLayers; i++) {
				network[i] = new Neuron[ammNeuron[i]];
				for (int j = 0; j < ammNeuron[i]; j++) {
					network[i][j] = copy.network[i][j];
				}
			}
		}
		

		//Выделение памяти чтобы не было кучи взаимосвязанных индивидлв
		if (copy.left != nullptr) {
			if (left != nullptr) {
				delete left;
			}
			left = new Tree;
			*left = Tree(*(copy.left));
		}
		else {
			if (left != nullptr) {
				delete left;
				left = nullptr;
			}
		}
		if (copy.right != nullptr) {
			if (right != nullptr) {
				delete right;
			}
			right = new Tree;
			*right = Tree(*(copy.right));
		}
		else {
			if (right != nullptr) {
				delete right;
				right = nullptr;
			}
		}
		
	}
	void calcFitness(double** x, int size,double K1);

	string getMatrix();



	double getFitness() {
		return fitness;
	}
	Tree(int d,int numInputs, int numOutputs);
	string getFunc();

	void doNeuronNetwork();

	bool getLastVertice() {
		return lastVertice;
	}

	void countNodes(int&);
	void recountLayers(int);

	void changeCoef(double *,int&);
	double getNumVertices();
	int getNumFunc() {
		return numberFunc;
	}
	double* getValue(double *x);
	int getNumNodes() {
		
		return numNodes;
	}
	int getAmmInputs() {
		return ammInputs;
	}
	~Tree() {
		if (network != nullptr) {
			if (!lastVertice) {
				for (int i = 0; i < ammLayers; i++) {
					delete[] network[i];
				}
			}
			delete[]network;
			network = nullptr;
		}
		if (ammNeuron != nullptr) {
			delete[] ammNeuron;
			ammNeuron = nullptr;
		}
		if (left != nullptr) {
			delete left;
			left = nullptr;
		}
		if (right != nullptr) {
			delete right;
			right = nullptr;
		}
	}
	void replaceNode(int, Tree&);
	void changeNode(int, Tree&);

	void trainWithDE(double** x, int size, double K1);




	void randFunc() {//Используется для оператора мутации
		if (lastVertice) {
			if (inputBranch) {
				numInput = rand() % ammInputs;
			}
			else {
				numberFunc = rand() % amFuncActive;
			}
		}
		else {
			if (inputBranch) {
				numberFunc = 0;
			}
			else {
				numberFunc = rand() % 2;
			}
		}
	}

	Tree* getLeft() {
		if (left == nullptr) {
			return nullptr;
		}
		return left;
	}
	Tree* getRight() {
		if (right == nullptr) {
			return nullptr;
		}
		return right;
	}



	Tree operator =(const Tree& copy) {
		
		ammOutputs = copy.ammOutputs;
		ammLayers = copy.ammLayers;
		mainNode = copy.mainNode;
		inputBranch = copy.inputBranch;
		size = copy.size;
		numberFunc = copy.numberFunc;
		layerLevel = copy.layerLevel;
		lastVertice = copy.lastVertice;
		numInput = copy.numInput;
		numVertices = copy.numVertices;
		numNodes = copy.numNodes;
		fitness = copy.fitness;
		ammInputs = copy.ammInputs;

		if (ammNeuron != nullptr) {
			delete[] ammNeuron;
		}

		if (copy.output != nullptr) {
			if (output != nullptr)
				delete output;
			output = new Neuron[ammInputs];
			for (int i = 0; i < ammInputs; i++) {
				output[i] = copy.output[i];
			}
		}

		if (copy.ammNeuron != nullptr) {
			ammNeuron = new int[ammLayers];
			for (int i = 0; i < ammLayers; i++) {
				ammNeuron[i] = copy.ammNeuron[i];
			}
		}
		if (copy.network != nullptr) {
			network = new Neuron * [ammLayers];
			for (int i = 0; i < ammLayers; i++) {
				network[i] = new Neuron[ammNeuron[i]];
				for (int j = 0; j < ammNeuron[i]; j++) {
					network[i][j] = copy.network[i][j];
				}
			}
		}


		//Выделение памяти чтобы не было кучи взаимосвязанных индивидлв
		if (copy.left != nullptr) {
			if (left != nullptr) {
				delete left;
			}
			left = new Tree;
			*left = Tree(*(copy.left));
		}
		else {
			if (left != nullptr) {
				delete left;
				left = nullptr;
			}
		}
		if (copy.right != nullptr) {
			if (right != nullptr) {
				delete right;
			}
			right = new Tree;
			*right = Tree(*(copy.right));
		}
		else {
			if (right != nullptr) {
				delete right;
				right = nullptr;
			}
		}
		return *this;
	}
	
};

