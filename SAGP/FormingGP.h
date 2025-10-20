#pragma once
#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include "MutationGP.h"
#include "CrossoverGP.h"
#include "..\\neuron_network\\Tree.h"
#include "../general/general_var.h"
class FormingGP
{
public:
	void replaceGeneration(Tree* arrIndividuals, Tree* arrChildren, int individuals) {
		for (int i = 0; i < individuals; i++) {
			for (int j = 0; j < individuals; j++) {
				if (arrIndividuals[i].getFitness() < arrChildren[j].getFitness()) {
					arrIndividuals[i] = arrChildren[j];
					arrChildren[j]=Tree();
					break;
				}

			}
		}
	}

};

