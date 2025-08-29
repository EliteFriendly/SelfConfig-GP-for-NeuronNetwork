#include "Tree.h"
#include <sstream>

void Tree::countNodes(int &ammount)
{

    if (left != nullptr)
        left->countNodes(ammount);
    if (right != nullptr)
        right->countNodes(ammount);
    numNodes = ammount;
    ammount++;
}

void Tree::recountLayers(int level)
{
    layerLevel = level;
    if (left != nullptr)
        left->recountLayers(level + 1);
    if (right != nullptr)
        right->recountLayers(level + 1);
}

/// @brief
/// @param x
/// @param size
void Tree::calcFitness(double **x, int size) // typeTask = "reg" or "class"
{
    // calculated a RMSE
    if (typeTask == "reg")
    {
        double RMSE = 0;
        for (int i = 0; i < size; i++)
        {
            double *res = getValue(x[i]); // Получаем значение нейронной сети для каждого входа
            for (int j = 0; j < ammOutputs; j++)
                RMSE += pow(res[j] - x[i][ammInputs + j], 2); // Считаем MSE
            delete[] res;
        }
        RMSE = sqrt(RMSE / (ammOutputs * size));                  // Считаем RMSE
        fitness = 1 / (1 + ef * RMSE + nf * numNodes / maxNodes); // Считаем фитнес, где ef - коэффициент при RMSE, nf
                                                                  // - коэффициент при количестве узлов, maxNodes -
                                                                  // максимальное количество узлов в дереве
    }
    // calculated a accuracy
    else if (typeTask == "class")
    {

        int correct = 0;
        for (int i = 0; i < size; i++)
        {
            double *res = getValue(x[i]); // Получаем значение нейронной сети для каждого входа
            int maxIndex = 0;


            for (int j = 1; j < ammOutputs; j++)
            { // Ищем индекс максимального значения
                if (res[j] > res[maxIndex])
                    maxIndex = j;
            }

            if ((maxIndex + 1) == x[i][ammInputs])
            { // Если индекс совпадает с номером класса, то
                // увеличиваем счетчик
                correct++;
            }
            delete[] res;
        }
        fitness = double(correct) / (size); // Считаем фитнес, где size - количество элементов в выборке
                                            // cout << fitness << endl;
    }

    if (fitness < 0)
        throw invalid_argument("Fitness is NULL");
}

string Tree::getMatrix()
{

    stringstream ss;

    for (int i = 0; i < ammLayers; i++)
    {
        for (int j = 0; j < ammNeuron[i]; j++)

            ss << "[" << network[i][j].getStrCoord() << "] ";
        ss << "End" << endl;
    }
    ss << "Outputs" << endl;
    for (int i = 0; i < ammOutputs; i++)
        ss << "[" << output[i].getStrCoord() << "] ";

    return ss.str();
}

void Tree::doNeuronNetwork()
{
    doHiddenNeuron(); // Сначала обьединяем в единую сеть все скрытые нейроны
    int amNeuron = 0;
    for (int i = 0; i < ammLayers; i++) // Смотрим сколько вообще нейронов в сети
        amNeuron += ammNeuron[i];

    int *coordXOutput = new int[amNeuron];
    int *coordYOutput = new int[amNeuron];
    int i1 = 0; // Количество нейронов где нет выхода
    for (int i = 0; i < ammLayers; i++)
    { // Пробегаемся по всем потеряшками
        for (int j = 0; j < ammNeuron[i]; j++)
        {
            if (network[i][j].getUseFunc() == -1)
            { // Если нейрон не используется, то просто пропускаем его
                continue;
            }
            if (!network[i][j].getOutput())
            {
                coordXOutput[i1] = i;
                coordYOutput[i1] = j;
                i1++;
                network[i][j].haveOutput();
            }
        }
    }

    if (output != nullptr)
    { // Если выходные нейроны уже были созданы, то удаляем
      // их
        delete[] output;
    }

    output = new Neuron[ammOutputs];

    for (int i = 0; i < ammOutputs; i++)
    {                          // Теперь все выходы конектим к выходам
        output[i] = Neuron(0); // Создаем выходной нейрон, который будет
                               // использовать нулевую функцию активации
        output[i].connect(i1, coordXOutput, coordYOutput, ammLayers, i);
    }

    delete[] coordXOutput;
    delete[] coordYOutput;
    int nodes = 0, lvl = 0;
    // Считаем количество узлов в дереве
    countNodes(nodes);
    // Считаем количество слоев в дереве
    recountLayers(lvl);
}

