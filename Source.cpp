#include <iostream>
#include <vector>

#include "Tree.h"

#include <fstream>

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
* ÇÀÏÎÌÍÈÒÜ 
* random_device rd;

		mt19937_64 gen(rd());
		ÃÅÍÅÐÀÒÎÐ ÁÅÇ ÎÃÐÀÍÈ×ÅÍÈÉ


*Èç çà ÷åãî àëãîðèòì ìîæåò ïåðåñòàòü ðàáîòàòü:


*×òî íóæíî èçìåíèòü:



*Îïòèìèçàöèÿ ìîìåíò:


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





void main() {
	srand(2);
	Tree proba(3, 2);
	proba.doNeuronNetwork();
	cout << proba.getFunc() << endl;
	cout<<proba.getMatrix();


	
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