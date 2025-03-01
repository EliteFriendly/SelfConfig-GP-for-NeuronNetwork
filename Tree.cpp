#include "Tree.h"
#include <sstream>


void Tree::countNodes(int& ammount)
{
	
	if (left != nullptr) {
		left->countNodes(ammount);
	}
	if (right != nullptr) {
		right->countNodes(ammount);
	}
	numNodes = ammount;
	ammount++;
}

void Tree::recountLayers(int level)
{
	layerLevel = level;
	if (left != nullptr) {
		left->recountLayers(level+1);
	}
	if (right != nullptr) {
		right->recountLayers(level+1);
	}
}


void Tree::calcFitness(double** x, int size,double K1)
{
	double error = 0;

	fitness = 0;//???????
	if (fitness == 0)
		fitness += 0.00001;
	if (fitness == NULL) {
		cout << "Фитнес равен NAN";
		exit(0);
	}
}

string Tree::getMatrix()
{

	stringstream ss;
	
	for (int i = 0; i < ammLayers; i++) {
		for (int j = 0; j < ammNeuron[i]; j++) {
			ss << "[" << network[i][j].getStrCoord() << "] ";
		}
		ss << "End" << endl;
	}


	return ss.str();
}






Tree::Tree(int d, int ammInput, bool inputBranch) :inputBranch(inputBranch), ammInputs(ammInput) {

	//Случай если дошли до самого конца
	if (d == 0) {
		lastVertice = true;
		ammNeuron = new int[1];
		ammNeuron[0] = 1;
		ammLayers++;
		if (inputBranch) {
			numInput = rand() % ammInputs;
			network = new Neuron * [1];
			network[0] = new Neuron(true, numInput);//?????????
			return;
		}
		else {
			numberFunc = rand() % amFuncActive;
			network = new Neuron * [1];
			network[0] = new Neuron(numberFunc);//?????????
			return;
		}
	};
	if (inputBranch) {
		numberFunc = 0;
		left = new Tree(d - 1, ammInput, true);
		right = new Tree(d - 1, ammInput, true);
	}
	else {
		numberFunc = rand() % 2;
		left = new Tree(d - 1, ammInput, false);
		right = new Tree(d - 1, ammInput, false);
	};
};



Tree::Tree(int d, int numInputs, int numOuputs)
{
	ammInputs = numInputs;
	ammOutputs = numOuputs;
	mainNode = true;
	//Так это основной узел, у него все определено левый и правый уже понятны
	numberFunc = 1;
	left = new Tree(d - 1, numInputs, true);
	right = new Tree(d - 1, numInputs, false);
}


string Tree::getFunc()
{
	stringstream ss;
	if (lastVertice) {
		if (inputBranch) {
			ss << 'I'<< numInput;
		}
		else {
			ss <<'N'<< numberFunc;
		}

	}
	else {
		if (left != nullptr) {
			ss << '(';
			ss << left->getFunc();
		}
		
		if (!lastVertice) {
			ss << strBinaryFunc[numberFunc];
		}
		
		if (right != nullptr) {
			ss << right->getFunc();
			ss << ')';
		}
	}
	

	return ss.str();
}

void Tree::doNeuronNetwork()
{
	if (lastVertice)
		return;
	left->doNeuronNetwork();
	right->doNeuronNetwork();
	if (numberFunc == 0) {//Случай сложения узлов
		ammLayers = max(left->ammLayers, right->ammLayers);//Ищем максимальное количество слоев
		int minLayers = min(left->ammLayers, right->ammLayers);
		bool leftMax = false;//Ищем тот самый узел где больше всего слоев
		if (ammLayers == left->ammLayers)
			leftMax = true;
		ammNeuron = new int[ammLayers];
		for (int i = 0; i < minLayers; i++) {//Считаем сколько нейорнов будет в каждом слое
			ammNeuron[i] = left->ammNeuron[i] + right->ammNeuron[i];
		}
		for (int i = minLayers; i < ammLayers; i++) {//Остаток, что был в большем
			ammNeuron[i] = leftMax ? left->ammNeuron[i] : right->ammNeuron[i];
		}
		network = new Neuron * [ammLayers];
		for (int i = 0; i < ammLayers; i++) {//Начинаем копировать нейроны
			network[i] = new Neuron [ammNeuron[i]];
			if (i < minLayers) {//Если происходит наслоение
				for (int j = 0; j < ammNeuron[i]; j++) {
					if (leftMax) {
						if (j < left->ammNeuron[i])
							network[i][j] = left->network[i][j];
						else
							network[i][j] = right->network[i][j - left->ammNeuron[i]];
					}
					else {
						if (j < right->ammNeuron[i])
							network[i][j] = right->network[i][j];
						else
							network[i][j] = left->network[i][j - right->ammNeuron[i]];
					}
				}
			}
			else {//Случай когда рассматриваем остаток
				for (int j = 0; j < ammNeuron[i]; j++) {
					if (leftMax) {
						network[i][j] = left->network[i][j];
					}
					else {
						network[i][j] = right->network[i][j];
					}
				}
			}
		}
	}


	if (numberFunc == 1) {//Случай если обьединение
		int ammLeftN = 0;//Количество нейронов слева
		ammLayers = left->ammLayers + right->ammLayers;
		ammNeuron = new int[ammLayers];
		for (int i = 0; i < left->ammLayers; i++) {
			ammNeuron[i] = left->ammNeuron[i];
			ammLeftN += ammNeuron[i];
		}
		for (int i = left->ammLayers; i < ammLayers; i++) {
			ammNeuron[i] = right->ammNeuron[i - left->ammLayers];
		}
		network = new Neuron * [ammLayers];
		int* coordXOutput = new int[ammLeftN];
		int* coordYOutput = new int[ammLeftN];
		int i1 = 0;//Количество нейронов где нет выхода
		for (int i = 0; i < ammLayers; i++) {
			network[i] = new Neuron[ammNeuron[i]];
			if (i < left->ammLayers) {
				for (int j = 0; j < ammNeuron[i]; j++) {
					network[i][j] = left->network[i][j];
					if (!network[i][j].getOutput()) {
						coordXOutput[i1] = i;
						coordYOutput[i1] = j;
						i1++;
						network[i][j].haveOutput();
					}
				}
			}
			else {
				for (int j = 0; j < ammNeuron[i]; j++) {
					network[i][j] = right->network[i-left->ammLayers][j];
					if (!network[i][j].getInput()) {
						network[i][j].connect(i1, coordXOutput, coordYOutput, i, j);
					}
				}
			}
		}
		
	}
}

