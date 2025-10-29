#pragma once
#include "../Diff_evolution/DiffEvolution.h"
#include "../Diff_evolution/IndividualDiffEvolution.h"
#include "../general/computing_limitation.h"
#include "../general/sample_storage.h"
#include "Neuron.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include <random>
#include "../general/general_var.h"
/*
Заходя суда помни!
номер слоя по x
количество нейронов по y
+------------------------------------>x
| [0;0] [1;0] [2;0] [3;0] [4;0] [5;0]
| [0;1] [...] [] [] [] []
| [] [] [] [] [] []
|
|
|
|
vy


*/






using namespace std;

class Tree
{
private:
    int numberFunc = -1; // Номер функции который используется в узле, в вершинах
    // функция активации
    int numVertices = 0; // Количество вершин где нужно настраивать коэффициенты
    // (не используется)
    int numNodes = -1;   // Количество узлов ниже
    int layerLevel = -1; // На каком уровне относительно начала находится узел
    int size = -1;       // Количество данных

    int numInput = -1;   // Номер входа
    int ammInputs = -1;  // Количество входов
    int ammOutputs = -1; // Количество выходов

    double fitness = -9'999'999; // Ну тут понятно

    bool lastVertice = false; // Последняя ли вершина
    bool inputBranch = false; // Является ли узел ветвью входа
    bool mainNode = false;    // Начальный ли узел
    bool unarFuncUs = false;  // Является ли узел функцией унарной

    //Связано с рекуррентной связью
    int** coordRNN_firstL = nullptr; //x,y: [2][ammRNN] координаты нейронов без входов с реккурентной связью, которые используются
    int amRNN = 0; //количество нейронов реккурентной связи

    // Связано с пригодностью
    double ef = 1;      // Коэффициент при RMSE
    double nf = 0;      // Коэффициент при количестве узлов
    int maxNodes = 100; // Максимальное количество узлов в дереве

    int* ammNeuron = nullptr; // Количество узлов в слое
    int ammLayers = 0;        // Количество слоев

    Neuron** network = nullptr; // Сеть нейронов без выходных

    Neuron* output = nullptr; // Нейроны на выход, без функции активации

    Tree* left = nullptr;
    Tree* right = nullptr;

    string typeTask = "reg";// Тип задачи, регрессия или классификация




