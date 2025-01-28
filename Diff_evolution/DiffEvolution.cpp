#include "DiffEvolution.h"


IndividualDiffEvolution DiffEvolution::crossover(IndividualDiffEvolution donor)
{
	
	int ammount=0;//���������� ��� ����� ��������� ������
	int number = rand() % N;//����� ��������
	double rep;//����������� �������� ����������
	double* coordinates = donor.getCoordinats();

	//������� ��������� ����
	for (int i = 0; i < ammDimens; i++) {
		rep = rand() % 1000 / 1000.0;
		if (rep < Cr) {
			coordinates[i] = arrIndividuals[number].getCoordinats()[i];
			ammount++;
		}
	}
	if (ammount == 0) {//� ������ ���� ��� �� ���� �� ���������
		int ran = rand() % ammDimens;
		coordinates[ran] = arrIndividuals[number].getCoordinats()[ran];
	}
	donor.replaceCoordinats(coordinates);
	return donor;
}

void DiffEvolution::surviveCrossover(IndividualDiffEvolution trial)
{
	for (int i = 0; i < N; i++) {
		if (arrIndividuals[i] < trial) {
			arrIndividuals[i] = trial;
			break;
		}
	}
}

void DiffEvolution::saveBest()
{
	for (int i = 0; i < N; i++) {
		if (best < arrIndividuals[i]) {
			best = arrIndividuals[i];
		}
	}
}

void DiffEvolution::startSearch(double acc, double F, double Cr, int N, int generations) {
	DiffEvolution::F = F;
	DiffEvolution::Cr = Cr;
	DiffEvolution::N = N;
	DiffEvolution::generations = generations;
	mutation.setF(F);//���������� ��� ���������� ������ �������

	arrIndividuals = new IndividualDiffEvolution[N];

	//��������� ������ ���������
	for (int i = 0; i < N; i++) {
		IndividualDiffEvolution ind(limitsDimension, func, ammDimens, acc, aim);
		arrIndividuals[i] = ind;
		arrIndividuals[i].calcFitness();
	}
	
	saveBest();

	//������ ������ �������� ����� ���������
	IndividualDiffEvolution newInd;
	for (int i = 0; i < generations; i++)
	{
		for (int j = 0; j < N; j++) {
			//cout << "����� ��������� = " << i << ", ����� �������� = " << j << endl;
			newInd = mutation.getDonor(arrIndividuals,best,N);
			newInd = crossover(newInd);
			newInd.calcFitness();
			surviveCrossover(newInd);
			newInd.~IndividualDiffEvolution();
		}
		saveBest();
	}
	
}