Tree::Tree(int d, int ammInput, bool inputBranch) : inputBranch(inputBranch), ammInputs(ammInput)
{

    // Случай если дошли до самого конца
    if (d == 0)
    {
        lastVertice = true;
        ammNeuron = new int[1];
        ammNeuron[0] = 1;
        ammLayers++;
        if (inputBranch)
        {
            numInput = rand() % ammInputs;
            network = new Neuron *[1];
            network[0] = new Neuron(true, numInput); //?????????
            return;
        }
        else
        {
            numberFunc = rand() % amFuncActive;
            network = new Neuron *[1];
            network[0] = new Neuron(numberFunc); //?????????
            return;
        }
    };
    if (inputBranch)
    {
        numberFunc = 0;
        left = new Tree(d - 1, ammInput, true);
        right = new Tree(d - 1, ammInput, true);
    }
    else
    {
        numberFunc = rand() % 2;
        left = new Tree(d - 1, ammInput, false);
        right = new Tree(d - 1, ammInput, false);
    };
}

Tree::Tree(int d, int ammInputs, int ammOutputs, string typeTask)
{
    Tree::ammInputs = ammInputs;
    Tree::ammOutputs = ammOutputs;
    Tree::typeTask = typeTask;
    mainNode = true;
    // Так это основной узел, у него все определено левый и правый уже понятны
    numberFunc = 1;
    left = new Tree(d - 1, ammInputs, true);
    right = new Tree(d - 1, ammInputs, false);
}

string Tree::getFunc()
{
    stringstream ss;
    if (lastVertice)
    {
        if (inputBranch)
            ss << 'I' << numInput;
        else
            ss << 'N' << numberFunc;
    }
    else
    {
        if (left != nullptr)
        {
            ss << '(';
            ss << left->getFunc();
        }

        if (!lastVertice)
            ss << strBinaryFunc[numberFunc];

        if (right != nullptr)
        {
            ss << right->getFunc();
            ss << ')';
        }
    }

    return ss.str();
}

