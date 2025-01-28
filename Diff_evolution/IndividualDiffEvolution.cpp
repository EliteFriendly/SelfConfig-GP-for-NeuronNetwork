#include "IndividualDiffEvolution.h"
#include <functional>

IndividualDiffEvolution::IndividualDiffEvolution(double* limitsDimension, function<double(double*)> func, int ammDimen, double acc,string aim) :
	ammDimen(ammDimen), func(func), acc(acc), aim(aim) {

	IndividualDiffEvolution:: limitsDimension = new double[ammDimen * 2];
	for (int i = 0; i < ammDimen*2; i++)
	{
		IndividualDiffEvolution::limitsDimension[i] = limitsDimension[i];
	}
	coordinats = new double[ammDimen];
	int n;//Хранит количество точек, временна

	for (int i = 0; i < ammDimen*2; i+=2) {
		n = (abs(limitsDimension[i] - limitsDimension[i + 1])) / acc;
		coordinats[i / 2] = (rand() % n)*acc + limitsDimension[i];

	}


}

IndividualDiffEvolution::IndividualDiffEvolution(const IndividualDiffEvolution& copy):
	ammDimen(copy.ammDimen), func(copy.func), 
	acc(copy.acc),fitness(copy.fitness),aim(copy.aim){

	if (limitsDimension == nullptr) {
		limitsDimension = new double[copy.ammDimen * 2];
	}
	if (coordinats == nullptr) {
		coordinats = new double[copy.ammDimen];
	}

	for (int i = 0; i < ammDimen * 2; i++)
	{
		IndividualDiffEvolution::limitsDimension[i] = copy.limitsDimension[i];
	}
	for (int i = 0; i < ammDimen; i++)
	{
		IndividualDiffEvolution::coordinats[i] = copy.coordinats[i];
	}
}

















bool IndividualDiffEvolution::operator <(const IndividualDiffEvolution& copy) {
	if (fitness < copy.fitness)
		return true;
	return false;
}
bool IndividualDiffEvolution::operator >(const IndividualDiffEvolution& copy) {
	if (fitness > copy.fitness)
		return true;
	return false;
}
//bool IndividualDiffEvolution::operator ==(const IndividualDiffEvolution& copy) {
//	if (fitness == copy.fitness)
//		return true;
//	return false;
//}

IndividualDiffEvolution IndividualDiffEvolution::operator+(const IndividualDiffEvolution& sec) 
{
	IndividualDiffEvolution copy(*this);
	for (int i = 0; i < copy.ammDimen; i++) {
		copy.coordinats[i] += sec.coordinats[i];
	}
	return copy;
}




IndividualDiffEvolution IndividualDiffEvolution::operator -(const IndividualDiffEvolution& sec)
{
	IndividualDiffEvolution copy(*this);
	for (int i = 0; i < copy.ammDimen; i++) {
		copy.coordinats[i] = copy.coordinats[i]- sec.coordinats[i];
	}
	return copy;
}

IndividualDiffEvolution IndividualDiffEvolution::operator=(const IndividualDiffEvolution& copy)
{
	ammDimen = copy.ammDimen;
	func = copy.func;
	acc = copy.acc;
	fitness = copy.fitness;
	aim = copy.aim;

	if (limitsDimension == nullptr) {
		limitsDimension = new double[copy.ammDimen * 2];
	}
	if (coordinats == nullptr) {
		coordinats = new double[copy.ammDimen];
	}

	for (int i = 0; i < ammDimen * 2; i++)
	{
		IndividualDiffEvolution::limitsDimension[i] = copy.limitsDimension[i];
	}
	for (int i = 0; i < ammDimen; i++)
	{
		IndividualDiffEvolution::coordinats[i] = copy.coordinats[i];
	}
	return *this;
}
IndividualDiffEvolution IndividualDiffEvolution::operator *(double sec)
{
	IndividualDiffEvolution copy(*this);
	for (int i = 0; i < copy.ammDimen; i++) {
		copy.coordinats[i] *= sec;
	}
	return copy;
}