#include "AdaptiveGeneticProgramming.h"

void AdaptiveGeneticProgramming::findBest()
{
	for (int i = 0; i < numIndividuals; i++) {
		if (arrayIndividuals[i].getFitness() > bestIndividual.getFitness()) {
			bestIndividual = arrayIndividuals[i];
		}
	}
}

void AdaptiveGeneticProgramming::setSelectionsArrays()
{
	selection[0]->setArrIndividuals(arrayIndividuals,numIndividuals);
	selection[1]->setArrIndividuals(arrayIndividuals, numIndividuals);
	selection[2]->setArrIndividuals(arrayIndividuals, numIndividuals);
	selection[3]->setArrIndividuals(arrayIndividuals, numIndividuals);
	selection[4]->setArrIndividuals(arrayIndividuals, numIndividuals);
	
}

Tree AdaptiveGeneticProgramming::createChild(int numInd)
{


	chosenSel[numInd] = probabilityChoice(selProbabilities, 5);

	//cout << "Номер генерации = " << i <<", Номер индивида = " << j << endl;
	int numParent1 = selection[chosenSel[numInd]]->getNumParents();
	int numParent2 = selection[chosenSel[numInd]]->getNumParents();
	while (numParent1 == numParent2) {
		numParent2 = selection[chosenSel[numInd]]->getNumParents();
	}
	
	chosenCross[numInd] = probabilityChoice(crossProbabilities, 4);
	Tree child = crossover[chosenCross[numInd]]->getChild(arrayIndividuals[numParent1], arrayIndividuals[numParent2]);

	chosenMut[numInd] = probabilityChoice(mutProbabilities, 4);

	mutation[chosenMut[numInd]]->doMutChild(child);
	return child;


}

void AdaptiveGeneticProgramming::threadsFitnessCalc(double**x,int ammThread)
{
	if (ammThread == 1) {

		for (int i = 0; i < numIndividuals; i++) {
			arrayChildren[i].trainWithDE(x,  size, K1);
		}
		return;
	}
	
	thread* thr = new thread[ammThread - 1];
	int usesThr;

	int t = numIndividuals / 2;

	//Tree* tmpArray = new Tree[t];

	//for (int i = 0; i < t; i++) {
	//	tmpArray[i] = arrayChildren[i];
	//}


	thr[0] = thread([&]() {
		for (int i = 0; i < t; i++) {
			arrayChildren[i].trainWithDE(x,  size, K1);
		}
		});
	for (int i = t; i < numIndividuals; i++) {
		arrayChildren[i].trainWithDE(x,  size, K1);
	}
	
	thr[0].join();

	//for (int i = 0; i < t; i++) {
	//	arrayChildren[i] = tmpArray[i];
	//}
	//for (int i = 0; i < numIndividuals; i++) {
	//	usesThr = 0;
	//	while (usesThr < (ammThread - 1)) {
	//		thr[usesThr] = thread([&]() {arrayChildren[i].trainWithDE(x, y, size, K1); });
	//		usesThr++;
	//		i++;
	//	}
	//	usesThr = 0;
	//	//arrayChildren[i].trainWithDE(x, y, size, K1);
	//	while (usesThr < (ammThread - 1)) {
	//		thr[usesThr].join();
	//		usesThr++;
	//	}
	//}
	delete[]thr;

}

int AdaptiveGeneticProgramming::findWinner(int* arrPlayers, int ammPlayer,double* arrFitness)
{
	double* fitnessPlayers = new double[ammPlayer];
	int ammUses;

	for (int i = 0; i < ammPlayer; i++) {
		fitnessPlayers[i] = 0;
		ammUses = 0;
		for (int j = 0; j < numIndividuals; j++) {
			if (arrPlayers[j] == i) {
				fitnessPlayers[i] += arrFitness[j];
				ammUses++;
			}
		}
		fitnessPlayers[i] /= ammUses;
	}
	int best;
	double fitbest = -9999999;

	for (int i = 0; i < ammPlayer; i++) {
		if (fitnessPlayers[i] > fitbest) {
			fitbest = fitnessPlayers[i];
			best = i;
		}
	}
	

	delete[] fitnessPlayers;
	return best;
}

