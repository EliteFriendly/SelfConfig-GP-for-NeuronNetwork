#include <functional>
#include <iostream>
#include <sstream>
#include "../general/general_var.h"

using namespace std;
class Neuron
{

    // Here parameters for RNN connections
    int **inputRNN = nullptr;     // Координаты входящих узлов реккурсия [amountInpRNN][2]
    double *weightsRNN = nullptr; // Вес связей реккурсия
    int amountInpRNN = 0;         // Только количество входов реккурсия
    bool haveRNN = false;         // Есть ли рекурсивные связи

    double value = 0.0; // Output value from neuron

    // Здесь находятся связи с узлами относительно нейрона
    int **input = nullptr; // Координаты входящих узлов [amountInp][2]
    bool output = false;   // Есть ли выходные узлы
    int amountInp = 0;     // Только количество входов

    int useFunc = -1;         // Номер используемой функции, или входа(от 0), -1 значит нейрон не используется
    bool inputBranch = false; // Находится ли в ветви входа

    double *weights = nullptr; // Веса вместе со свободным

  public:
    Neuron() {};
    Neuron(const Neuron &copy); // Конструктор копирования
    Neuron(bool inputBranch, int numberInput) : inputBranch(inputBranch), useFunc(numberInput)
    {
        input = new int *[0];
        output = false;
    }
    Neuron(int useFunc) : useFunc(useFunc) {};

    void setWeights(double *coef, int &cursor);
    double getValue(function<double(double)> &funcActivation, double *inputs)
    {
        if (inputBranch)
        {
            return inputs[useFunc];
        }
        double sum = 0;
        for (int i = 0; i < amountInp; i++)
        {
            sum += weights[i] * inputs[i];
        }
        sum += weights[amountInp];
        if (haveRNN)
        {
            for (int i = 0; i < amountInpRNN; i++)
            {
                sum += weightsRNN[i] * inputs[amountInp + 1 + i];
            }
        }

        value = funcActivation(sum);
        return value;
    }
    double getValueRNN()
    {
        return value;
    }
    int getUseFunc()
    {
        return useFunc;
    }
    bool getInputBranch()
    {
        return inputBranch;
    }
    bool getHaveRNN()
    {
        return haveRNN;
    }
    int **getcoordRNN_firstL()
    {
        return inputRNN;
    }
    int getAmountInp()
    {
        return amountInp + amountInpRNN;
    }

    void connect(int amount, int *xOutputs, int *yOutputs, int x,
                 int y); // Вводить х и у те, относительно коннекчущего узла

    void addConnectRNN(int xOutputs, int yOutputs, int x,
                    int y); // Вводить х и у те, относительно коннекчущего узла

    string getStrCoord()
    {
        if (useFunc == -1)
            return "S"; // Означает что нейрон не используется, т.е. Shadow - тень
        stringstream ss;
        if (input == nullptr)
        {
            ss << " nullptr ";
            return ss.str();
        }
        for (int i = 0; i < amountInp; i++)
        {
            ss << "{ " << input[i][0] << " ; " << input[i][1] << " }";
        }
        if (haveRNN)
        {
            ss << " RNN=";
            for (int i = 0; i < amountInpRNN; i++)
            {
                ss << "| " << inputRNN[i][0] << " ; " << inputRNN[i][1] << " |";
            }
            return ss.str();
        }
    }
    int **getCoord()
    {
        return input;
    }
    bool getOutput()
    {
        return output;
    }
    bool getInput()
    {
        if (input == nullptr)
        {
            return false;
        }
        return true;
    }
    double *getWeights()
    {
        return weights;
    }
    void haveOutput()
    {
        output = true;
    }
    ~Neuron();
    Neuron operator=(const Neuron &copy)
    {
        output = copy.output;
        amountInp = copy.amountInp;
        useFunc = copy.useFunc;
        inputBranch = copy.inputBranch;
        if (copy.input != nullptr)
        {
            if (input != nullptr)
            {
                for (int i = 0; i < amountInp; i++)
                {
                    delete[] input[i];
                }
                delete[] input;
            }
            input = new int *[amountInp];
            for (int i = 0; i < amountInp; i++)
            {
                input[i] = new int[2];
                input[i][0] = copy.input[i][0];
                input[i][1] = copy.input[i][1];
            }
        }
        if (copy.weights != nullptr)
        {
            if (weights != nullptr)
                delete[] weights;
            weights = new double[amountInp + 1];
            for (int i = 0; i < amountInp + 1; i++)
            {
                weights[i] = copy.weights[i];
            }
        }
        return *this;
    }
};
