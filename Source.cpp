#include <iostream>
#include <vector>
#include "AdaptiveGeneticProgramming.h"
#include "Tree.h"

#include <fstream>
#include "MutationGP.h"
#include "CrossoverGP.h"
#include <time.h>
#include <fstream>
using namespace std;
int dimension = 4;
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
Èçìåíåí ñïîñîá êîïèðîâàíèÿ âåòîê

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


	//Download database Iris
	ifstream file("Iris.txt");
	if (!file.is_open()) {
		cerr << "Error opening file" << endl;
		exit(1);
	}
	//Read data from file
	double** data = new double* [150];
	for (int i = 0; i < 150; i++) {
		data[i] = new double[5];
		for (int j = 0; j < 5; j++) {
			file >> data[i][j];
			if (file.peek() == ',') {
				file.ignore();
			}
		}
	}




	int treeDepth = 3; //depth of tree
	int amOutputs = 1; //number of outputs
	CrossoverGP** crossover = new CrossoverGP * [4];
	crossover[0] = new EmptyCrossover();
	crossover[1] = new UniformCrossover();
	crossover[2] = new StandartCrossover();
	crossover[3] = new OnepointCrossover();

	MutationGP** mutation = new MutationGP * [2];
	mutation[0] = new PointMutation("Average");
	mutation[1] = new TreeMutation(treeDepth);
	//do array from 1 to 100
	srand(10);
	setlocale(LC_ALL, "Russian");
	//generate train data 
	int str = 150; //number of train data
	/*double a = -4;
	double b = 4;
	double h = (b - a) / str;
	double** trainData = new double* [str];
	for (int i = 0; i < str; i++) {
		trainData[i] = new double[dimension + 1];
		for (int j = 0; j < dimension; j++) {
			trainData[i][j] = a + h * i;
			trainData[i][dimension] = addNoise(funcRastrigin(trainData[i]), 10);
		}
	}*/

	try {
		AdaptiveGeneticProgramming proba(treeDepth);
		proba.startTrain(data, dimension,amOutputs, str, 20, 20);
		Tree best = proba.getBest();
		cout << "Best fitness: " << best.getFitness() << endl;
		cout << "Error: " << proba.classificationError(data, str) << endl;
		proba.saveBestIndividualtoFile();
	}
	catch (const std::exception& e) {
		cout << "Exception caught: " << e.what() << endl;
		exit(0);
	}
	catch (...) {
		cerr << "Unknown exception caught" << endl;
		exit(0);
	}









	

	cout.precision(6);

	//do stress test
	//Stress test with all mutations and crossover




	
	//for (int c = 0; c < 4; c++) {
	//	for (int m = 0; m < 2; m++) {
	//		for (int i = 0; i < 1000; i++) {
	//			srand(i+ c*100000+m*10000);
	//			
	//			try {
	//				Tree parent1(treeDepth, dimension, 1);
	//				parent1.doNeuronNetwork();

	//				Tree parent2(treeDepth, dimension, 1);
	//				parent2.doNeuronNetwork();


	//				Tree son = crossover[c]->getChild(parent1, parent2);//ÃÄÅ ÒÎ ÇÄÅÑÜ ÓÒÅ×ÊÀ
	//				mutation[m]->doMutChild(son);
	//				son.doNeuronNetwork();
	//				son.trainWithDE(trainData, str);
	//				son.calcFitness(trainData, str);
	//									
	//				double fitness = son.getFitness();

	//				if (fitness < 0 or fitness > 1 or fitness == NULL) {
	//					throw invalid_argument("Fitness is NULL or less than 0 or more than 1");
	//				}
	//				cout << "Fitness: " << son.getFitness() << " in " << i << " cycle " << "Crossover: " << c << " Mutation: " << m << endl;
	//				//cout << proba.getMatrix() << endl;
	//				//cout << proba.getValue(pr)[0] << ' ';

	//			}
	//			catch (const std::exception& e) {
	//				cout << "Exception caught: " << e.what() << endl;
	//				cout << "with next settings:" << endl;
	//				cout << "rand: " << i + c * 100000 + m * 10000 << endl;
	//				cout << "Crossover: " << c << endl;
	//				cout << "Mutation: " << m << endl;
	//			
	//				exit(0);
	//			}
	//			catch (...) {
	//				cerr << "Unknown exception caught" << endl;
	//			}
	//		}
	//	}
	//}
	//


    cout << "Good";


	
	

}