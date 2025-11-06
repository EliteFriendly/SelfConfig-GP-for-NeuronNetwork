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
        long double RMSE = 0;
        for (int i = 0; i < size; i++)
        {
            double *res = getValue(x[i]); // Получаем значение нейронной сети для каждого входа
            for (int j = 0; j < ammOutputs; j++) {
                RMSE += pow(res[j] - x[i][ammInputs + j] , 2); // Считаем MSE
                
            }

            delete[] res;
            if (std::isinf(RMSE)) {
                fitness = 0;
                
                return;
            }
        }
        if (std::isnan(RMSE)) {
                fitness = 0;
                return;
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
            numInput = gen() % ammInputs;
            network = new Neuron *[1];
            network[0] = new Neuron(true, numInput); //?????????
            return;
        }
        else
        {
            numberFunc = gen() % amFuncActive;
            network = new Neuron *[1];
            network[0] = new Neuron(numberFunc); //????????? Вроде работает
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
        numberFunc = gen() % 3;
        if (numberFunc == 2)
        {
            unarFuncUs = true;
            right = new Tree(d - 1, ammInput, false);
        }
        else{
            unarFuncUs = false;
            right = new Tree(d - 1, ammInput, false);
            left = new Tree(d - 1, ammInput, false);
        }
        
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
        if (unarFuncUs){
            ss << strUnaryFunc[numberFunc - amUnaryFunc - 1];//Потому что в этой переменной хранятся все функции
            ss << '(';
            ss << right->getFunc();
            ss << ')';
        }
        else
        {
            if (left != nullptr)
            {
                ss << '(';
                ss << left->getFunc();
            }
            if (!lastVertice)
                ss << strBinaryFunc[numberFunc];//Если не унарная, то <2 а значит входит в границы

            if (right != nullptr)
            {
                ss << right->getFunc();
                ss << ')';
            }
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
    if (left != nullptr) {
        left->doHiddenNeuron();
    }
    right->doHiddenNeuron();
    if (numberFunc == 0)
    { // Случай сложения узлов
        //cout<<"add"<<endl;
        ammLayers = max(left->ammLayers ,
            right->ammLayers); // Ищем максимальное количество слоев
        int minLayers = min(left->ammLayers , right->ammLayers);
        bool leftMax = false; // Ищем тот самый узел где больше всего слоев
        if (ammLayers == left->ammLayers)
            leftMax = true;

        int L_or_RmaxNeuron = 0 , Lmax = 0 ,
            Rmax = 0; // L_or_RmaxNeuron является максимальным количеством нейронов
        // в бОльшей сети
        for (int i = 0; i < ammLayers; i++)
        {
            // Берем максимальное количество нейронов в каждом слое, чтобы потом
            // создать новый массив
            if (i < left->ammLayers)
                Lmax = max(left->ammNeuron[i] , Lmax);
            if (i < right->ammLayers)
                Rmax = max(right->ammNeuron[i] , Rmax);
            // Если это не последний слой, то берем максимальное количество нейронов в
            // каждом слое
            if (leftMax)
                L_or_RmaxNeuron = max(left->ammNeuron[i] , L_or_RmaxNeuron);
            else
                L_or_RmaxNeuron = max(right->ammNeuron[i] , L_or_RmaxNeuron);
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
        network = new Neuron * [ammLayers];
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
        //cout<<"con"<<endl;
        ammLayers = left->ammLayers + right->ammLayers;
        ammNeuron = new int[ammLayers];
        for (int i = 0; i < left->ammLayers; i++)
        {
            ammNeuron[i] = left->ammNeuron[i];
            ammLeftN += ammNeuron[i];
        }
        for (int i = left->ammLayers; i < ammLayers; i++)
            ammNeuron[i] = right->ammNeuron[i - left->ammLayers];
        network = new Neuron * [ammLayers];
        int* coordXOutput = new int[ammLeftN];
        int* coordYOutput = new int[ammLeftN];
        int i1 = 0; // Количество нейронов где нет выхода
        //cout << "con1" << endl;
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
                        network[i][j].connect(i1 , coordXOutput , coordYOutput , i , j);
                }
            }
        }
        //cout << "con end" << endl;
        delete[] coordXOutput;
        delete[] coordYOutput;
    }
    if (numberFunc == 2)
    {//Случай реккурсивной связи (унарная функция только с правой веткой работает)
        //cout<<"start"<<endl;
        int maxAmmNeuron = 0;
        ammLayers = right->ammLayers;
        ammNeuron = new int[ammLayers];
        for (int i = 0; i < ammLayers; i++)
        {
            ammNeuron[i] = right->ammNeuron[i];
            maxAmmNeuron = max(maxAmmNeuron , ammNeuron[i]);
        }
        if (left!= nullptr) {
            throw logic_error("Left not nullptr");
        }
        network = new Neuron * [ammLayers];
        //For neurons without output
        int* coordXOutput = new int[maxAmmNeuron];
        int* coordYOutput = new int[maxAmmNeuron];
        //For neurons without input
        int* coordXInput = new int[maxAmmNeuron];
        int* coordYInput = new int[maxAmmNeuron];




        int iInput = 0;//количество нейронов у которых нет входа
        int iOutput = 0;//количество нейронов у которых нет выхода
        //cout<<"start2"<<endl;
        for (int i = 0; i < ammLayers; i++)
        {
            network[i] = new Neuron[ammNeuron[i]];
            for (int j = 0; j < ammNeuron[i]; j++)
            {

                network[i][j] = right->network[i][j];

                if (right->network[i][j].getUseFunc() == -1)
                    continue;
                if (!network[i][j].getOutput())
                {
                    if (iOutput > maxAmmNeuron) {
                        throw logic_error("Too many neurons without output");
                        //Если это произойдет - значит будут проблемы с поиском этих нейронов(их больше чем ты думал)
                    }
                    coordXOutput[iOutput] = i;
                    coordYOutput[iOutput] = j;
                    iOutput++;

                }
                if (!network[i][j].getInput()) {
                    if (iInput > maxAmmNeuron) {
                        throw logic_error("Too many neurons without input");
                        //Если это произойдет - значит будут проблемы с поиском этих нейронов(их больше чем ты думал)
                    }
                    coordXInput[iInput] = i;
                    coordYInput[iInput] = j;
                    iInput++;
                }
            }
        }

        bool* flag;//Есть ли данная пара(старый и новый input) в новой нейронной сети
        if (coordRNN_firstL != nullptr) {

            if (amRNN != iOutput) {//Делаем надбавку/убавку
                //cout<<"add/remove"<<endl;
                int** tmp = new int* [2];
                tmp[0] = new int[amRNN];
                tmp[1] = new int[amRNN];
                for (int i = 0;i < amRNN;i++) {
                    tmp[0][i] = coordRNN_firstL[0][i];
                    tmp[1][i] = coordRNN_firstL[1][i];

                }
                delete[] coordRNN_firstL[0];
                delete[] coordRNN_firstL[1];
                delete[] coordRNN_firstL;
                coordRNN_firstL = new int* [2];
                coordRNN_firstL[0] = new int[iOutput];
                coordRNN_firstL[1] = new int[iOutput];
                for (int i = 0;i < iOutput;i++) {
                    coordRNN_firstL[0][i] = tmp[0][i];
                    coordRNN_firstL[1][i] = tmp[1][i];
                }
                delete[] tmp[0];
                delete[] tmp[1];
                delete[] tmp;
                amRNN = iOutput;
            }
            //cout<<"check"<<endl;
            flag = new bool[iOutput];//Найдется ли пара в новой сети для наследования от старой
            for (int i = 0;i < amRNN;i++) {
                flag[i] = false;
                if (coordRNN_firstL[0][i] != coordXInput[i] or coordRNN_firstL[1][i] != coordYInput[i]) {
                    for (int j = i;j < iInput;j++) {
                        if (coordRNN_firstL[0][i] == coordXInput[j] and coordRNN_firstL[1][i] == coordYInput[j]) {
                            flag[i] = true;

                            break;
                        }
                    }
                }

            }
            //cout<<"end check"<<endl;


        }
        else {
            amRNN = iOutput;
            //cout<<"RNN: "<<amRNN<<endl;
            flag = new bool[amRNN];
            coordRNN_firstL = new int* [2];
            coordRNN_firstL[0] = new int[amRNN];
            coordRNN_firstL[1] = new int[amRNN];
            
            for (int i = 0; i < iOutput; i++) {
                flag[i] = false;
            }
            
        }
        int random;
        for (int i = 0; i < iOutput; i++)
        {
            if (flag[i] == false) {
                random = gen() % iInput;
                coordRNN_firstL[0][i] = coordXInput[random];
                coordRNN_firstL[1][i] = coordYInput[random];
                //cout<< coordRNN_firstL[0][i] << " " << coordRNN_firstL[1][i] << endl;
            }
            network[coordRNN_firstL[0][i]][coordRNN_firstL[1][i]].addConnectRNN(coordXOutput[i], coordYOutput[i], coordRNN_firstL[0][i], coordRNN_firstL[1][i]);
        }
        
        delete[] coordXOutput;
        delete[] coordYOutput;
        delete[] coordXInput;
        delete[] coordYInput;
        delete[] flag;
        //cout<<"end delete"<<endl;
    }

}

