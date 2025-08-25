#pragma once
#include <iostream>
#include "..\\neuron_network\\Tree.h"
#include <vector>
#include <random>
using namespace std;

class CrossoverGP
{
protected:

	int* arrayReach = nullptr;//0 - недостижима 1 - достижима
	void findReach(Tree& first, Tree& second) {

		int ammNodes = first.getNumNodes();

		if (arrayReach != nullptr) {
			delete[] arrayReach;
			arrayReach = nullptr;
		}

		arrayReach = new int[ammNodes];

		for (int i = 0; i < ammNodes; i++) {
			arrayReach[i] = 0;
		}
		for (int i = 0; i < ammNodes; i++) {

			//Проверяем каждый узел на то, что он будет достижим

			Tree* node1 = &first;
			Tree* node2 = &second;
			/*
			Если узел что мы ищем уже был проверен, то его пропускаем
			*/
			while (arrayReach[i] == 0) {
				/*
					Отсчет узлов начинается слева, тем самым мы проверяем если выбранный узел больше
				первого узла слева, то значит выбранный узел точно правее и наоборот

				*/
				if (node1->getNumNodes() == i) {
					//Если узел был достигнут, то изменяем значение в массиве
					arrayReach[node1->getNumNodes()] = 1;
					break;
				}
				if (node1->getLeft() != nullptr and i <= node1->getLeft()->getNumNodes()) {
					//Проверка что оба узла хранят одну функцию
					if ((node1->getLastVertice() == node2->getLastVertice())) {
						node1 = node1->getLeft();//Идем по маршруту и там и сям
						node2 = node2->getLeft();
						arrayReach[node1->getNumNodes()] = 1;//Пройденный узел отмечаем
						continue;
					}
					else {
						//Если оказался 1 узел последний
						if (node1->getLastVertice() or node2->getLastVertice()) {
							arrayReach[node1->getNumNodes()] = 1;//Данный последний узел достижим
							arrayReach[i] = 0;//А искомый - нет
							break;
						}
						else {//Если не совпали

							//Вдруг узел справа окажется тем самым
							if (node1->getRight()->getNumNodes() == i) {
								arrayReach[i] = 1;
								break;
							}
							else {//Иначе говорим что правый достижим
								arrayReach[node1->getRight()->getNumNodes()] = 1;
								break;
							}
						}

					}


				}
				//Аналогично но справа
				if (node1->getRight() != nullptr and i <= node1->getRight()->getNumNodes()) {
					if ((node1->getLastVertice() == node2->getLastVertice())) {
						node1 = node1->getRight();
						node2 = node2->getRight();
						arrayReach[node1->getNumNodes()] = 1;
						continue;
					}
					else {
						if (node1->getLastVertice()) {
							arrayReach[node1->getNumNodes()] = 1;
							arrayReach[i] = 0;
							break;
						}
						else {
							if (node1->getRight()->getNumNodes() == i and !(node2->getLastVertice())) {
								arrayReach[i] = 1;
								break;
							}
							else {
								arrayReach[i] = 0;
								break;
							}
						}

					}
				}

			}






		}

	}

	int ruleReplace(int chosenNode, bool inputBranch, Tree& second) {
		int secNode;
		if (inputBranch) {
			secNode = second.getLeft()->getNumNodes();
			return rand() % (secNode + 1);//????
		}
		else {
			secNode = second.getRight()->getNumNodes() - second.getLeft()->getNumNodes();//Чтобы узнать сколько узлов в правой ветке
			return rand() % (secNode)+second.getLeft()->getNumNodes() + 1;
		}

	}
	
public:
	virtual Tree getChild(Tree &first, Tree &second) = 0;


	~CrossoverGP() {
		if (arrayReach != nullptr) {
			delete[] arrayReach;
			arrayReach = nullptr;
		}
	}
};


class EmptyCrossover : public CrossoverGP {
public:

	Tree getChild(Tree &first, Tree &second) {

		if (rand() % 2) {
			return first;
		}
		else
		{
			return second;
		}
	}

};



class StandartCrossover : public CrossoverGP {



