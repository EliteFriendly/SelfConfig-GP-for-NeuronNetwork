#pragma once
#include <iostream>
#include <sstream>

using namespace std;
class Neuron
{
	//Здесь находятся связи с узлами относительно нейрона
	int** input = nullptr;//Координаты входящих узлов
	bool output = false;//Есть ли выходные узлы
	int amountInp = 0;//Только количество входов
	
	int useFunc;//Номер используемой функции, или входа(от 0)
	bool inputBranch = false;//Находится ли в ветви входа

	double* coefficients;//Коэффициенты вместе со свободным

public:
	Neuron() {};
	Neuron(const Neuron& copy) {
		output = copy.output;
		amountInp = copy.amountInp;
		useFunc = copy.useFunc;
		inputBranch = copy.inputBranch;
		if (copy.input != nullptr) {
			if (input != nullptr) {
				for (int i = 0; i < amountInp; i++) {
					delete[] input[i];
				}
				delete[] input;
			}
			input = new int* [amountInp];
			for (int i = 0; i < amountInp; i++) {
				input[i] = new int[2];
				input[i][0] = copy.input[i][0];
				input[i][1] = copy.input[i][1];
			}
		}
		if (copy.coefficients != nullptr) {
			if (coefficients != nullptr)
				delete[] coefficients;
			coefficients = new double[amountInp+1];
			for (int i = 0; i < amountInp+1; i++) {
				coefficients[i] = copy.coefficients[i];
			}
		}
		

	}
	Neuron(bool inputBranch, int numberInput):inputBranch(inputBranch),useFunc(numberInput) {
		input = new int*[0];
		output = false;
	}
	Neuron(int useFunc):useFunc(useFunc){};
	
	void setCoefficients(double* coef) {
		coefficients = new double[amountInp + 1];
		for (int i = 0; i < amountInp + 1; i++) {
			coefficients[i] = coef[i];
		}
	}

	double getValue(function <double(double)> &funcActivation, double* inputs) {
		if (inputBranch) {
			return inputs[useFunc];
		}
		double sum = 0;
		for (int i = 0; i < amountInp; i++) {
			sum += coefficients[i] * inputs[i];
		}
		sum += coefficients[amountInp];
		return funcActivation(sum);
	}

	int getUseFunc() {
		return useFunc;
	}
	bool getInputBranch() {
		return inputBranch;
	}
	int getAmountInp() {
		return amountInp;
	}

	void connect(int amount,int* xOutputs, int* yOutputs, int x, int y)//Вводить х и у те, относительно коннекчущего узла
	{
		if (input == nullptr) {//РАссмотрен случай когда справа ТОЧНО не окажется входящих узлов
			input = new int* [amount];
			Neuron::amountInp = amount;
			for (int i = 0; i < amount; i++) {
				input[i] = new int[2];
				input[i][0] = xOutputs[i] - x;
				input[i][1] = yOutputs[i] - y;
			}
		}
		else {
			cout << endl << "Ошибка коннекта";
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
			if (input != nullptr) {
				for (int i = 0; i < amountInp; i++) {
					delete[] input[i];
				}
				delete[] input;
			}
			input = new int* [amountInp];
			for (int i = 0; i < amountInp; i++) {
				input[i] = new int[2];
				input[i][0] = copy.input[i][0];
				input[i][1] = copy.input[i][1];
			}
		}
		if (copy.coefficients != nullptr) {
			if (coefficients != nullptr)
				delete[] coefficients;
			coefficients = new double[amountInp + 1];
			for (int i = 0; i < amountInp + 1; i++) {
				coefficients[i] = copy.coefficients[i];
			}
		}
		return *this;
	}
};

