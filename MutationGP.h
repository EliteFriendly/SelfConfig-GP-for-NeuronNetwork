#pragma once
#include <iostream>
#include <vector>
#include"Tree.h"
#include <random>

using namespace std;

class MutationGP
{
protected:
	int treeSize;

public:

	virtual void doMutChild(Tree&) = 0;

};



class TreeMutation : public MutationGP {


	int treeDeapth;
public:
	TreeMutation(int treeDeapth) : treeDeapth(treeDeapth) {}

	void doMutChild(Tree& ind) {

		int r = ind.getNumNodes();
		int chosenNode = rand() % (r + 1);
		//Tree* node = &ind;
		Tree newNode(treeDeapth, ind.getAmmInputs());//��� ���������� ������� �� �����
		ind.replaceNode(chosenNode, newNode);
		int nodes = 0, lvl = 0;
		ind.recountLayers(lvl);
		ind.countNodes(nodes);
	}

};




class PointMutation : public MutationGP {

	string powerMut;

	bool checkChance(int ammNodes) {

		double probability;

		if (powerMut == "Average") {

			probability = 1.0 / ammNodes;

		}

		else if (powerMut == "Weak") {

			probability = 1.0 / (ammNodes * 3);

		}
		else if (powerMut == "Strong") {

			probability = 3.0 / ammNodes;
			if (probability > 1) {
				probability = 1;
			}

		}
		else {
			cout << "Wrong name mutation";
			exit(0);

		}

		mt19937 gen(rand());

		double mut = gen() % 1000000 / 1000000.0;

		if (probability > mut) {
			return true;
		}
		else {
			return false;
		}
	}

public:

	PointMutation(string powerMut) :powerMut(powerMut) {}

	void doMutChild(Tree& ind) {
		int r = ind.getNumNodes()+1;
		bool mutCheck;//�� ��� ����� ��������� ���������� �� ����
		for (int i = 0; i < r; i++) {

			//��������� ������ ���� �� ���� �������

			mutCheck = checkChance(r);
			if (mutCheck) {
				//���� ����������� ��������, �� ���� � i-�� ����
				Tree* node = &ind;
				bool t = false;
				while (t == false) {
					/*
						������ ����� ���������� �����, ��� ����� �� ��������� ���� ��������� ���� ������
					������� ���� �����, �� ������ ��������� ���� ����� ������ � ��������

					*/
					if (node->getNumNodes() == i) {
						t = true;
						break;
					}
					if (node->getLeft() != nullptr and i <= node->getLeft()->getNumNodes()) {
						node = node->getLeft();
						continue;
					}
					if (node->getRight() != nullptr and i <= node->getRight()->getNumNodes()) {
						node = node->getRight();
						continue;
					}

				}
				node->randFunc();
			}
		}

	}

};