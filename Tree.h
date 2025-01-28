#pragma once
#include <functional>
#include <vector>
#include <iostream>
#include "Diff_evolution/DiffEvolution.h"


/*
������ ���� �����!
����� ����� �� x
���������� �������� �� y
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
	
	int numberFunc;//����� ������� ������� ������������ � ����
	int numVertices = 0;//���������� ������ ��� ����� ����������� ������������
	int numNodes;//���������� ����� ����
	int layerLevel;//�� ����� ������ ������������ ������ ��������� ����
	int size;//���������� ������

	int numInput = NULL;//����� �����
	int ammInputs;//���������� ������

	double fitness=-9999999;//�� ��� �������

	bool lastVertice;//��������� �������
	bool inputBranch;//����� �����
	bool mainNode;//��������� �� ����

	int* ammNeuron = nullptr;//���������� ����� � ����
	int ammLayers;
	Neuron** network = nullptr;

	Tree* left = nullptr;
	Tree* right = nullptr;


	vector<string> strBinaryFunc = { "+",">" };//���������� ����� �������

	vector<function <Tree* (Tree*, Tree*)>> binaryFunc = {
		[](Tree* x,Tree* y) {return x + y; },
		[](Tree* x,Tree* y) {return x > y; }
	};//������� �� �������� �������

	Tree(int d, int numInput, bool inputBranch):inputBranch(inputBranch) {
		ammInputs = numInput;
		//������ ���� ����� �� ������ �����
		if (d == 0) {
			lastVertice = true;
			if (rand() % (numInput + 1)) {
				numInput = rand() % numInput;
				numVertices = 0;
			}
			else {
				numVertices = 1;
				coef = 1;
			}
			return;
		}
		int r = rand() % 2;
	}




public:
	Tree() {}
	Tree(const Tree &copy) :numberFunc(copy.numberFunc), lastVertice(copy.lastVertice),
		unarFuncUs(copy.unarFuncUs), coef(copy.coef),numVertices(copy.numVertices),numNodes(copy.numNodes),fitness(copy.fitness),
		layerLevel(copy.layerLevel), numInput(copy.numInput), ammInputs(copy.ammInputs), numCluster(copy.numCluster)
	{

		if (copy.label != nullptr) {
			if (label != nullptr) {
				delete[] label;
				label = nullptr;
			}
			label = new int[copy.size];
			size = copy.size;
			for (int i = 0; i < size; i++) {
				label[i] = copy.label[i];
			}
		}

		/*this->operator=(copy);
		cout << 1;*/
		//��������� ������ ����� �� ���� ���� ��������������� ���������
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
	double getFitness() {
		return fitness;
	}
	Tree(int d,int numInputs);
	string getFunc();


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
	double getValue(double *x);
	int getNumNodes() {
		
		return numNodes;
	}
	int getAmmInputs() {
		return ammInputs;
	}
	~Tree() {
		if (label != nullptr) {
			delete[] label;
			label = nullptr;
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




	void randFunc() {//������������ ��� ��������� �������
		if (lastVertice) {
			int r = rand() % (ammInputs + 1);//��������� � ����
			if (r == 0) {
				numVertices = 1;
			}
			else {
				numVertices = 0;
				numInput = rand() % ammInputs;
			}
		}
		else {
			if (unarFuncUs) {
				numberFunc = rand() % unarFunc.size();
			}
			else {
				numberFunc = rand() % binaryFunc.size();
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

		if (copy.label != nullptr) {
			if (label != nullptr) {
				delete[] label;
				label = nullptr;
			}
			label = new int[copy.size];
			size = copy.size;
			for (int i = 0; i < size; i++) {
				label[i] = copy.label[i];
			}
			
		}



		numCluster = copy.numCluster;

		numberFunc = copy.numberFunc;
		layerLevel = copy.layerLevel;
		lastVertice = copy.lastVertice; 
		unarFuncUs = copy.unarFuncUs;
		coef = copy.coef;
		numInput = copy.numInput;
		numVertices = copy.numVertices;
		numNodes = copy.numNodes;
		fitness = copy.fitness;
		ammInputs = copy.ammInputs;
		//��������� ������ ����� �� ���� ���� ��������������� ���������
		if (copy.left != nullptr) {
			if (left != nullptr) {
				delete left;
			}
			left = new Tree;
			//left->operator=(*copy.left);
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
			//right->operator=(*copy.right);
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

