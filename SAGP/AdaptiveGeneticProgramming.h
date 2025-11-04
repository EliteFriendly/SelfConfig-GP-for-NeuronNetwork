#pragma once
#include "../general/computing_limitation.h"
#include "../general/sample_storage.h"
#include "../neuron_network/Tree.h"
#include "CrossoverGP.h"
#include "FormingGP.h"
#include "MutationGP.h"
#include "SelectionGP.h"
#include <atomic>
#include <condition_variable>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class AdaptiveGeneticProgramming
{
  private:
    string typeTask; // Либо "reg" либо "class"
    int treeDepth;
    Tree bestIndividual;
    int numIndividuals;
    int numGeneration;
    Tree *arrayIndividuals = nullptr;
    Tree *arrayChildren = nullptr;

    double socialCard = 0.1; // Минимальная вероятность выбора

    ComputingLimitation computingLimitation; // Limits on amount calculations fitness function
    double dataTrainPart = 0.75;             // Part of data for training
    int ammInputs;                           // Количество осей или входов
    int ammOutputs;                          // Количество выходов или выходных нейронов
    int size;                                // Количество точек
    string markFile = "1";                      // Используется для именовании файла
    bool saveTrail = false; // Используется для сохранения результатов в момент поиска лучшего индивида
    ofstream fileTrail; // Используется для сохранения результатов в момент поиска лучшего индивида

    int ammSel = 5;
    SelectionGP **selection = nullptr;
    /*
    0-ранговая
    1-пропорциональная
    2-4 турнирная(3,5,10)
    */
    double *selProbabilities = nullptr;
    int *chosenSel = nullptr;
    ofstream fSel;

    int ammCross = 4;
    CrossoverGP **crossover = nullptr;
    /*
    0-пустое
    1-стандарт
    2-одноточечное
    3-равномерное
    */
    double *crossProbabilities = nullptr;
    int *chosenCross = nullptr;
    ofstream fCross;

    int ammMut = 4;
    MutationGP **mutation = nullptr;
    /*
    0-дерево
    1-слабая
    2-средняя
    3-сильная
    */
    double *mutProbabilities = nullptr;
    int *chosenMut = nullptr;
    ofstream fMut;

    FormingGP forming;

    void findBest();

    int probabilityChoice(double *arrProb, int size)
    {
        mt19937 gen(rand());
        double sum = arrProb[0];
        int number = 0;
        double probability = gen() % 1'000'000 / 1000000.0;
        while (probability > sum)
        {
            number++;
            if (number == size)
                return number - 1;
            sum += arrProb[number];
        }
        return number;
    }

    void setSelectionsArrays();

    Tree createChild(int);

    void threadsFitnessCalc(SampleStorage &data, int ammThread);

    int findWinner(int *arr, int ammPlayer, double *arrFitness);

    void changeProbabilities(double *arrProba, int numWinner, int ammPlayers);

    void recalcProbabilities();

    void saveProbabilities()
    {
        for (int i = 0; i < ammSel; i++)
            fSel << selProbabilities[i] << '\t';
        fSel << endl;
        for (int i = 0; i < ammMut; i++)
        {
            fCross << crossProbabilities[i] << '\t';
            fMut << mutProbabilities[i] << '\t';
        }
        fCross << endl;
        fMut << endl;
    }

  public:
    AdaptiveGeneticProgramming(int treeDepth, string typeTask) : treeDepth(treeDepth), typeTask(typeTask)
    {

        selection = new SelectionGP *[5];
        selection[0] = new RankedSelection;
        selection[1] = new ProportionalSelection;
        selection[2] = new TournamentSelection(3);
        selection[3] = new TournamentSelection(5);
        selection[4] = new TournamentSelection(10);

        crossover = new CrossoverGP *[4];
        crossover[0] = new EmptyCrossover;
        crossover[1] = new StandartCrossover;
        crossover[2] = new OnepointCrossover;
        crossover[3] = new UniformCrossover;

        mutation = new MutationGP *[4];
        mutation[0] = new TreeMutation(treeDepth - 1);
        mutation[1] = new PointMutation("Weak");
        mutation[2] = new PointMutation("Average");
        mutation[3] = new PointMutation("Strong");

        selProbabilities = new double[5];
        crossProbabilities = new double[4];
        mutProbabilities = new double[4];

        for (int i = 0; i < 5; i++)
            selProbabilities[i] = 1.0 / 5;
        for (int i = 0; i < 4; i++)
        {
            crossProbabilities[i] = 0.3;
            mutProbabilities[i] = 1.0 / 4;
        }
        crossProbabilities[0] = 0.1; // Ибо там пустой кроссовер
    }

    void startTrain(double **x, int ammInputs, int amOutPuts, int size, int numIndividuals, int numGeneration);
    Tree getBest()
    {
        return bestIndividual;
    }
    double getError(double **x, int size)
    {
        if (typeTask == "reg")
        {
            double sum = 0; // Среднеквадратичная ошибка
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < ammOutputs; j++)
                {
                    double res = bestIndividual.getValue(x[i])[j];
                    sum += pow(res - x[i][ammInputs + j],
                               2); // x[i][ammInputs + j] - это значение из обучающей выборки
                }
            }
            sum = pow(sum / (ammOutputs * size), 0.5); // Среднеквадратичная ошибка
            return sum;
        }
        if (typeTask == "class")
        {
            int count = 0;
            for (int i = 0; i < size; i++)
            {
                double *res = bestIndividual.getValue(x[i]);
                int maxIndex = 0;
                for (int j = 1; j < ammOutputs; j++) // Ищем индекс максимального значения
                    if (res[j] > res[maxIndex])
                        maxIndex = j;
                if ((maxIndex + 1) == x[i][ammInputs])
                { // Если индекс совпадает с номером класса, то
                  // увеличиваем счетчик
                    count++;
                }
                delete[] res;
            }
            return double(count) / size;
        }
        else
        {
            throw "Error: wrong type of task";
        }
    }

    void setDataTrainPart(double part)
    {
        dataTrainPart = part;
    }
    void setComputingLimitation(int limit)
    {
        computingLimitation.setComputingLimitation(limit);
    }
    void numFileAndTrail(string special, bool saveTrail = false)
    {
        markFile = special;
        AdaptiveGeneticProgramming::saveTrail = saveTrail;
        fileTrail.open("algorithm_results/trail/" + markFile + ".txt");
        if (!fileTrail.is_open())
        {
            throw "Error: can't open file for trail";
        }
    }
    void saveBestIndividualtoFile()
    {

        bestIndividual.saveNetworkToFile("algorithm_results/best"+markFile + "_best.txt");
    }

    ~AdaptiveGeneticProgramming()
    {
        if (arrayIndividuals != nullptr)
            delete[] arrayIndividuals;
        if (arrayChildren != nullptr)
            delete[] arrayChildren;
    }
};
