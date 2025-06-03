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


//n-dimension functions for tests
double funcSphere(double* x) {
	double y = 0;
	for (int i = 0; i < dimension; i++) {
		y += pow(x[i], 2);
	}
	return y;
}




double addNoise(double x, int power) {
	if (rand() % 2) {
		return (x + double(rand() % power + rand() % 1) / 100.0 * x);
	}
	else {
		return (x - double(rand() % power + rand() % 1) / 100.0 * x);
	}
}



//Âñå íåéðîíû 7 òèïà!!!!!!!!

void main() {
	//do array from 1 to 100
	srand(10);
	setlocale(LC_ALL, "Russian");
	//generate train data 
	int str = 50; //number of train data
	double a = -4;
	double b = 4;
	double h = (b - a) / str;
	double** trainData = new double* [str];
	for (int i = 0; i < str; i++) {
		trainData[i] = new double[dimension+1];
		for (int j = 0;j < dimension; j++) {
			trainData[i][j] = a + h * i;
			trainData[i][dimension] = addNoise(funcRastrigin(trainData[i]), 10);
		}
	}
	//cout train data
	//for (int i = 0; i < str; i++) {
	//	for (int j = 0; j < dimension + 1; j++) {
	//		cout << trainData[i][j] << ' ';
	//	}
	//	cout << endl;
	//}


	double* data = new double[1000000];
	double* pr = new double[2]{ 1.0,2.0 };
	for (int i = 0; i < 1000000; i++) {
		data[i] = 2;

	}

	//srand(4);
	//Tree proba1(rand() % 4 + 2, dimension, 1);
	//proba1.doNeuronNetwork();
	//cout << proba1.getMatrix() << endl;
	//proba1.trainWithDE(trainData, str);
	//proba1.calcFitness(trainData, str);
	//cout << "Fitness END: " << proba1.getFitness() << endl;

	//cout << proba1.getValue(pr)[0] << ' ';

	/*srand(109);
	Tree proba(rand() % 2 + 2, dimension, 1);
	proba.doNeuronNetwork();
	proba.trainWithDE(trainData, str);
	proba.calcFitness(trainData, str);


	cout << "Fitness END: " << proba.getFitness() << endl;
	cout << proba.getMatrix() << endl;*/



	//do stress test
	
	for (int i = 0; i < 400; i++) {
		srand(i);
		try {
			Tree proba(rand()%4+2, dimension, 1);
			proba.doNeuronNetwork();
			proba.trainWithDE(trainData, str);
			proba.calcFitness(trainData, str);
			cout << "Fitness: " << proba.getFitness()<<" in "<< i <<" cycle" << endl;
			//cout << proba.getMatrix() << endl;
			//cout << proba.getValue(pr)[0] << ' ';
		}
		catch (const std::exception& e) {
			cout << "Exception caught: " << e.what() << endl;
			cout << "with next settings:" << endl;
			cout << "i: " << i << endl;
			cout << "rand: " << rand() % 4 + 2 << endl;
			
		}
		catch (...) {
			cerr << "Unknown exception caught" << endl;
		}
	}





	
	//MutationGP* cro = new TreeMutation(3);
	//for (int i = 0; i < 1000; i++) {
	//	srand(i);
	// 	
	//	int p1 = 0;
	//	proba1.countNodes(p1);
	//	//cout << proba1.getFunc() << endl;
	//	cro->doMutChild(proba1);
	//	//cout << proba1.getFunc() << endl;
	//	proba1.doNeuronNetwork();
	//	//cout << proba1.getMatrix();
	//	cout << i << endl;
	//}


	



	
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