void Tree::changeCoef(double *coef)
{
    // Тут заполняется ТОЛЬКО нейронная сеть
    int cursor = 0; // Это будет динамически меняться в каждом нейроне
    for (int i = 1; i < ammLayers; i++)
        for (int j = 0; j < ammNeuron[i]; j++)
            network[i][j].setWeights(coef, cursor);
    // Теперь заполняем выходные значения, или те что выходные нейроны
    for (int i = 0; i < ammOutputs; i++)
        output[i].setWeights(coef, cursor);
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

double* Tree::getValue(double* x)
{
    // Создаем массив для хранения значений
    double** res = new double* [ammLayers];
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
            double* input = new double[network[i][j].getAmountInp() + network[i][j].getAmountInpRNN()]; // Заполняется элементами,
            // которые будут подаваться
            // на вход в нейрон
            for (int w = 0; w < network[i][j].getAmountInp(); w++)
            {
                input[w] = res[i + network[i][j].getCoord()[w][0]][j + network[i][j].getCoord()[w][1]];
            }
            //Случай для RNN
            for (int w = 0; w < network[i][j].getAmountInpRNN(); w++) {
                input[w + network[i][j].getAmountInp()] = network[i + network[i][j].getcoordRNNInputs()[w][0]][j + network[i][j].getcoordRNNInputs()[w][1]].getValueRNN();
            }
            res[i][j] = network[i][j].getValue(funcActivation[network[i][j].getUseFunc()] ,
                input); // Динамически заполняем матрицу полученными значениями
            delete[] input;
        }
    }

    // Теперь заполняем выходные значения, или те что выходые нейроны
    double* outputRes = new double[ammOutputs];
    // По принципу тут все тоже самое, что и в случае с обычными нейронами
    for (int i = 0; i < ammOutputs; i++)
    {
        double* input = new double[output[i].getAmountInp()];
        for (int w = 0; w < output[i].getAmountInp(); w++)
        {
            input[w] = res[ammLayers + output[i].getCoord()[w][0]][i + output[i].getCoord()[w][1]];
        }
        outputRes[i] = output[i].getValue(funcActivation[output[i].getUseFunc()] , input);
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

            if (newNode.getUnar()) {
				unarFuncUs = true;
				numberFunc = newNode.getNumFunc();
				delete left;
				left = nullptr;
			}
            else {
                unarFuncUs = false;
                numberFunc = newNode.getNumFunc();
                if (left == nullptr) {
                    left = new Tree;
                    //left->operator=(*copy.left);
                    *left = Tree(*(newNode.left));
                    left->numNodes = -1; // Сделано для того, чтобы не было изменений в этой ветви
                }
                

            }
           
        }
        return;
    }

    if (left != nullptr and search <= left->getNumNodes())
        left->changeNode(search, newNode);
    if (right != nullptr and search <= right->getNumNodes())
        right->changeNode(search, newNode);
}