    void doHiddenNeuron();

public:
    Tree()
    {}
    Tree(int depth , int ammInput , bool inputBranch);
    Tree(const Tree& copy)
    {

        if (copy.output != nullptr)
        {
            if (output != nullptr)
                delete[] output;
            // Выделяем память под выходные нейроны
            output = new Neuron[copy.ammOutputs];
            for (int i = 0; i < copy.ammOutputs; i++)
                output[i] = copy.output[i];
        }
        else if (output != nullptr)
        {
            delete[] output;
            output = nullptr;
        }

        if (copy.ammNeuron != nullptr)
        {
            if (ammNeuron != nullptr)
                delete[] ammNeuron;

            ammNeuron = new int[copy.ammLayers];
            for (int i = 0; i < copy.ammLayers; i++)
                ammNeuron[i] = copy.ammNeuron[i];
        }
        else if (ammNeuron != nullptr)
        {
            delete[] ammNeuron;
            ammNeuron = nullptr;
        }
        if (copy.network != nullptr)
        {
            if (network != nullptr)
            {
                for (int i = 0; i < ammLayers; i++)
                    delete[] network[i];
                delete[] network;
            }
            network = new Neuron * [copy.ammLayers];
            for (int i = 0; i < copy.ammLayers; i++)
            {
                network[i] = new Neuron[ammNeuron[i]]; // Обращаемся к своему, потому
                // что уже скопировали
                for (int j = 0; j < ammNeuron[i]; j++)
                    network[i][j] = copy.network[i][j];
            }
        }
        else if (network != nullptr)
        {
            for (int i = 0; i < ammLayers; i++)
                delete[] network[i];
            delete[] network;
            network = nullptr;
        }
        if (copy.coordRNN_firstL != nullptr) {
            if (coordRNN_firstL != nullptr) {
                for (int i = 0; i < 2; i++)
                    delete[] coordRNN_firstL[i];
                delete[] coordRNN_firstL;
            }
            amRNN = copy.amRNN;
            coordRNN_firstL = new int* [2];
            for (int i = 0; i < 2; i++) {
                coordRNN_firstL[i] = new int[amRNN];
                for (int j = 0; j < amRNN; j++) {
                    coordRNN_firstL[i][j] = copy.coordRNN_firstL[i][j]; 
                }
            }
        }

        unarFuncUs = copy.unarFuncUs;
        typeTask = copy.typeTask;
        ammOutputs = copy.ammOutputs;
        ammLayers = copy.ammLayers;
        mainNode = copy.mainNode;
        inputBranch = copy.inputBranch;
        size = copy.size;
        numberFunc = copy.numberFunc;
        layerLevel = copy.layerLevel;
        lastVertice = copy.lastVertice;
        numInput = copy.numInput;
        numVertices = copy.numVertices;
        numNodes = copy.numNodes;
        fitness = copy.fitness;
        ammInputs = copy.ammInputs;

        // Выделение памяти чтобы не было кучи взаимосвязанных индивидлв
        if (copy.left != nullptr)
        {
            if (left != nullptr)
                delete left;
            // left = new Tree; старый вариант при котором работало, но была утечка
            //*left = Tree(*(copy.left));
            left = new Tree(*(copy.left));
        }
        else if (left != nullptr)
        {
            delete left;
            left = nullptr;
        }
        if (copy.right != nullptr)
        {
            if (right != nullptr)
                delete right;
            /*right = new Tree; Старый вариант при котором работало, но была утечка
             *right = Tree(*(copy.right));*/
            right = new Tree(*(copy.right));
        }
        else if (right != nullptr)
        {
            delete right;
            right = nullptr;
        }
    }

    Tree(int d , int numInputs , int numOutputs , string typeTask);

    void calcFitness(double** x , int size);

    string getMatrix();

    void doNeuronNetwork();

    void setWeightsFitness(double ef , double nf , int maxN)
    {
        this->ef = ef;
        this->nf = nf;
        this->maxNodes = maxN;
    }

    double getFitness()
    {
        return fitness;
    }

    string getFunc();

    string getCoordStr()
    {
        stringstream ss;
        for (int i = 0; i < ammLayers; i++)
        {
            for (int j = 0; j < ammNeuron[i]; j++)
            {
                if (network[i][j].getUseFunc() == -1 or network[i][j].getInputBranch())
                    continue; // Пропускаем неиспользуемые нейроны и входные нейроны
                double* coef = network[i][j].getWeights();
                
                for (int k = 0; k < network[i][j].getAmountInp() + 1; k++)
                    ss << coef[k] << " ";
                if (network[i][j].getHaveRNN()) {
                    ss << endl << "RNN: ";
                    coef = network[i][j].getWeightsRNN();
                    for (int k = 0; k < network[i][j].getAmountInpRNN(); k++)
                        ss << coef[k] << " ";
                    ss << endl<<"NN: ";
                }
                
            }
        }
        for (int i = 0; i < ammOutputs; i++)
        {
            if (output[i].getUseFunc() == -1 or output[i].getInputBranch())
                continue; // Пропускаем неиспользуемые нейроны и входные нейроны
            double* coef = output[i].getWeights();
            for (int k = 0; k < output[i].getAmountInp() + 1; k++)
                ss << coef[k] << " ";
        }
        return ss.str();
    }

    bool getLastVertice()
    {
        return lastVertice;
    }
    bool getUnar()
    {
        return unarFuncUs;
    }

    void countNodes(int&);
    void recountLayers(int);

    void changeCoef(double*);
    int getNumVertices();
    int getNumFunc()
    {
        return numberFunc;
    }
    double* getValue(double* x);
    int getNumNodes()
    {
        return numNodes;
    }

