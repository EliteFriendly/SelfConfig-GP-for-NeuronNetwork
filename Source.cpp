#include <iostream>
#include <vector>

#include "Tree.h"

#include <fstream>
#include "MutationGP.h"
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
	MutationGP* cro = new TreeMutation(3);
	for (int i = 0; i < 1000; i++) {
		srand(i);
	 	Tree proba1(3, 2);
		int p1 = 0;
		proba1.countNodes(p1);
		//cout << proba1.getFunc() << endl;
		cro->doMutChild(proba1);
		//cout << proba1.getFunc() << endl;
		proba1.doNeuronNetwork();
		//cout << proba1.getMatrix();
		cout << i << endl;
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