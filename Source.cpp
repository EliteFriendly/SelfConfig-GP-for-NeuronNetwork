#include "SAGP/AdaptiveGeneticProgramming.h"
#include "neuron_network/Tree.h"
#include <fstream>
#include <iostream>
#include <locale.h>
#include <time.h>
#include <vector>
#include "general/sample_storage.h"
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
    AdaptiveGeneticProgramming proba(treeDepth, "class");
    proba.numFile(number);

    proba.startTrain(data, dimension - 1, amOutputs, size, 30, 30);
    Tree best = proba.getBest();
    // fileOut << proba.getError(dataTest, size * 0.25) << endl;
    fileOut << proba.getError(storage.getTrainData(), storage.getTrainSize()) << endl;
    fileOut << proba.getError(storage.getTestData(), storage.getTestSize()) << endl;
    proba.saveBestIndividualtoFile();
    fileOut.close();
}

int main()
{

    clock_t tStart = clock();

    string path[3] = {"test/Iris.txt", "test/wine.txt", "test/breast_cancer.txt"};
    int size[3] = {150, 178, 569};
    int amOutputs[3] = {3, 3, 2};
    int dimension[3] = {4 + 1, 13 + 1, 30 + 1};
    int depth[3] = {3, 3, 3};

    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            test(path[i], size[i], dimension[i], amOutputs[i], j + 10 * i, depth[i]);
        }
    }

    cout << "Good";

    printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
    return 0;
}
