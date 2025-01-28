#include <iostream>
#include <vector>
#include "MutationGP.h"
#include "Tree.h"
#include "CrossoverGP.h"
#include <fstream>
#include "AdaptiveGeneticProgramming.h"
#include <time.h>
#include <fstream>
using namespace std;
int dimension = 1;
const double PI = 3.1415926535;

double func0(double x) {
	return x;
}
double func1(double x) {
	return 3*sin(x) + 5*cos(x);
}


double func2(double x) {
	return pow(10*x,0.5);
}
double func3(double* x) {
	return pow(x[0], 1.5) + 10 * sin(x[0]);
}


double funcTest(double* x) {
	return x[0] + x[1];
}

double funcRastrigin(double *x) {
	double y = 0;
	for (int i = 0; i < dimension; i++) {
		y += (pow(x[i], 2) - cos(2 * PI * x[i]) + 10);
	}
	return y;
}

double funcRosenbrock(double* x) {
	double y = 0;
	for (int i = 0; i < dimension - 1; i++) {
		y += (100 * pow(pow(x[i], 2) - x[i + 1], 2) + pow(x[i] - 1, 2));
	}
	return y;
}





/*
* ЗАПОМНИТЬ 
* random_device rd;

		mt19937_64 gen(rd());
		ГЕНЕРАТОР БЕЗ ОГРАНИЧЕНИЙ

ЭТОТ АЛГОРИТМ РЕАЛИЗОВАН ТОЛЬКО ДЛЯ 1 ВЫХОДА


!!!Не знаю что делать если функциональное множества меняется на термальное

*Из за чего алгоритм может перестать работать:


*Что нужно изменить:
Сделать поиск минимального расстояния до другого кластера




*Оптимизация момент:

В СЕЛЕКЦИИ 5 РАЗ СОРТИРУЕТСЯ МАССИВ ИНДИВИДОВ

НЕ УДАЛЯЕТСЯ МАССИВ РАЗМЕЧЕННЫХ ИНДИВИДОВ

*/

void yi(int* in) {
	in[0];
}


double addNoise(double x, int power) {
	if (rand() % 2) {
		return (x + double(rand() % power + rand() % 1) / 100.0 * x);
	}
	else {
		return (x - double(rand() % power + rand() % 1) / 100.0 * x);
	}
}

void qsortRecursive(int* mas, int size) {
	// Указатели в начало и в конец массива
	int i = 0;
	int j = size - 1;
	// Центральный элемент массива
	int mid = mas[size / 2];
	// Делим массив
	do {
		// Пробегаем элементы, ищем те, которые нужно перекинуть в другую часть
		// В левой части массива пропускаем (оставляем на месте) элементы, которые меньше центрального
		while (mas[i] < mid) {
			i++;
		}
		// В правой части пропускаем элементы, которые больше центрального
		while (mas[j] > mid) {
			j--;
		}
		// Меняем элементы местами
		if (i <= j) {
			int tmp = mas[i];
			mas[i] = mas[j];
			mas[j] = tmp;
			i++;
			j--;
		}
	} while (i <= j);
	// Рекурсивные вызовы, если осталось, что сортировать
	if (j > 0) {
		qsortRecursive(mas, j + 1);
	}
	if (i < size) {
		qsortRecursive(&mas[i], size - i);
	}
}



void doResearch(int number, int filename) {

	setlocale(0, "");
	int str = 200;
	int col = 2;

	ifstream file("Num_"+to_string(number)+".txt");


	double** data = new double* [str];
	double tmp;

	for (int i = 0; i < str; i++) {
		data[i] = new double[col];
		for (int j = 0; j < col; j++) {
			file >> data[i][j];
		}
		file >> tmp;
	}
	ofstream out("Points/Out_"+to_string(number) + to_string(filename) + ".txt");

	

	AdaptiveGeneticProgramming proba(1, 3);
	proba.numFile(filename+number*10);
	proba.startTrain(data, col, str, 20, 20);


	ofstream res("Results/Input_" + to_string(number) + to_string(filename) + ".txt");

	res << proba.getBest().getFunc()<<endl;
	res << proba.getBest().getFitness() << endl;;

	for (int i = 0; i < str; i++) {
		for (int j = 0; j < col; j++) {
			out << data[i][j]<<'\t';
		}
		out << proba.getBest().getLabel()[i];
		out << endl;
	}

	out.close();
	res.close();


	for (int i = 0; i < str; i++)
		delete[] data[i];

	delete[] data;

}




void main() {

	for (int i = 0; i < 5; i++) {
		cout << "Number = " << i<<endl;
		doResearch(3, i);
	}



	
	//srand(3);
	//AdaptiveGeneticProgramming proba(1.2, 3);
	//proba.startTrain(data, 2, str, 10, 7);
	////
	//// 
	//// 
	//for (int i = 0; i < str; i++) {
	//	cout<<proba.getBest().getLabel()[i]<<endl;
	//}
	//for (int i = 0; i < str; i++) {
	//	delete[] data[i];
	//}
	//delete[] data;

}