	Tree getChild(Tree &first, Tree &second) {

		Tree child(first);
		int r = child.getNumNodes();
		int chosenNode = rand() % r;
		bool chosenInputBranch = false;
		if (chosenNode <= child.getLeft()->getNumNodes()) {
			chosenInputBranch = true;
		}
		//Выбор рандомного узла для 2 родителя
		int chosenNode2 = ruleReplace(chosenNode,chosenInputBranch,second);
		Tree* nodeParent = &second;
		bool t = false;
		//Начало спуска до этого узла
		while (t == false) {
			if (nodeParent->getNumNodes() == chosenNode2) {
				t = true;
				break;
			}
			if (nodeParent->getLeft() != nullptr and chosenNode2 <= nodeParent->getLeft()->getNumNodes()) {
				nodeParent = nodeParent->getLeft();
				continue;
			}
			if (nodeParent->getRight() != nullptr and chosenNode2 <= nodeParent->getRight()->getNumNodes()) {
				nodeParent = nodeParent->getRight();
				continue;
			}


		}
		//Замена у ребенка выбранного узла у первого и второго родителя
		child.replaceNode(chosenNode, *nodeParent);
		//Используется чтобы пронумеровать все узлы
		int z = 0, lvl = 0;
		child.recountLayers(lvl);
		child.countNodes(z);
		return child;

	}
};


class OnepointCrossover : public CrossoverGP {

private:

	



public:

	Tree getChild(Tree &first, Tree &second) {

		Tree child(first);
		findReach(child, second);
		int chosenNode = rand() % child.getNumNodes();

		while (!arrayReach[chosenNode]) {
			chosenNode = rand() % child.getNumNodes();
		}


		//Если вероятность прокнула, то идем о i-го узла
		Tree* node1 = &first;
		Tree* node2 = &second;
		bool t = false;
		while (t == false) {
			/*
				Отсчет узлов начинается слева, тем самым мы проверяем если выбранный узел больше
			первого узла слева, то значит выбранный узел точно правее и наоборот

			*/
			if (node1->getNumNodes() == chosenNode) {
				t = true;
				break;
			}
			if (node1->getLeft() != nullptr and chosenNode <= node1->getLeft()->getNumNodes()) {
				node1 = node1->getLeft();
				node2 = node2->getLeft();
				continue;
			}
			if (node1->getRight() != nullptr and chosenNode <= node1->getRight()->getNumNodes()) {
				node1 = node1->getRight();
				node2 = node2->getRight();
				continue;
			}

		}
		
		child.replaceNode(chosenNode, *node2);
		//Используется чтобы пронумеровать все узлы
		int z = 0, lvl = 0;
		child.recountLayers(lvl);
		child.countNodes(z);
		return child;

	}


};





class UniformCrossover : public CrossoverGP {


	bool checkChance(int ammNodes) {

		double probability;


		probability = 1.0 / ammNodes;

		mt19937 gen(rand());
		gen.seed(rand());

		double mut = gen() % 1000000 / 1000000.0;

		if (probability > mut) {
			return true;
		}
		else {
			return false;
		}
	}



public:

	Tree getChild(Tree& first, Tree& second) {

		Tree child(first);
		findReach(child, second);
		int r = child.getNumNodes();
		bool swapCheck;
		for (int i = 0; i < r; i++) {
			//Проверяем каждый узел на шанс свапа

			if (!arrayReach[i]) {//Если не достижимо
				continue;
			}
			Tree* node1 = &first;
			Tree* node2 = &second;
			

			swapCheck = checkChance(2);//Ибо 2 родителя
			if (swapCheck) {
				bool t = false;
				while (t == false) {
					/*
						Отсчет узлов начинается слева, тем самым мы проверяем если выбранный узел больше
					первого узла слева, то значит выбранный узел точно правее и наоборот

					*/
					if (node1->getNumNodes() == i) {
						t = true;
						break;
					}
					if (node1->getLeft() != nullptr and i <= node1->getLeft()->getNumNodes()) {
						node1 = node1->getLeft();
						node2 = node2->getLeft();
						continue;
					}
					if (node1->getRight() != nullptr and i <= node1->getRight()->getNumNodes()) {
						node1 = node1->getRight();
						node2 = node2->getRight();
						continue;
					}

				}
				child.changeNode(i, *node2);
			}
			
		}




		int z = 0, lvl = 0;
		child.recountLayers(lvl);
		child.countNodes(z);
		return child;

	}
};