void Tree::doHiddenNeuron()
{
    if (lastVertice == true)
        return;

    // Освобождение памяти в случае двойного вызово функции
    if (network != nullptr)
    {
        for (int i = 0; i < ammLayers; i++)
            delete[] network[i];
        delete[] network;
        network = nullptr;
    }
    if (ammNeuron != nullptr)
    {
        delete[] ammNeuron;
        ammNeuron = nullptr;
    }

    left->doHiddenNeuron();
    right->doHiddenNeuron();
    if (numberFunc == 0)
    { // Случай сложения узлов
        ammLayers = max(left->ammLayers,
                        right->ammLayers); // Ищем максимальное количество слоев
        int minLayers = min(left->ammLayers, right->ammLayers);
        bool leftMax = false; // Ищем тот самый узел где больше всего слоев
        if (ammLayers == left->ammLayers)
            leftMax = true;

        int L_or_RmaxNeuron = 0, Lmax = 0,
            Rmax = 0; // L_or_RmaxNeuron является максимальным количеством нейронов
                      // в бОльшей сети
        for (int i = 0; i < ammLayers; i++)
        {
            // Берем максимальное количество нейронов в каждом слое, чтобы потом
            // создать новый массив
            if (i < left->ammLayers)
                Lmax = max(left->ammNeuron[i], Lmax);
            if (i < right->ammLayers)
                Rmax = max(right->ammNeuron[i], Rmax);
            // Если это не последний слой, то берем максимальное количество нейронов в
            // каждом слое
            if (leftMax)
                L_or_RmaxNeuron = max(left->ammNeuron[i], L_or_RmaxNeuron);
            else
                L_or_RmaxNeuron = max(right->ammNeuron[i], L_or_RmaxNeuron);
        }
        // Считаем сколько нейронов будет в каждом слое,
        // накладывая с сохранением струтктуры
        int maxAmNeuron = Lmax + Rmax;

        // for (int i = 0; i < minLayers; i++) {//Считаем сколько нейорнов будет в
        // каждом слое 	ammNeuron[i] = maxAmNeuron;
        // }
        ammNeuron = new int[ammLayers];
        for (int i = 0; i < ammLayers; i++) // Остаток, что был в большем
            ammNeuron[i] = maxAmNeuron;
        network = new Neuron *[ammLayers];
        for (int i = 0; i < ammLayers; i++)
        { // Начинаем копировать нейроны
            network[i] = new Neuron[ammNeuron[i]];
            if (i < minLayers)
            { // Если происходит наслоение
                for (int j = 0; j < ammNeuron[i]; j++)
                {
                    if (leftMax)
                    {
                        if (j < left->ammNeuron[i])
                        {
                            network[i][j] = left->network[i][j];
                        }
                        else if (j < L_or_RmaxNeuron)
                        {
                            network[i][j] = Neuron(); // Если нейрон не заполнен, то он просто пустышка
                        }
                        else if (j >= L_or_RmaxNeuron and j < L_or_RmaxNeuron + right->ammNeuron[i])
                        {
                            network[i][j] = right->network[i][j - L_or_RmaxNeuron];
                        }
                        else
                        {
                            network[i][j] = Neuron();
                        }
                    }
                    else if (j < right->ammNeuron[i])
                    {
                        network[i][j] = right->network[i][j];
                    }
                    else if (j < L_or_RmaxNeuron)
                    {
                        network[i][j] = Neuron();
                    }
                    else if (j >= L_or_RmaxNeuron and j < L_or_RmaxNeuron + left->ammNeuron[i])
                    {
                        network[i][j] = left->network[i][j - L_or_RmaxNeuron];
                    }
                    else
                    {
                        network[i][j] = Neuron();
                    }
                }
            }
            else
            { // Случай когда рассматриваем остаток
                for (int j = 0; j < ammNeuron[i]; j++)
                {
                    if (leftMax)
                        if (j < left->ammNeuron[i])
                            network[i][j] = left->network[i][j];
                        else
                            network[i][j] = Neuron();
                    else if (j < right->ammNeuron[i])
                        network[i][j] = right->network[i][j];
                    else
                        network[i][j] = Neuron();
                }
            }
        }
    }

    if (numberFunc == 1)
    {                     // Случай если обьединение
        int ammLeftN = 0; // Количество нейронов слева
        ammLayers = left->ammLayers + right->ammLayers;
        ammNeuron = new int[ammLayers];
        for (int i = 0; i < left->ammLayers; i++)
        {
            ammNeuron[i] = left->ammNeuron[i];
            ammLeftN += ammNeuron[i];
        }
        for (int i = left->ammLayers; i < ammLayers; i++)
            ammNeuron[i] = right->ammNeuron[i - left->ammLayers];
        network = new Neuron *[ammLayers];
        int *coordXOutput = new int[ammLeftN];
        int *coordYOutput = new int[ammLeftN];
        int i1 = 0; // Количество нейронов где нет выхода
        for (int i = 0; i < ammLayers; i++)
        {
            network[i] = new Neuron[ammNeuron[i]];
            if (i < left->ammLayers)
            {
                for (int j = 0; j < ammNeuron[i]; j++)
                {

                    network[i][j] = left->network[i][j];

                    if (left->network[i][j].getUseFunc() == -1)
                        continue;
                    if (!network[i][j].getOutput())
                    {
                        coordXOutput[i1] = i;
                        coordYOutput[i1] = j;
                        i1++;
                        network[i][j].haveOutput();
                    }
                }
            }
            else
            {
                for (int j = 0; j < ammNeuron[i]; j++)
                {

                    network[i][j] = right->network[i - left->ammLayers][j];

                    if (right->network[i - left->ammLayers][j].getUseFunc() == -1)
                        continue;
                    if (!network[i][j].getInput())
                        network[i][j].connect(i1, coordXOutput, coordYOutput, i, j);
                }
            }
        }
        delete[] coordXOutput;
        delete[] coordYOutput;
    }
}

void Tree::changeCoef(double *coef)
{
    // Тут заполняется ТОЛЬКО нейронная сеть
    int cursor = 0; // Это будет динамически меняться в каждом нейроне
    for (int i = 1; i < ammLayers; i++)
        for (int j = 0; j < ammNeuron[i]; j++)
            network[i][j].setCoefficients(coef, cursor);
    // Теперь заполняем выходные значения, или те что выходные нейроны
    for (int i = 0; i < ammOutputs; i++)
        output[i].setCoefficients(coef, cursor);
}

int Tree::getNumVertices()
{

    if (lastVertice)
        return numVertices;

    if (left != nullptr and right != nullptr)
    {
        numVertices = left->getNumVertices() + right->getNumVertices();
    }
    else
    {
        if (left != nullptr)
            numVertices = left->getNumVertices();
        if (right != nullptr)
            numVertices = right->getNumVertices();
    }

    return numVertices;
}

