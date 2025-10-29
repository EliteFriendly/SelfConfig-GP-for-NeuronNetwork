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
    SampleStorage storage(size, dimension - 1, data, 100, "class"); // 75% for training

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

    std::vector<std::string> file_names = {
    "I_6_20b.txt",
    "I_8_14.txt", 
    "I_12_1.txt",
    "I_12_2.txt",
    "I_12_4.txt",
    "I_14_3.txt",
    "I_14_4.txt",
    "I_15_3x.txt",
    "I_15_10.txt",
    "I_18_4.txt",
    "I_24_6.txt",
    "I_32_5.txt"
    };

// Массив количества изменяемых параметров для каждой задачи
    std::vector<int> parameter_counts = {
    2,  // I.6.20b: theta, t
    4,  // I.8.14: x1, y1, x2, y2
    3,  // I.12.1: q1, q2, r
    4,  // I.12.2: q, E, v, B
    2,  // I.12.4: mu, r
    2,  // I.14.3: m, h
    2,  // I.14.4: k, x
    3,  // I.15.3x: x1, u, t
    2,  // I.15.10: m, v
    4,  // I.18.4: m, v, r, theta
    2,  // I.24.6: n, theta2
    2   // I.32.5: q, a
    };
    string st = "test/" + file_names[0];
    //cout << st << endl;
    ifstream file(st);
    if (!file.is_open())
    {
        cerr << "Error opening files" << endl;
        exit(1);
    }
    int size = 1000;
    dimension = parameter_counts[0];
    double** data = new double* [size];
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
    int treeDepth = 3;
    file.close();
    AdaptiveGeneticProgramming proba(treeDepth, "reg");
    proba.numFile(123);
    proba.startTrain(data , dimension, 1 , size , 10 , 10);
    Tree best = proba.getBest();
    cout << best.getFunc() << endl;
    cout << best.getMatrix() << endl;
    cout << best.getCoordStr() << endl;
    cout << proba.getError(data,size)<<endl;
    


    cout << "Good";

    printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
    return 0;
}
