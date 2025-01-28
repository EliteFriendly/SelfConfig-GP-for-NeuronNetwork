#pragma once
class Neuron
{
	//Здесь находятся связи с узлами относительно нейрона
	int** input = nullptr;
	bool ouput = false;

	int ammInputs = 0;

	
	int useFunc;

	void addInput(int xInput, int yInput) {
		if (input == nullptr) {
			//Создание с нуля если еще ничего не было
			input = new int* [1];
			ammInputs++;
			input[0] = new int[2];
			input[0][0] = xInput;
			input[0][1] = yInput;
			return;
		}
		
		int** tmp = new int*[ammInputs];
		tmp = input;
		//Копирование с одновременной чисткой
		for (int i = 0; i < ammInputs; i++) {
			tmp[i][0] = input[i][0];
			tmp[i][1] = input[i][1];
			delete[] input[i];
		}
		delete[]input;
		ammInputs++;
		input = new int* [ammInputs];
		//Копирование в увеличенный массив
		for (int i = 0; i < ammInputs - 1; i++) {
			input[i] = new int[2];
			input[i][0] = tmp[i][0];
			input[i][1] = tmp[i][1];
			delete[] tmp[i];
		}
		delete[] tmp;
		input[ammInputs - 1][0] = xInput;
		input[ammInputs - 1][1] = yInput;


	}

public:
	Neuron(int useFunc):useFunc(useFunc){};
	
	void connectTo(Neuron& second,int* xOutput, int* yOutput)//Вводить х и у те, куда идет подключение
	{
		
	}

};

