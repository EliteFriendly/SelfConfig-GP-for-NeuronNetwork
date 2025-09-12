#include <iostream>

using namespace std;

#pragma once
class SampleStorage
{
  private:
    double **trainData = nullptr;
    double **testData = nullptr;
    int size = 0;
    int amDimensions = 0;
    double trainPart;

    void classificationSort(double **Data)
    {
        int numbersOfClasses = 0;
        for (int i = 0; i < size; i++)
        {
            numbersOfClasses = max(numbersOfClasses, (int)trainData[i][amDimensions]);
        }
        double *amOfEveryClass = new double[numbersOfClasses];
        for (int i = 0; i < size; i++)
        {
            amOfEveryClass[(int)trainData[i][amDimensions]]++;
        }

        int *trainClass = new int[numbersOfClasses];
        int *usesClass = new int[numbersOfClasses];
        for (int i = 0; i < numbersOfClasses; i++)
        {
            trainClass[i] = trainPart * amOfEveryClass[i];
            usesClass[i] = 0;
        }
        int i1 = 0, i2 = 0;
        for (int j = 0; j < size; j++)
        {
            for (int w = 0; w < amDimensions; w++)
            {
                if (usesClass[(int)trainData[j][amDimensions]] < trainClass[(int)trainData[j][amDimensions]])
                {
                    trainData[i1][w] = Data[j][w];
                    usesClass[(int)trainData[j][amDimensions]]++;
                    i1++;
                }
                else
                {
                    testData[i2][w] = Data[j][w];
                    i2++;
                }
            }
        }

        delete[] amOfEveryClass;
        delete[] trainClass;
        delete[] usesClass;
    }

    double getDistance(double *a, double *b)
    {
        double sum = 0;
        for (int i = 0; i < amDimensions - 1; i++)
        {
            sum += (a[i] - b[i]) * (a[i] - b[i]);
        }
        return sqrt(sum);
    }

    void regressionSort(double **Data)
    {
        double **sortedData = new double *[size];

        for (int i = 0; i < size; i++)
        {
            sortedData[i] = new double[amDimensions];
            for (int j = 0; j < amDimensions; j++)
            {
                sortedData[i][j] = Data[i][j];
            }
        }
        // Start sorting by closer point
        for (int i = 0; i < size; i++)
        {
            double Bestdistance = 99999, distance;
            int number;
            for (int j = 1; j < size - 1; j++)
            {
                distance = getDistance(sortedData[i], sortedData[j]);

                if (distance < Bestdistance)
                {
                    Bestdistance = distance;
                    number = j;
                }
            }
            double *temp = sortedData[i];
            sortedData[i] = sortedData[number];
            sortedData[number] = temp;
        }
        double remainder = 1 - trainPart;
        double tmp = 0;
        for (int i = 0; i < size; i++)
        {
            tmp += remainder;
            for (int j = 0; j < amDimensions; j++)
            {
                if (tmp < 1)
                {
                    trainData[i][j] = sortedData[i][j];
                }
                else
                {
                    testData[i][j] = sortedData[i][j];
                }
            }
            if (tmp >= 1.0)
            {
                tmp -= 1.0;
            }
        }

        for (int i = 0; i < size; i++)
        {
            delete[] sortedData[i];
        }
        delete[] sortedData;
    }

  public:
    SampleStorage(int size, int amDimensions, double **data, double trainPart, string target)
    {
        this->size = size;
        this->amDimensions = amDimensions;
        this->trainData = new double *[trainPart * size];
        this->testData = new double *[(1 - trainPart) * size];
        this->trainPart = trainPart;
        if (target == "reg")
        {
            regressionSort(data);
        }
        if (target == "class")
        {
            classificationSort(data);
        }
    }
    double **getTrainData()
    {
        return trainData;
    }
    double **getTestData()
    {
        return testData;
    }
    ~SampleStorage()
    {
        if (trainData != nullptr)
        {
            delete[] trainData;
        }
        if (testData != nullptr)
        {
            delete[] testData;
        }
    }
};
