#pragma once
#include <iostream>
#include <sstream>

using namespace std;
class Neuron
{
	//����� ��������� ����� � ������ ������������ �������
	int** input = nullptr;//���������� �������� �����
	bool output = false;//���� �� �������� ����
	int amountInp = 0;//������ ���������� ������
	
	int useFunc;//����� ������������ �������, ��� �����(�� 0)
	bool inputBranch = false;//��������� �� � ����� �����


public:
	Neuron() {};
	Neuron(const Neuron& copy) {
		output = copy.output;
		amountInp = copy.amountInp;
		useFunc = copy.useFunc;
		inputBranch = copy.inputBranch;
		if (copy.input != nullptr) {
			input = new int* [amountInp];
			for (int i = 0; i < amountInp; i++) {
				input[i] = new int[2];
				input[i][0] = copy.input[i][0];
				input[i][1] = copy.input[i][1];
			}
		}

	}
	Neuron(bool inputBranch, int numberInput):inputBranch(inputBranch),useFunc(numberInput) {
		input = new int*[0];
		output = false;
	}
	Neuron(int useFunc):useFunc(useFunc){};
	
	void connect(int amount,int* xOutputs, int* yOutputs, int x, int y)//������� � � � ��, ������������ ������������ ����
	{
		if (input == nullptr) {//���������� ������ ����� ������ ����� �� �������� �������� �����
			input = new int* [amount];
			Neuron::amountInp = amount;
			for (int i = 0; i < amount; i++) {
				input[i] = new int[2];
				input[i][0] = xOutputs[i] - x;
				input[i][1] = yOutputs[i] - y;
			}
		}
		else {
			cout << endl << "������ ��������";
			exit(0);
		}
	}
	string getStrCoord() {
		stringstream ss;
		if (input == nullptr) {
			ss << " nullptr ";
			return ss.str();
		}
		for (int i = 0; i < amountInp; i++) {
			ss << "{ " << input[i][0] << " ; " << input[i][1] << " }";
		}
		return ss.str();
	}
	int** getCoord() {
		return input;
	}
	bool getOutput() {
		return output;
	}
	bool getInput() {
		if (input == nullptr) {
			return false;
		}
		return true;
	}
	void haveOutput() { output = true; }
	~Neuron() {
		if (input != nullptr) {
			if (inputBranch) {
			}
			else {
				if (amountInp != 0) {
					for (int i = 0; i < amountInp; i++) {
						delete[] input[i];
					}
				}
				
			}
			delete[] input;
			
		}
		input = nullptr;
	}
	Neuron operator = (const Neuron& copy) {
		output = copy.output;
		amountInp = copy.amountInp;
		useFunc = copy.useFunc;
		inputBranch = copy.inputBranch;
		if (copy.input != nullptr) {
			input = new int* [amountInp];
			for (int i = 0; i < amountInp; i++) {
				input[i] = new int[2];
				input[i][0] = copy.input[i][0];
				input[i][1] = copy.input[i][1];
			}
		}
		return *this;
	}
};