double *Tree::getValue(double *x)
{
    // Создаем массив для хранения значений
    double **res = new double *[ammLayers];
    for (int i = 0; i < ammLayers; i++)
        res[i] = new double[ammNeuron[i]];

    for (int i = 0; i < ammLayers; i++)
    {
        for (int j = 0; j < ammNeuron[i]; j++)
        {
            if (network[i][j].getInputBranch())
            {                                              // В случае если это входной нейрон
                res[i][j] = x[network[i][j].getUseFunc()]; // Просто берем значение из
                                                           // входного массива
                continue;
            }
            if (network[i][j].getUseFunc() == -1)
            {
                // Если нейрон не используется, то просто заполняем его нулем
                res[i][j] = 0;
                continue;
            }
            // В случае если это обычный нейрон, то заполняем его входные значения
            double *input = new double[network[i][j].getAmountInp()]; // Заполняется элементами,
                                                                      // которые будут подаваться
                                                                      // на вход в нейрон
            for (int w = 0; w < network[i][j].getAmountInp(); w++)
            {
                input[w] = res[i + network[i][j].getCoord()[w][0]][j + network[i][j].getCoord()[w][1]];
            }
            res[i][j] = network[i][j].getValue(funcActivation[network[i][j].getUseFunc()],
                                               input); // Динамически заполняем матрицу полученными значениями
            delete[] input;
        }
    }

    // Теперь заполняем выходные значения, или те что выходые нейроны
    double *outputRes = new double[ammOutputs];
    // По принципу тут все тоже самое, что и в случае с обычными нейронами
    for (int i = 0; i < ammOutputs; i++)
    {
        double *input = new double[output[i].getAmountInp()];
        for (int w = 0; w < output[i].getAmountInp(); w++)
        {
            input[w] = res[ammLayers + output[i].getCoord()[w][0]][i + output[i].getCoord()[w][1]];
        }
        outputRes[i] = output[i].getValue(funcActivation[output[i].getUseFunc()], input);
        delete[] input;
    }
    for (int i = 0; i < ammLayers; i++)
        delete[] res[i];

    delete[] res;

    return outputRes; // Возвращаем массив значений, в зависимости от количества
                      // выходов
}

void Tree::replaceNode(int search, Tree &newNode) // Замена выбранного узла
{
    if (numNodes == search)
    { // Если мы дошли до узла под каким то номером
        *this = newNode;
        return;
    }
    // Поиск по другим узлам если не нашли подходящего номера
    if (left != nullptr and search <= left->getNumNodes())
        left->replaceNode(search, newNode);
    if (right != nullptr and search <= right->getNumNodes())
        right->replaceNode(search, newNode);
}

void Tree::changeNode(int search,
                      Tree &newNode) // Отличие от replace в том, что не меняются остальные узлы
{
    if (numNodes == search)
    { // Если мы дошли до узла под каким то номером
        if (newNode.getLastVertice())
        {
            //*this = newNode;
            if (newNode.getLastVertice() == lastVertice)
            {
                *this = newNode;
                return;
            }
            return;
        }
        if (lastVertice)
        {
            int tnumnodes = numNodes;
            *this = newNode;
            numNodes = tnumnodes;
        }
        else
        {

            numberFunc = newNode.getNumFunc();
            if (left != nullptr)
                delete left;
            left = new Tree;
            // left->operator=(*copy.left);
            *left = Tree(*(newNode.left));
            left->numNodes = -1; // Сделано для того, чтобы не было изменений в этой ветви
        }
        return;
    }

    if (left != nullptr and search <= left->getNumNodes())
        left->changeNode(search, newNode);
    if (right != nullptr and search <= right->getNumNodes())
        right->changeNode(search, newNode);
}

void Tree::trainWithDE(double **x, int size, ComputingLimitation &cLimitation)
{

    // get amCoefficents from network
    int amCoefficients = 0;
    for (int i = 1; i < ammLayers; i++)
        for (int j = 0; j < ammNeuron[i]; j++)
        {
            if (network[i][j].getAmountInp() != 0)
                amCoefficients += network[i][j].getAmountInp() + 1;
        }
    for (int i = 0; i < ammOutputs; i++)
        amCoefficients += output[i].getAmountInp() + 1;

    if (amCoefficients == 0)
    {
        cout << "Ошибка в количестве коэффициентов, возможно не хватает нейронов в "
                "сети";
        exit(0);
    }
    function<double(double *)> func = [&](double *input) {
        if (cLimitation.useComputing()){
        changeCoef(input);
        calcFitness(x, size);
        return fitness;}
        else{
            changeCoef(input);
            return 0.0;
        }
    };

    double *limits = new double[amCoefficients * 2];

    for (int i = 0; i < amCoefficients * 2; i++)
        if (i % 2)
            limits[i] = 30;
        else
            limits[i] = -30;

    DiffEvolution DE(func, limits, amCoefficients, "targetToBest1", "max");
    DE.startSearch(0.01, 0.5, 0.5, 50, 50, cLimitation);
    int i = 0;
    double *coef = DE.getBestCoordinates();
    changeCoef(coef);
    fitness = DE.getBestFitness();
    // calcFitness(x, size, true);
    delete[] limits;
}
