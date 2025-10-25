#include "Neuron.h"

Neuron::Neuron(const Neuron &copy)
{
    value = copy.value;
    amountInpRNN = copy.amountInpRNN;
    output = copy.output;
    amountInp = copy.amountInp;
    useFunc = copy.useFunc;
    inputBranch = copy.inputBranch;
    haveRNN = copy.haveRNN;
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
    if (copy.inputRNN != nullptr)
    {
        if (inputRNN != nullptr)
            delete[] inputRNN;
        inputRNN = new int *[amountInpRNN];
        for (int i = 0; i < amountInpRNN; i++)
        {
            inputRNN[i] = new int[2];
            inputRNN[i][0] = copy.inputRNN[i][0];
            inputRNN[i][1] = copy.inputRNN[i][1];
        }
    }
    if (copy.weightsRNN != nullptr)
    {
        if (weightsRNN != nullptr)
            delete[] weightsRNN;
        weightsRNN = new double[amountInpRNN];
        for (int i = 0; i < amountInpRNN; i++)
        {
            weightsRNN[i] = copy.weightsRNN[i];
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
}

void Neuron::setWeights(double *coef, int &cursor)
{
    if (useFunc == -1)
    { // Если нейрон не используется, то не нужно заполнять коэффициенты
        weights = nullptr;
        return;
    }
    if (weights == nullptr)
    { // Если уже были коэффициенты, то удаляем их
        weights = new double[amountInp + 1];
    }
    
    if (weightsRNN == nullptr) {
        weightsRNN = new double[amountInpRNN];
    }

    for (int i = 0; i < amountInp + 1; i++)
    {
        weights[i] = coef[i + cursor];
    }
    cursor += amountInp + 1;
    //RNN
    for (int i = 0; i < amountInpRNN; i++){
        weightsRNN[i] = coef[i + cursor];
    }
    cursor += amountInpRNN;
}

void Neuron::connect(int amount, int *xOutputs, int *yOutputs, int x,
                     int y) // Вводить х и у те, относительно коннекчущего узла(координаты нейрона без входов)
{
    if (useFunc == -1)
    {
        throw logic_error("Нейрон не используется, не может быть подключен к другим узлам");
        exit(0);
    }

    if (input == nullptr)
    { // РАссмотрен случай когда справа ТОЧНО не окажется входящих узлов
        input = new int *[amount];
        Neuron::amountInp = amount;
        for (int i = 0; i < amount; i++)
        {
            input[i] = new int[2];
            input[i][0] = xOutputs[i] - x;
            input[i][1] = yOutputs[i] - y;
        }
    }
    else
    {
        cout << endl << "Ошибка коннекта";
        exit(0);
    }
}
void Neuron::addConnectRNN(int xOutputs , int yOutputs , int x , int y)
{
    //x,y координаты нейрона в который поступает связь
    if (useFunc == -1)
    {
        throw logic_error("Нейрон не используется, не может быть подключен к другим узлам");
        exit(0);
    }
    haveRNN = true;
    amountInpRNN++;
    if (inputRNN != nullptr)
    { // РАссмотрен случай когда справа ТОЧНО не окажется входящих узлов
        int** tmp = new int* [amountInpRNN - 1];
        for (int i = 0; i < amountInpRNN - 1; i++) {
            tmp[i] = new int[2];
            tmp[i][0] = inputRNN[i][0];
            tmp[i][1] = inputRNN[i][1];
            delete[] inputRNN[i];
        }
        delete[] inputRNN;
        inputRNN = new int *[amountInpRNN];
        for (int i = 0; i < amountInpRNN - 1; i++)
        {
            inputRNN[i] = new int[2];
            inputRNN[i][0] = tmp[i][0];
            inputRNN[i][1] = tmp[i][1];
            delete[] tmp[i];
        }
        delete[] tmp;
    }
    else {
        inputRNN = new int*[amountInpRNN];
    }

    inputRNN[amountInpRNN - 1] = new int[2];
    inputRNN[amountInpRNN - 1][0] = xOutputs - x;
    inputRNN[amountInpRNN - 1][1] = yOutputs - y;
}

Neuron::~Neuron()
{
    if (inputRNN != nullptr)
    {

        for (int i = 0; i < amountInpRNN; i++)
        {
            delete[] inputRNN[i];
        }

        delete[] inputRNN;
        inputRNN = nullptr;
    }
    if (weightsRNN != nullptr)
    {
        delete[] weightsRNN;
        weightsRNN = nullptr;
    }
    if (input != nullptr)
    {
        if (inputBranch)
        {
        }
        else
        {
            if (amountInp != 0)
            {
                for (int i = 0; i < amountInp; i++)
                {
                    delete[] input[i];
                }
            }
            delete[] input;
        }
    }
    input = nullptr;
    if (weights != nullptr)
    {
        delete[] weights;
    }
    weights = nullptr;
}
