#pragma once
#include <functional>
#include <vector>
#include <iostream>
#include "Diff_evolution/DiffEvolution.h"
#include "Neuron.h"
#include <string>
#include <fstream>

/*
������ ���� �����!
����� ���� �� x
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


static double fixExp(double x) {//fix exponent
	if (x <= -10) return exp(-10);
	if (x >= 10) return exp(10);
	return exp(x);
};
static double fixLog(double x) {//fix log
	if (x <= 0) return 0;
	return log(x);
};
static double fixPow(double x, double y) {//fix pow
	double result = pow(x, y);
	if (result == 0 && x != 0) return 0;
	if (abs(result) > 1e50) {
		if (result > 0) return 1e50;
		else return -1e50;
	}
	return pow(x, y);
};




using namespace std;

class Tree
{
private:

	int numberFunc;//����� ������� ������� ������������ � ����, � �������� ������� ���������
	int numVertices = 0;//���������� ������ ��� ����� ����������� ������������ (�� ������������)
	int numNodes;//���������� ����� ����
	int layerLevel;//�� ����� ������ ������������ ������ ��������� ����
	int size;//���������� ������

	int numInput = NULL;//����� �����
	int ammInputs;//���������� ������
	int ammOutputs;//���������� �������

	double fitness = -9999999;//�� ��� �������

	bool lastVertice;//��������� �� �������
	bool inputBranch;//�������� �� ���� ������ �����
	bool mainNode;//��������� �� ����


	//������� � ������������
	double ef = 1;//����������� ��� RMSE
	double nf = 5;//����������� ��� ���������� �����
	int maxNodes = 50;//������������ ���������� ����� � ������


	int* ammNeuron = nullptr;//���������� ����� � ����
	int ammLayers = 0;//���������� �����

	Neuron** network = nullptr;//���� �������� ��� ��������

	Neuron* output = nullptr;//������� �� �����, ��� ������� ���������

	Tree* left = nullptr;
	Tree* right = nullptr;


	vector<string> strBinaryFunc = { "+",">" };//���������� ����� �������

	int amFuncActive = 15;//���������� ������� ���������
	function <double(double)> funcActivation[16] = {
		[](double x) { return x; } ,//0
		[](double x) {return sin(x); },//1
		[](double x) {if (x < -1) return -1.0; if (x > 1) return 1.0; else return x; },//2
		[](double x) {return 2 / (1 + fixExp(x)) - 1; },//3
		[](double x) {return fixExp(x); },//4
		[](double x) {return abs(x); },//5
		[](double x) {return 1 - fixExp(x); },//6
		[](double x) {return 0; },//7
		[](double x) {return fixPow(x,2); },//8
		[](double x) {return fixPow(x,3); },//9
		[](double x) { if (x == 0) return 0.0; return fixPow(x,-1); },//10
		[](double x) {return 1; },//11
		[](double x) {return 1 / (1 + fixExp(-x)); },//12
		[](double x) {return fixExp(-(x * x) / 2); },//13
		[](double x) {if (x < -1 / 2) return -1.0; if (x > 1 / 2) return 1.0; else return x + 1 / 2; }//14
	};


	void doHiddenNeuron();


public:
	Tree() {}
	Tree(int d, int ammInput, bool inputBranch);
	Tree(const Tree& copy)
	{

		if (copy.output != nullptr) {
			if (output != nullptr)
				delete[] output;
			//�������� ������ ��� �������� �������
			output = new Neuron[copy.ammOutputs];
			for (int i = 0; i < copy.ammOutputs; i++) {
				output[i] = copy.output[i];
			}
		}
		else {
			if (output != nullptr) {
				delete[] output;
				output = nullptr;
			}
		}

		if (copy.ammNeuron != nullptr) {
			if (ammNeuron != nullptr) {
				delete[] ammNeuron;
			}

			ammNeuron = new int[copy.ammLayers];
			for (int i = 0; i < copy.ammLayers; i++) {
				ammNeuron[i] = copy.ammNeuron[i];
			}
		}
		else {
			if (ammNeuron != nullptr) {
				delete[] ammNeuron;
				ammNeuron = nullptr;
			}
		}
		if (copy.network != nullptr) {
			if (network != nullptr) {
				for (int i = 0; i < ammLayers; i++) {
					delete[] network[i];
				}
				delete[] network;
			}
			network = new Neuron * [copy.ammLayers];
			for (int i = 0; i < copy.ammLayers; i++) {
				network[i] = new Neuron[ammNeuron[i]];//���������� � ������, ������ ��� ��� �����������
				for (int j = 0; j < ammNeuron[i]; j++) {
					network[i][j] = copy.network[i][j];
				}
			}
		}
		else {
			if (network != nullptr) {
				for (int i = 0; i < ammLayers; i++) {
					delete[] network[i];
				}
				delete[] network;
				network = nullptr;
			}
		}





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






		//��������� ������ ����� �� ���� ���� ��������������� ���������
		if (copy.left != nullptr) {
			if (left != nullptr) {
				delete left;
			}
			//left = new Tree; ������ ������� ��� ������� ��������, �� ���� ������
			//*left = Tree(*(copy.left));
			left = new Tree(*(copy.left));
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
			/*right = new Tree; ������ ������� ��� ������� ��������, �� ���� ������
			*right = Tree(*(copy.right));*/
			right = new Tree(*(copy.right));
		}
		else {
			if (right != nullptr) {
				delete right;
				right = nullptr;
			}
		}


	}


	Tree(int d, int numInputs, int numOutputs);

	void calcFitness(double** x, int size);

	string getMatrix();

	void doNeuronNetwork();

	void setCoefficientsFitness(double ef, double nf, int maxN) {
		this->ef = ef;
		this->nf = nf;
		this->maxNodes = maxN;
	}

	double getFitness() {
		return fitness;
	}

	string getFunc();


	bool getLastVertice() {
		return lastVertice;
	}

	void countNodes(int&);
	void recountLayers(int);

	void changeCoef(double*);
	int getNumVertices();
	int getNumFunc() {
		return numberFunc;
	}
	double* getValue(double* x);
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


		if (output != nullptr) {
			delete[] output;
			output = nullptr;
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

	void trainWithDE(double** x, int size);




	void randFunc() {//������������ ��� ��������� �������
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
				numberFunc = 0;// ������ �������� ����� ��� ������� �����
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

	void saveNetworkToFile(string fileName) {
		ofstream file(fileName);
		if (!file.is_open()) {
			cerr << "Error opening file: " << fileName << endl;
			return;
		}

		file << "fitness: " << fitness << endl;
		file << "Number of inputs: " << ammInputs << endl;
		file << "Number of outputs: " << ammOutputs << endl;
		file << "Number of vertices: " << numVertices << endl;
		file << "Number of nodes: " << numNodes << endl;
		file << "EF: " << ef << endl;
		file << "NF: " << nf << endl;
		file << "Max nodes: " << maxNodes << endl;
		file << "Number of layers: " << ammLayers << endl;
		file << "Number of neurons in each layer: ";
		for (int i = 0; i < ammLayers; i++) {
			file << ammNeuron[i] << " ";
		}


		file << endl << "FuncActivation:";
		for (int i = 0; i < ammLayers; i++) {

			for (int j = 0; j < ammNeuron[i]; j++) {
				file << "[ ";
				if (network[i][j].getInputBranch()) {
					file << "Input - " << network[i][j].getUseFunc();
				}
				else {
					file << "" << network[i][j].getUseFunc();
				}
				file << " ] ";
			}
			file << endl;
		}

		file << "Coordinates: " << endl;
		file << this->getMatrix() << endl;

		file << "Coefficients: " << endl;
		for (int i = 0; i < ammLayers; i++) {
			for (int j = 0; j < ammNeuron[i]; j++) {
				if (network[i][j].getUseFunc() == -1 or network[i][j].getInputBranch()) {
					continue; // ���������� �������������� ������� � ������� �������
				}
				double* coef = network[i][j].getCoefficients();

				for (int k = 0; k < network[i][j].getAmountInp() + 1; k++) {
					file << coef[k] << " ";
				}

			}

		}
		for (int i = 0; i < ammOutputs; i++) {
			if (output[i].getUseFunc() == -1 or output[i].getInputBranch()) {
				continue; // ���������� �������������� ������� � ������� �������
			}
			double* coef = output[i].getCoefficients();
			for (int k = 0; k < output[i].getAmountInp() + 1; k++) {
				file << coef[k] << " ";
			}
		}

		file << endl;



		file.close();


	}
	void loadNetworkFromFile(string fileName) {
		//�� ��������!!! ����� �������� ���� ��������� � ����
		ifstream file(fileName);
		if (!file.is_open()) {
			cerr << "Error opening file: " << fileName << endl;
			return;
		}
		string line;
		while (getline(file, line)) {
			if (line.find("fitness:") != string::npos) {
				fitness = stod(line.substr(line.find(":") + 1));
			}
			else if (line.find("Number of inputs:") != string::npos) {
				ammInputs = stoi(line.substr(line.find(":") + 1));
			}
			else if (line.find("Number of outputs:") != string::npos) {
				ammOutputs = stoi(line.substr(line.find(":") + 1));
			}
			else if (line.find("Number of vertices:") != string::npos) {
				numVertices = stoi(line.substr(line.find(":") + 1));
			}
			else if (line.find("Number of nodes:") != string::npos) {
				numNodes = stoi(line.substr(line.find(":") + 1));
			}
			else if (line.find("EF:") != string::npos) {
				ef = stod(line.substr(line.find(":") + 1));
			}
			else if (line.find("NF:") != string::npos) {
				nf = stod(line.substr(line.find(":") + 1));
			}
			else if (line.find("Max nodes:") != string::npos) {
				maxNodes = stoi(line.substr(line.find(":") + 1));
			}
			else if (line.find("Number of layers:") != string::npos) {
				ammLayers = stoi(line.substr(line.find(":") + 1));
				if (ammNeuron != nullptr) delete[] ammNeuron;
				ammNeuron = new int[ammLayers];
				if (network != nullptr) {
					for (int i = 0; i < ammLayers; i++) {
						delete[] network[i];
					}
					delete[] network;
				}
				network = new Neuron * [ammLayers];
				for (int i = 0; i < ammLayers; i++) {
					network[i] = nullptr; // �������������� ���������
				}
			}
			else if (line.find("Number of neurons in each layer:") != string::npos) {
				stringstream ss(line.substr(line.find(":") + 1));
				for (int i = 0; i < ammLayers; i++) {
					ss >> ammNeuron[i];
				}
			}
			else if (line.find("FuncActivation:") != string::npos) {
				for (int i = 0; i < ammLayers; i++) {
					getline(file, line);
					stringstream ss(line);
					for (int j = 0; j < ammNeuron[i]; j++) {
						string funcStr;
						ss >> funcStr;
						if (funcStr == "Input") {
							int useFunc;
							ss >> useFunc;
							network[i][j] = Neuron(true, useFunc); // ������� ������

						}
						else {
							int useFunc = stoi(funcStr);
							network[i][j] = Neuron(useFunc);
						}
					}
				}
			}
			else if (line.find("Coordinates:") != string::npos) {
				getline(file, line);
				
			}
			else if (line.find("Coefficients:") != string::npos) {
				int cursor = 0;
				for (int i = 0; i < ammLayers; i++) {
					for (int j = 0; j < ammNeuron[i]; j++) {
						if (network[i][j].getUseFunc() == -1 or network[i][j].getInputBranch()) {
							continue; // ���������� �������������� ������� � ������� �������
						}
						double* coef = new double[network[i][j].getAmountInp() + 1];
						for (int k = 0; k < network[i][j].getAmountInp() + 1; k++) {
							file >> coef[k];
						}
						network[i][j].setCoefficients(coef,cursor);
						delete[] coef;
					}
				}
				for (int i = 0; i < ammOutputs; i++) {
					if (output[i].getUseFunc() == -1 or output[i].getInputBranch()) {
						continue; // ���������� �������������� ������� � ������� �������
					}
					double* coef = new double[output[i].getAmountInp() + 1];
					for (int k = 0; k < output[i].getAmountInp() + 1; k++) {
						file >> coef[k];
					}
					output[i].setCoefficients(coef, cursor);
					delete[] coef;
				}
			}
		}
		file.close();
		// ������������� ���� ��������
		

	}


	Tree operator =(const Tree& copy) {
		
		if (copy.output != nullptr) {
			if (output != nullptr)
				delete[] output;
			//�������� ������ ��� �������� �������
			output = new Neuron[copy.ammOutputs];
			for (int i = 0; i < copy.ammOutputs; i++) {
				output[i] = copy.output[i];
			}
		}
		else {
			if (output != nullptr) {
				delete[] output;
				output = nullptr;
			}
		}

		if (copy.ammNeuron != nullptr) {
			if (ammNeuron != nullptr) {
				delete[] ammNeuron;
			}

			ammNeuron = new int[copy.ammLayers];
			for (int i = 0; i < copy.ammLayers; i++) {
				ammNeuron[i] = copy.ammNeuron[i];
			}
		}
		else {
			if (ammNeuron != nullptr) {
				delete[] ammNeuron;
				ammNeuron = nullptr;
			}
		}
		if (copy.network != nullptr) {
			if (network != nullptr) {
				for (int i = 0; i < ammLayers; i++) {
					delete[] network[i];
				}
				delete[] network;
			}
			network = new Neuron * [copy.ammLayers];
			for (int i = 0; i < copy.ammLayers; i++) {
				network[i] = new Neuron[ammNeuron[i]];//���������� � ������, ������ ��� ��� �����������
				for (int j = 0; j < ammNeuron[i]; j++) {
					network[i][j] = copy.network[i][j];
				}
			}
		}
		else {
			if (network != nullptr) {
				for (int i = 0; i < ammLayers; i++) {
					delete[] network[i];
				}
				delete[] network;
				network = nullptr;
			}
		}





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






		//��������� ������ ����� �� ���� ���� ��������������� ���������
		if (copy.left != nullptr) {
			if (left != nullptr) {
				delete left;
			}
			//left = new Tree; ������ ������� ��� ������� ��������, �� ���� ������
			//*left = Tree(*(copy.left));
			left = new Tree(*(copy.left));
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
			/*right = new Tree; ������ ������� ��� ������� ��������, �� ���� ������
			*right = Tree(*(copy.right));*/
			right = new Tree(*(copy.right));
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

