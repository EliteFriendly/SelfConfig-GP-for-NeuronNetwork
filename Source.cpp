#include "SAGP/AdaptiveGeneticProgramming.h"
#include "neuron_network/Tree.h"
#include <fstream>
#include <iostream>
#include <locale.h>
#include <time.h>
#include <vector>
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

int main()
{
    clock_t tStart = clock();
    cout << endl;
    // Download database Iris
    ifstream file("test/Friedman1_noise1.0_full.txt");
    int size = 200;
    int dimension = 11;
    if (!file.is_open())
    {
        cerr << "Error opening file" << endl;
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
        //cout << endl;
    }

    int treeDepth = 3; // depth of tree
    int amOutputs = 3; // number of outputs

    // do array from 1 to 100

    setlocale(LC_ALL, "Russian");


    try
    {
        AdaptiveGeneticProgramming proba(treeDepth, "reg");
        proba.startTrain(data, dimension, amOutputs, size, 20, 20);
        Tree best = proba.getBest();
        cout << "Best fitness: " << best.getFitness() << endl;
        cout<< "Error: "<< proba.getError(data, size) << endl;
        proba.saveBestIndividualtoFile();
    }
    catch (const std::exception &e)
    {
        cout << "Exception caught: " << e.what() << endl;
        exit(0);
    }
    catch (...)
    {
        cerr << "Unknown exception caught" << endl;
        exit(0);
    }

    cout.precision(6);

    cout << "Good";

    printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
    return 0;
}
