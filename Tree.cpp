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

	clustering(x, size);

	int* sizeClust = new int[numCluster];
	for (int i = 0; i < numCluster; i++) {
		sizeClust[i] = 0;
	}


	for (int i = 0; i < size; i++) {
		sizeClust[label[i] - 1]+=1;
	}
	if (sizeClust[0] <= 1 or sizeClust[1] <= 1) {
		fitness = -999;
		delete[] sizeClust;
		return;
	}


	double a = distanceAverageIn(x, sizeClust, size, 1);
	a += distanceAverageIn(x, sizeClust, size, 2);
	a /= 2;
	double b = distanceAverageOut(x, sizeClust, size);



	fitness = (b-a)/(max(b,a));
	if (fitness == 0)
		fitness += 0.00001;
	if (fitness == NULL) {
		cout << "Фитнес равен NAN";
		exit(0);
	}
	delete[] sizeClust;
}

Tree::Tree(int d, int numInputs)
{
	ammInputs = numInputs;
	//Случай если дошли до самого конца
	if (d == 0) {
		lastVertice = true;
		if (rand() % (numInputs+1)) {
			numInput = rand() % numInputs;
			numVertices = 0;
		}
		else {
			numVertices = 1;
			coef = 1;
		}
		return;
	}
	
	int r = rand() % 2;
	
	
	if (r) {
		//В случае если унарная
		unarFuncUs = true;
		
		numberFunc = rand() % (unarFunc.size());
		//Tree a(d - 1);
		right = new Tree(d-1,numInputs);
	}
	else {
		//В случае если бинарная
		unarFuncUs = false;
		numberFunc = rand() % (binaryFunc.size());
		//Tree l(d - 1);
		//Tree r(d - 1);
		left = new Tree(d - 1,numInputs);
		right = new Tree(d - 1,numInputs);
	}
}


string Tree::getFunc()
{
	stringstream ss;
	if (lastVertice) {
		if (numVertices == 1) {
			ss << coef;
		}
		else {
			ss <<'X' << numInput;
		}

	}
	else {
		if (left != nullptr) {
			ss << '(';
			ss << left->getFunc();
		}
		if (unarFuncUs) {
			if (numberFunc == 0 and !lastVertice) {
				ss << '(';
			}
			else {
				ss << strUnarFunc[numberFunc];
				if (lastVertice == false) {
					ss << '(';
				}
			}
		}
		else {
			if (!lastVertice) {
				ss << strBinaryFunc[numberFunc];
			}

		}
		if (right != nullptr) {
			ss << right->getFunc();
			ss << ')';
		}
	}
	

	return ss.str();
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
		coef = in[z];//Замена коэффициентов в случае если все ок
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

double Tree::getValue(double* x)
{
	//if (right != nullptr and left ==nullptr) {//Если справа что то есть то это точно унарная функци

	//	return unarFunc[numberFunc](right->getValue(x));
	//	
	//}
	if (lastVertice) {//Если дошли до вершины
		if (numVertices==1) {
			return coef;
		}
		if (numVertices==0) {
			return x[numInput];
		}
		else {
			cout << "Непредвиденность в getValue";
			exit(0);
		}
	}

	if (unarFuncUs) {
		return unarFunc[numberFunc](right->getValue(x));
	}
	else {//Если попались в бинарную функцию
		return binaryFunc[numberFunc](left->getValue(x),right->getValue(x));
	}
	
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
		if (newNode.getUnar() == unarFuncUs) {
			numberFunc = newNode.getNumFunc();
		}
		else {
			if (newNode.getUnar()) {
				unarFuncUs = true;
				numberFunc = newNode.getNumFunc();
				left->~Tree();
				left = nullptr;
			}
			else {
				unarFuncUs = false;
				numberFunc = newNode.getNumFunc();
				if (left != nullptr) {
					delete left;
				}
				left = new Tree;
				//left->operator=(*copy.left);
				*left = Tree(*(newNode.left));
				left->numNodes = -1;//Сделано для того, чтобы не было изменений в этой ветви
			}
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
	if (label == nullptr) {
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



}




double Tree::distanceAverageIn(double** data, int* sizeClust, int str, int cluster) {

	double sum = 0, dist = 0;

	for (int i = 0; i < str; i++) {
		for (int j = 0; j < str; j++) {
			if (label[i] != label[j] or label[i] != cluster or i==j)
				continue;
			dist = 0;
			for (int w = 0; w < ammInputs; w++) {

				dist += pow(data[i][w] - data[j][w], 2);

			}
			dist = pow(dist, 0.5);
			sum += dist;



		}
	}


	sum = (sum / (sizeClust[cluster-1])) / (sizeClust[cluster - 1] - 1);

	return sum;

}
double Tree::distanceAverageOut(double** data, int* sizeClust, int str) {

	double sum = 0, dist = 0;


	for (int i = 0; i < str; i++) {
		for (int j = 0; j < str; j++) {
			if (label[i] == label[j])
				continue;

			dist = 0;
			for (int w = 0; w < ammInputs; w++) {

				dist += pow(data[i][w] - data[j][w], 2);

			}
			dist = pow(dist, 0.5);
			sum += dist;



		}
	}

	for (int i = 0; i < numCluster; i++) {
		sum /= sizeClust[i];
	}

	return sum;

}

void Tree::clustering(double** data, int str)
{
	
	if (numCluster == 2) {

		for (int i = 0; i < str; i++) {
			if (getValue(data[i]) > 0) {
				label[i] = 1;
			}
			else {
				label[i] = 2;
			}
		}



	}



}