void Tree::changeCoef(double *in,int &z)
{
	//Заполнение будет происходить слева направо
	if (left != nullptr) {//Идем сначала по левой стороне до конца
		left->changeCoef(in, z);
	}
	if (right != nullptr) {//Если нет ничего слева
		right->changeCoef(in, z);
	}
	if (lastVertice and numVertices == 1) {
		//coef = in[z];//Замена коэффициентов в случае если все ок
		z++;//Работа с памятью!!!
	}
}

double Tree::getNumVertices()
{

	if (lastVertice) {
		return numVertices;
	}

	if (left != nullptr and right != nullptr) {
		numVertices = left->getNumVertices() + right->getNumVertices();
	}
	else {
		if (left != nullptr) {
			numVertices = left->getNumVertices();
		}
		if (right != nullptr) {
			numVertices = right->getNumVertices();
		}
	}

	return numVertices;
}

double* Tree::getValue(double* x)
{
	double** res = new double*[ammLayers];
	for (int i = 0; i < ammLayers; i++) {
		res[i] = new double[ammNeuron[i]];
	}

	for(int i = 0; i < ammLayers; i++){
		for (int j = 0; j < ammNeuron[i]; j++) {
			if (network[i][j].getInputBranch()) {
				res[i][j] = x[network[i][j].getUseFunc()];
				continue;
			}
			for (int w = 0; w < network[i][j].getAmountInp(); w++) {
				
			}
		}
	}


	//if (right != nullptr and left ==nullptr) {//Если справа что то есть то это точно унарная функци

	//	return unarFunc[numberFunc](right->getValue(x));
	//	
	//}
	//if (lastVertice) {//Если дошли до вершины
	//	if (numVertices==1) {
	//		return coef;
	//	}
	//	if (numVertices==0) {
	//		return x[numInput];
	//	}
	//	else {
	//		cout << "Непредвиденность в getValue";
	//		exit(0);
	//	}
	//}


	return 0.0;
}

void Tree::replaceNode(int search, Tree& newNode)//Замена выбранного узла
{
	if (numNodes == search) {//Если мы дошли до узла под каким то номером
		*this = newNode;
		return;
	}
	//Поиск по другим узлам если не нашли подходящего номера
	if (left != nullptr and search <= left->getNumNodes()) {
		left->replaceNode(search, newNode);
	}
	if (right != nullptr and search <= right->getNumNodes()) {
		right->replaceNode(search, newNode);
	}
}

void Tree::changeNode(int search, Tree& newNode)//Отличие от replace в том, что не меняются остальные узлы
{
	if (numNodes == search) {//Если мы дошли до узла под каким то номером
		if (newNode.getLastVertice()) {
			//*this = newNode;
			if (newNode.getLastVertice() == lastVertice) {
				*this = newNode;
				return;
			}
			return;
		}
		if (lastVertice) {
			int tnumnodes = numNodes;
			*this = newNode;
			numNodes = tnumnodes;
		}
		else {


			numberFunc = newNode.getNumFunc();
			if (left != nullptr) {
				delete left;
			}
			left = new Tree;
			//left->operator=(*copy.left);
			*left = Tree(*(newNode.left));
			left->numNodes = -1;//Сделано для того, чтобы не было изменений в этой ветви
			
		}
		return;
	}

	if (left != nullptr and search <= left->getNumNodes()) {
		left->changeNode(search, newNode);
	}
	if (right != nullptr and search <= right->getNumNodes()) {
		right->changeNode(search, newNode);
	}
}

void Tree::trainWithDE(double** x, int size, double K1)
{
	/*if (label == nullptr) {
		label = new int[size];
		Tree::size = size;
	}





	int numVertices = getNumVertices();
	if (numVertices == 0) {
		calcFitness(x, size, K1);
		return;
	}
	function <double(double*)> func = [&](double* input) {
		int i = 0;
		changeCoef(input, i);
		calcFitness(x, size, K1);
		return fitness;
		};

	double* limits = new double[numVertices * 2];

	for (int i = 0; i < numVertices * 2; i++) {
		if (i % 2) {
			limits[i] = 30;
		}
		else {
			limits[i] = -30;
		}
	}


	DiffEvolution DE(func, limits, numVertices, "targetToBest1", "max");
	DE.startSearch(0.01, 0.5, 0.5, 50, 50);
	int i = 0;
	double* coef = DE.getBestCoordinates();
	changeCoef(coef, i);
	calcFitness(x, size, K1);
*/


}