    int getAmmInputs()
    {
        return ammInputs;
    }

    
    void replaceNode(int , Tree&);
    void changeNode(int , Tree&);

    void trainWithDE(SampleStorage& data , int size , ComputingLimitation& cLimitation);

    void randFunc()
    { // Используется для оператора мутации
        if (lastVertice)
            if (inputBranch)
                numInput = gen() % ammInputs;
            else
                numberFunc = gen() % amFuncActive;
        else if (inputBranch)
            numberFunc = 0; // Случай сложения узлов для входной ветви
        else
            if (unarFuncUs) {
                if (coordRNN_firstL != nullptr) {
                    for (int i = 0; i < 2; i++)
                        delete[] coordRNN_firstL[i];
                    delete[] coordRNN_firstL;
                    coordRNN_firstL = nullptr;
                    amRNN = 0;
                }
            }
            else {
                numberFunc = gen() % 2;
            }
            
    }

    Tree* getLeft()
    {
        if (left == nullptr)
            return nullptr;
        return left;
    }
    Tree* getRight()
    {
        if (right == nullptr)
            return nullptr;
        return right;
    }

    void saveNetworkToFile(string fileName)
    {
        ofstream file(fileName);
        if (!file.is_open())
        {
            cerr << "Error opening file: " << fileName << endl;
            return;
        }

        file << "fitness: " << fitness << endl;
        file << "Number of inputs: " << ammInputs << endl;
        file << "Number of outputs: " << ammOutputs << endl;
        file << "Number of vertices: " << numVertices << endl;
        file << "Number of nodes: " << numNodes << endl;
        file << "EF: " << ef << endl;
        file << "NF: " << nf << endl;
        file << "Max nodes: " << maxNodes << endl;
        file << "Number of layers: " << ammLayers << endl;
        file << "Number of neurons in each layer: ";
        for (int i = 0; i < ammLayers; i++)
            file << ammNeuron[i] << " ";

        file << endl << "FuncActivation:";
        for (int i = 0; i < ammLayers; i++)
        {

            for (int j = 0; j < ammNeuron[i]; j++)
            {
                file << "[ ";
                if (network[i][j].getInputBranch())
                    file << "Input - " << network[i][j].getUseFunc();
                else
                    file << "" << network[i][j].getUseFunc();
                file << " ] ";
            }
            file << endl;
        }

        file << "Coordinates: " << endl;
        file << this->getMatrix() << endl;

        file << "Weights: " << endl;
        for (int i = 0; i < ammLayers; i++)
        {
            for (int j = 0; j < ammNeuron[i]; j++)
            {
                if (network[i][j].getUseFunc() == -1 or network[i][j].getInputBranch())
                    continue; // Пропускаем неиспользуемые нейроны и входные нейроны
                double* coef = network[i][j].getWeights();

                for (int k = 0; k < network[i][j].getAmountInp() + 1; k++)
                    file << coef[k] << " ";
            }
        }
        for (int i = 0; i < ammOutputs; i++)
        {
            if (output[i].getUseFunc() == -1 or output[i].getInputBranch())
                continue; // Пропускаем неиспользуемые нейроны и входные нейроны
            double* coef = output[i].getWeights();
            for (int k = 0; k < output[i].getAmountInp() + 1; k++)
                file << coef[k] << " ";
        }

        file << endl;

        file.close();
    }
    void loadNetworkFromFile(string fileName)
    {
        // Не работает!!! Нужно написать ввод координат в сеть
        ifstream file(fileName);
        if (!file.is_open())
        {
            cerr << "Error opening file: " << fileName << endl;
            return;
        }
        string line;
        while (getline(file , line))
        {
            if (line.find("fitness:") != string::npos)
            {
                fitness = stod(line.substr(line.find(":") + 1));
            }
            else if (line.find("Number of inputs:") != string::npos)
            {
                ammInputs = stoi(line.substr(line.find(":") + 1));
            }
            else if (line.find("Number of outputs:") != string::npos)
            {
                ammOutputs = stoi(line.substr(line.find(":") + 1));
            }
            else if (line.find("Number of vertices:") != string::npos)
            {
                numVertices = stoi(line.substr(line.find(":") + 1));
            }
            else if (line.find("Number of nodes:") != string::npos)
            {
                numNodes = stoi(line.substr(line.find(":") + 1));
            }
            else if (line.find("EF:") != string::npos)
            {
                ef = stod(line.substr(line.find(":") + 1));
            }
            else if (line.find("NF:") != string::npos)
            {
                nf = stod(line.substr(line.find(":") + 1));
            }
            else if (line.find("Max nodes:") != string::npos)
            {
                maxNodes = stoi(line.substr(line.find(":") + 1));
            }
            else if (line.find("Number of layers:") != string::npos)
            {
                ammLayers = stoi(line.substr(line.find(":") + 1));
                if (ammNeuron != nullptr)
                    delete[] ammNeuron;
                ammNeuron = new int[ammLayers];
                if (network != nullptr)
                {
                    for (int i = 0; i < ammLayers; i++)
                        delete[] network[i];
                    delete[] network;
                }
                network = new Neuron * [ammLayers];
                for (int i = 0; i < ammLayers; i++)
                    network[i] = nullptr; // Инициализируем указатели
            }
            else if (line.find("Number of neurons in each layer:") != string::npos)
            {
                stringstream ss(line.substr(line.find(":") + 1));
                for (int i = 0; i < ammLayers; i++)
                    ss >> ammNeuron[i];
            }
            else if (line.find("FuncActivation:") != string::npos)
            {
                for (int i = 0; i < ammLayers; i++)
                {
                    getline(file , line);
                    stringstream ss(line);
                    for (int j = 0; j < ammNeuron[i]; j++)
                    {
                        string funcStr;
                        ss >> funcStr;
                        if (funcStr == "Input")
                        {
                            int useFunc;
                            ss >> useFunc;
                            network[i][j] = Neuron(true , useFunc); // Входной нейрон
                        }
                        else
                        {
                            int useFunc = stoi(funcStr);
                            network[i][j] = Neuron(useFunc);
                        }
                    }
                }
            }
            else if (line.find("Coordinates:") != string::npos)
            {
                getline(file , line);
            }
            else if (line.find("Weights:") != string::npos)
            {
                int cursor = 0;
                for (int i = 0; i < ammLayers; i++)
                {
                    for (int j = 0; j < ammNeuron[i]; j++)
                    {
                        if (network[i][j].getUseFunc() == -1 or network[i][j].getInputBranch())
                        {
                            continue; // Пропускаем неиспользуемые нейроны и входные нейроны
                        }
                        double* coef = new double[network[i][j].getAmountInp() + 1];
                        for (int k = 0; k < network[i][j].getAmountInp() + 1; k++)
                            file >> coef[k];
                        network[i][j].setWeights(coef , cursor);
                        delete[] coef;
                    }
                }
                for (int i = 0; i < ammOutputs; i++)
                {
                    if (output[i].getUseFunc() == -1 or output[i].getInputBranch())
                        continue; // Пропускаем неиспользуемые нейроны и входные нейроны
                    double* coef = new double[output[i].getAmountInp() + 1];
                    for (int k = 0; k < output[i].getAmountInp() + 1; k++)
                        file >> coef[k];
                    output[i].setWeights(coef , cursor);
                    delete[] coef;
                }
            }
        }
        file.close();
        // Инициализация сети нейронов
    }