void Tree::trainWithDE(SampleStorage &data, int size, ComputingLimitation &cLimitation)
{
    // get amCoefficents from network
    int amWeights = 0;
    for (int i = 1; i < ammLayers; i++)
        for (int j = 0; j < ammNeuron[i]; j++)
        {
            if (network[i][j].getAmountInp() != 0 or network[i][j].getAmountInpRNN() != 0)
                amWeights += network[i][j].getAmountInp() + 1 + network[i][j].getAmountInpRNN();
        }
    for (int i = 0; i < ammOutputs; i++)
        amWeights += output[i].getAmountInp() + 1;

    if (amWeights == 0)
    {
        cout << "Ошибка в количестве коэффициентов, возможно не хватает нейронов в "
                "сети";
        exit(0);
    }
    function<double(double *)> func = [&](double *input) {
        if (cLimitation.useComputing())
        {
            changeCoef(input);
            calcFitness(data.getTrainData(), size);
            return fitness;
        }
        else
        {
            changeCoef(input);
            return 0.0;
        }
    };
    function<double(double *)> overFittingFunc = [&](double *input) {
        if (cLimitation.useComputing())
        {
            changeCoef(input);
            calcFitness(data.getTestData(), data.getTestSize());
            return fitness;
        }
        else
        {
            changeCoef(input);
            return 0.0;
        }
    };

    double *limits = new double[amWeights * 2];

    for (int i = 0; i < amWeights * 2; i++)
        if (i % 2)
            limits[i] = 30;
        else
            limits[i] = -30;

    DiffEvolution DE(func, limits, amWeights, "targetToBest1", "max");
    DE.setOverFittingFunc(overFittingFunc);
    DE.startSearch(0.01, 0.5, 0.5, numInd, numGen, cLimitation);
    int i = 0;
    double *coef = DE.getBestCoordinates();
    changeCoef(coef);
    fitness = DE.getBestFitness();
    delete[] limits;
}
