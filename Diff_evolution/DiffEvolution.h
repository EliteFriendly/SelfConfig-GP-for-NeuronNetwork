#pragma once
#include "../general/computing_limitation.h"
#include "DiffMutation.h"
#include "IndividualDiffEvolution.h"
#include <algorithm>
#include <cmath>
#include <ctime>
#include <functional>
#include <iostream>
#include "../general/general_var.h"

using namespace std;

class DiffEvolution
{
  private:
    function<double(double *)> overFittingFunc;
    function<double(double *)> func;
    string aim;      // Цель задачи
    double F;        // Масштабирующий фактор
    double Cr;       // Вероятность скрещивания
    int N;           // Количество индивидов
    int generations; // Количество поколений

    IndividualDiffEvolution *arrIndividuals; // Вектор содержащий целевые вектора
    IndividualDiffEvolution best;            // Лучшее найденное решение
    IndividualDiffEvolution bestPation;      // Best solution before overfitting
    double *trackBest;                       // Отслеживание лучшего решения
    double rejectionRate = 0.3;              // Part of all generation, which not improve the quality of solution

    int maxPartPatience = 0.1; // Part of all generation, before overfitting
    int currentPatience = 0;

    int ammDimens;                     // Количество измерений(осей)
    double *limitsDimension = nullptr; // Ограничения каждой оси

    DiffMutation mutation;

    IndividualDiffEvolution crossover(IndividualDiffEvolution);

    void surviveCrossover(IndividualDiffEvolution);

    void saveBest();

    bool networkQualityCheck(int generation); // This function reject the bad neural network to save recources
    bool overFittingCheck();                  // This function reject the bad neural network to save recources

  public:
    DiffEvolution(function<double(double *)> func, double *limitsDimension, int ammDimens, string typeMut, string aim)
        : ammDimens(ammDimens), func(func), aim(aim)
    {
        DiffEvolution::limitsDimension = new double[ammDimens * 2];

        for (int i = 0; i < ammDimens * 2; i++)
        {
            DiffEvolution::limitsDimension[i] = limitsDimension[i];
        }

        mutation.setType(typeMut);
        mutation.setLimits(limitsDimension, ammDimens);
    }
    void startSearch(double acc, double F, double Cr, int N, int generation, ComputingLimitation &cl);
    void setOverFittingFunc(function<double(double *)> func)
    {
        overFittingFunc = func;
    }
    double *getBestCoordinates()
    {
        return best.getCoordinats();
    }

    double getBestFuncValue()
    {
        return func(best.getCoordinats());
    }
    double getBestFitness()
    {
        return best.getFitness();
    }
    void setRejectionRate(double rejectionRate)
    {
        this->rejectionRate = rejectionRate;
    }

    ~DiffEvolution()
    {
        if (limitsDimension != nullptr)
        {
            delete[] limitsDimension;
            limitsDimension = nullptr;
        }
        if (arrIndividuals != nullptr)
        {
            delete[] arrIndividuals;
            arrIndividuals = nullptr;
        }
        if (trackBest != nullptr)
        {
            delete[] trackBest;
            trackBest = nullptr;
        }
    }
};
