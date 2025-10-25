#include "SAGP/AdaptiveGeneticProgramming.h"
#include "neuron_network/Tree.h"
#include <fstream>
#include <iostream>
#include <locale.h>
#include <time.h>
#include <vector>
#include "general/sample_storage.h"
#include "general/general_var.h"
using namespace std;
int dimension = 4;
const double PI = 3.1415926535;

double func0(double x)
{
    return x;
}
double func1(double x)
{
    return 3 * sin(x) + 5 * cos(x);
}

double func2(double x)
{
    return pow(10 * x, 0.5);
}
double func3(double *x)
{
    return pow(x[0], 1.5) + 10 * sin(x[0]);
}

double funcTest(double *x)
{
    return x[0] + x[1];
}

double funcRastrigin(double *x)
{
    double y = 0;
    for (int i = 0; i < dimension; i++)
        y += (pow(x[i], 2) - cos(2 * PI * x[i]) + 10);
    return y;
}

double funcRosenbrock(double *x)
{
    double y = 0;
    for (int i = 0; i < dimension - 1; i++)
        y += (100 * pow(pow(x[i], 2) - x[i + 1], 2) + pow(x[i] - 1, 2));
    return y;
}

/*
* ЗАПОМНИТЬ
* random_device rd;

                mt19937_64 gen(rd());
                ГЕНЕРАТОР БЕЗ ОГРАНИЧЕНИЙ
                НО ВРОДЕ ГЕНЕРИРУЕТ И ОТРИЦАТЕЛЬНЫЕ ЧИСЛА

*Из за чего алгоритм может перестать работать:
Изменен способ копирования веток

*Что нужно изменить:



*Оптимизация момент:


*/

// n-dimension functions for tests
double funcSphere(double *x)
{
    double y = 0;
    for (int i = 0; i < dimension; i++)
        y += pow(x[i], 2);
    return y;
}

double addNoise(double x, int power)
{
    if (rand() % 2)
        return x + double(rand() % power + rand() % 1) / 100.0 * x;
    else
        return x - double(rand() % power + rand() % 1) / 100.0 * x;
}

void test(string path, int size, int dim, int amOutputs, int number, int depth)
{
    ifstream file(path);
    dimension = dim;
    if (!file.is_open())
    {
        cerr << "Error opening files" << endl;
        exit(1);
    }
    // Read data from file
    double **data = new double *[size];
    for (int i = 0; i < size; i++)
    {

        data[i] = new double[dimension];

        for (int j = 0; j < dimension; j++)
        {
            file >> data[i][j];
            if (file.peek() == ',')
                file.ignore();
            // cout << data[i][j] << " ";
        }


        // cout << endl;
    }
    file.close();
    SampleStorage storage(size, dimension - 1, data, 0.75, "class"); // 75% for training

    int treeDepth = depth; // depth of tree

    ofstream fileOut("algorithm_results/Results/Error" + to_string(number) + ".txt");
    if (!fileOut.is_open())
    {
        cout << "Error opening file out" << endl;
        exit(1);
    }
    cout << "Iteration " << number << endl;
    AdaptiveGeneticProgramming proba(treeDepth, "reg");
    proba.numFile(number);

    proba.startTrain(data, dimension - 1, 1, size, 10, 10);
    Tree best = proba.getBest();
    // fileOut << proba.getError(dataTest, size * 0.25) << endl;
    fileOut << proba.getError(storage.getTrainData(), storage.getTrainSize()) << endl;
    fileOut << proba.getError(storage.getTestData(), storage.getTestSize()) << endl;
    //proba.saveBestIndividualtoFile();
    fileOut.close();
}

int main()
{
    setlocale(0,"");
    clock_t tStart = clock();

    string path[3] = {"test/Iris.txt", "test/wine.txt", "test/breast_cancer.txt"};
    int size[3] = {150, 178, 569};
    int amOutputs[3] = {3, 3, 2};
    int dimension[3] = {4 + 1, 13 + 1, 30 + 1};
    int depth[3] = {3, 3, 3};

    double* coef = new double[1000];
    for(int i = 0; i < 1000; i++)
    {
        coef[i] = 1;
    }
    //srand(105);
    /*Tree proba(3 , 2 , 1 , "reg");
    proba.doNeuronNetwork();
    proba.changeCoef(coef);
    cout <<"Proba: "<< proba.getFunc() << endl;
    cout << proba.getValue(coef)[0] << endl;
    cout << proba.getValue(coef)[0] << endl;
    
    cout << proba.getValue(coef)[0] << endl;

    exit(0);
    */


    //Stress test
    int treeDepth = 6;


    CrossoverGP** crossover = new CrossoverGP * [4];
    crossover[0] = new EmptyCrossover;
    crossover[1] = new StandartCrossover;
    crossover[2] = new OnepointCrossover;
    crossover[3] = new UniformCrossover;

    MutationGP **mutation = new MutationGP *[2];
    mutation[0] = new TreeMutation(treeDepth - 1);
    mutation[1] = new PointMutation("Strong");

  
    for (int j = 0; j < 4; j++) {
        for (int k = 0; k < 2; k++) {
            for (int l = 0; l < 10000; l++) {
                cout<<j<< " " << k << " " << l << endl;
                try {
                    Tree proba1(treeDepth , 2 , 1 , "reg");
                    Tree proba2(treeDepth , 2 , 1 , "reg");
                    proba1.doNeuronNetwork();
                    proba2.doNeuronNetwork();

                    Tree child = crossover[j]->getChild(proba1 , proba2);
                    child.doNeuronNetwork();

                    
                    mutation[k]->doMutChild(child);
                    child.doNeuronNetwork();
                    child.changeCoef(coef);
                    //cout << proba.getCoordStr();
                    child.getValue(coef)[0];
                    }
                catch (const std::exception& e) {
                    cout << e.what() << endl;
                    cout << "Crossover: " << j << endl;
                    cout << "Mutation: " << k << endl;
                    cout << "Tree: " << l << endl;
                }
            }

        }
    }
    

        
    /*Tree proba1(3 , 2 , 1 , "reg");
    Tree proba2(3 , 2 , 1 , "reg");
    proba1.doNeuronNetwork();
    proba2.doNeuronNetwork();
    cout <<"Proba 1: "<< proba1.getFunc() << endl;
    cout <<"Proba 2: "<< proba2.getFunc() << endl;
    CrossoverGP* cossover = new StandartCrossover;
    MutationGP* mutation = new PointMutation("Strong");
    Tree child = cossover->getChild(proba1 , proba2);
    child.doNeuronNetwork();
    cout << "Child: " << child.getFunc() << endl;
    
    mutation->doMutChild(child);
    child.doNeuronNetwork();
    cout << "Child after mutation: " << child.getFunc() << endl;
    cout << child.getMatrix() << endl;
    child.changeCoef(coef);
    //cout << proba.getCoordStr();
    cout << child.getValue(coef)[0] << endl;
    cout << child.getValue(coef)[0] << endl;
    
    cout << child.getValue(coef)[0] << endl;
    */
    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 1; j++)
        {
           // test(path[i], size[i], dimension[i], amOutputs[i], j + 10 * i, depth[i]);
        }
    }

    cout << "Good";

    printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
    return 0;
}