void AdaptiveGeneticProgramming::changeProbabilities(double* arrProba, int numWinner, int ammPlayers)
{
	double sumPenlty = 0;
	double penalty = 2.0 / (numGeneration * ammPlayers);

	for (int i = 0; i < ammPlayers; i++) {
		if (i != numWinner) {
			if (arrProba[i] > socialCard) {
				if ((arrProba[i] - penalty) < socialCard) {
					sumPenlty += arrProba[i] - socialCard;
					arrProba[i] = socialCard;
				}
				else {
					arrProba[i] -= penalty;
					sumPenlty += penalty;
				}
			}

		}
	}

	arrProba[numWinner] += sumPenlty;
}

void AdaptiveGeneticProgramming::recalcProbabilities()
{
	double sumFitness = 0;
	double* arrFitness = new double[numIndividuals];
	for (int i = 0; i < numIndividuals; i++) {
		arrFitness[i] = arrayChildren[i].getFitness();
		sumFitness += arrFitness[i];
	}
	int winner;
	//Селекция
	winner = findWinner(chosenSel, 5, arrFitness);
	changeProbabilities(selProbabilities, winner, 5);

	//Кроссовер
	winner = findWinner(chosenCross, 4, arrFitness);
	changeProbabilities(crossProbabilities, winner, 4);

	//Мутация
	winner = findWinner(chosenMut, 4, arrFitness);
	changeProbabilities(mutProbabilities, winner, 4);

	saveProbabilities();

}

void AdaptiveGeneticProgramming::startTrain(double** x, int ammInputs, int size, int numIndividuals, int numGeneration)
{

	fSel.open("Probabilities/ProbabilSel_" + to_string(numberFile)+".txt");

	fCross.open("Probabilities/ProbabilCross_" + to_string(numberFile) + ".txt");
	fMut.open("Probabilities/ProbabilMut_" + to_string(numberFile) + ".txt");

	saveProbabilities();


	AdaptiveGeneticProgramming::size = size;
	AdaptiveGeneticProgramming::ammInputs = ammInputs;


	

	AdaptiveGeneticProgramming::numIndividuals = numIndividuals;
	AdaptiveGeneticProgramming::numGeneration = numGeneration;
	arrayIndividuals = new Tree[numIndividuals];
	arrayChildren = new Tree[numIndividuals];
	chosenCross = new int[numIndividuals];
	chosenMut = new int[numIndividuals];
	chosenSel = new int[numIndividuals];



	//Первая иницилизация поколения
	for (int i = 0; i < numIndividuals; i++) {
		Tree t(treeDepth-1,ammInputs);
		//Подсчет узлов и уровней
		int nodes = 0, lvl = 0;
		t.recountLayers(lvl);
		t.countNodes(nodes);

		arrayIndividuals[i] = t;
		arrayIndividuals[i].trainWithDE(x,size, K1);
	}

	findBest();//Первый поиск лучшего индивида
	//Основное начало алгоритма
	int numParent1, numParent2;

	for (int i = 0; i < numGeneration; i++) {
		cout << "Номер генерации = " << i << endl;
		setSelectionsArrays();
		for (int j = 0; j < numIndividuals; j++) {
			//cout << "Номер генерации = " << i <<", Номер индивида = " << j << endl;
		
			arrayChildren[j] = createChild(j);
			//arrayChildren[j].trainWithDE(x, y, size, K1);
		}

		threadsFitnessCalc(x,1);
		recalcProbabilities();

		forming.replaceGeneration(arrayIndividuals, arrayChildren, numIndividuals);
		findBest();


	}

}