    Tree operator=(const Tree& copy)
    {

        if (copy.output != nullptr)
        {
            if (output != nullptr)
                delete[] output;
            // Выделяем память под выходные нейроны
            output = new Neuron[copy.ammOutputs];
            for (int i = 0; i < copy.ammOutputs; i++)
                output[i] = copy.output[i];
        }
        else if (output != nullptr)
        {
            delete[] output;
            output = nullptr;
        }

        if (copy.ammNeuron != nullptr)
        {
            if (ammNeuron != nullptr)
                delete[] ammNeuron;

            ammNeuron = new int[copy.ammLayers];
            for (int i = 0; i < copy.ammLayers; i++)
                ammNeuron[i] = copy.ammNeuron[i];
        }
        else if (ammNeuron != nullptr)
        {
            delete[] ammNeuron;
            ammNeuron = nullptr;
        }
        if (copy.network != nullptr)
        {
            if (network != nullptr)
            {
                for (int i = 0; i < ammLayers; i++)
                    delete[] network[i];
                delete[] network;
            }
            network = new Neuron * [copy.ammLayers];
            for (int i = 0; i < copy.ammLayers; i++)
            {
                network[i] = new Neuron[ammNeuron[i]]; // Обращаемся к своему, потому
                // что уже скопировали
                for (int j = 0; j < ammNeuron[i]; j++)
                    network[i][j] = copy.network[i][j];
            }
        }
        else if (network != nullptr)
        {
            for (int i = 0; i < ammLayers; i++)
                delete[] network[i];
            delete[] network;
            network = nullptr;
        }
        if (copy.coordRNN_firstL != nullptr) {
            if (coordRNN_firstL != nullptr) {
                for (int i = 0; i < 2; i++)
                    delete[] coordRNN_firstL[i];
                delete[] coordRNN_firstL;
            }
            amRNN = copy.amRNN;
            coordRNN_firstL = new int* [2];
            for (int i = 0; i < 2; i++) {
                coordRNN_firstL[i] = new int[amRNN];
                for (int j = 0; j < amRNN; j++) {
                    coordRNN_firstL[i][j] = copy.coordRNN_firstL[i][j]; 
                }
            }
        }


        unarFuncUs = copy.unarFuncUs;
        typeTask = copy.typeTask;
        ammOutputs = copy.ammOutputs;
        ammLayers = copy.ammLayers;
        mainNode = copy.mainNode;
        inputBranch = copy.inputBranch;
        size = copy.size;
        numberFunc = copy.numberFunc;
        layerLevel = copy.layerLevel;
        lastVertice = copy.lastVertice;
        numInput = copy.numInput;
        numVertices = copy.numVertices;
        numNodes = copy.numNodes;
        fitness = copy.fitness;
        ammInputs = copy.ammInputs;

        // Выделение памяти чтобы не было кучи взаимосвязанных индивидлв
        if (copy.left != nullptr)
        {
            if (left != nullptr)
                delete left;
            // left = new Tree; старый вариант при котором работало, но была утечка
            //*left = Tree(*(copy.left));
            left = new Tree(*(copy.left));
        }
        else if (left != nullptr)
        {
            delete left;
            left = nullptr;
        }
        if (copy.right != nullptr)
        {
            if (right != nullptr)
                delete right;
            /*right = new Tree; Старый вариант при котором работало, но была утечка
             *right = Tree(*(copy.right));*/
            right = new Tree(*(copy.right));
        }
        else if (right != nullptr)
        {
            delete right;
            right = nullptr;
        }

        return *this;
    }

    ~Tree()
    {
        if (network != nullptr)
        {
            if (!lastVertice)
                for (int i = 0; i < ammLayers; i++)
                    delete[] network[i];
            delete[] network;
            network = nullptr;
        }

        if (output != nullptr)
        {
            delete[] output;
            output = nullptr;
        }
        if (ammNeuron != nullptr)
        {
            delete[] ammNeuron;
            ammNeuron = nullptr;
        }
        if (left != nullptr)
        {
            delete left;
            left = nullptr;
        }
        if (right != nullptr)
        {
            delete right;
            right = nullptr;
        }
        if (coordRNN_firstL != nullptr) {
            for (int i = 0; i < 2; i++)
                delete[] coordRNN_firstL[i];
            delete[] coordRNN_firstL;
            coordRNN_firstL = nullptr;
        }
    }
};
