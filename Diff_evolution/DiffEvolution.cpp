#include "DiffEvolution.h"

IndividualDiffEvolution DiffEvolution::crossover(IndividualDiffEvolution donor)
{

    int ammount = 0;         // Количество раз когда поменялся вектор
    int number = gen() % N; // Номер родителя
    double rep;              // Вероятность поменять координаты
    double *coordinates = donor.getCoordinats();

    // Процесс изменения гена
    for (int i = 0; i < ammDimens; i++)
    {
        rep = gen() % 1000 / 1000.0;
        if (rep < Cr)
        {
            coordinates[i] = arrIndividuals[number].getCoordinats()[i];
            ammount++;
        }
    }
    if (ammount == 0)
    { // В случае если ген ни разу не поменялся
        int ran = gen() % ammDimens;
        coordinates[ran] = arrIndividuals[number].getCoordinats()[ran];
    }
    donor.replaceCoordinats(coordinates);
    return donor;
}

void DiffEvolution::surviveCrossover(IndividualDiffEvolution trial)
{
    for (int i = 0; i < N; i++)
    {
        if (arrIndividuals[i] < trial)
        {
            arrIndividuals[i] = trial;
            break;
        }
    }
}

void DiffEvolution::saveBest()
{
    for (int i = 0; i < N; i++)
    {
        if (best < arrIndividuals[i])
        {
            best = arrIndividuals[i];
        }
    }
}

bool DiffEvolution::networkQualityCheck(int generation)
{
    if (generation < round(generations * rejectionRate)+1)
        return true;
    for (int i = generation; i > generation - round(generations * rejectionRate); i--)
    {
        if (trackBest[i] > trackBest[i - 1])
            return true;
    }
    //return true;
    return false;
}

bool DiffEvolution::overFittingCheck(int generation)
{
    trackValidationFitness[generation] = overFittingFunc(best.getCoordinats());
    //cout << maxPartPatience << endl;
    if (generation < round(generations * maxPartPatience)+1) {
        bestPation = best;
        return false;
    }
    for (int i = generation; i > generation - round(generations * maxPartPatience); i--)
    {
        if (trackValidationFitness[i] >= trackValidationFitness[i - 1]) {
            bestPation = best;
            return false;
        }
            
    }
    best = bestPation;
    return true;
    /*if ((validationError / best.getFitness()) >= 0.9) // We want MAXIMAZE fitness
    {
        bestPation = best;
        currentPatience = 0;
        return false; // All good
    }
    else
    {
        currentPatience++;
    }
    if (currentPatience >= maxPartPatience * generations)
    {
        // cout<<"Overfitted"<<endl;
        best = bestPation; // Return to best individual before overfitting
        return true;       // Overfitted
    }
    return false;*/
}

void DiffEvolution::startSearch(double acc, double F, double Cr, int N, int generations, ComputingLimitation &cl)
{
    DiffEvolution::F = F;
    DiffEvolution::Cr = Cr;
    DiffEvolution::N = N;
    DiffEvolution::generations = generations;
    mutation.setF(F); // Установака для корректной работы мутации

    arrIndividuals = new IndividualDiffEvolution[N];

    trackBest = new double[generations];
    trackValidationFitness = new double[generations];

    // Генерация первой популяции
    for (int i = 0; i < N; i++)
    {
        IndividualDiffEvolution ind(limitsDimension, func, ammDimens, acc, aim);
        arrIndividuals[i] = ind;
        arrIndividuals[i].calcFitness();
    }

    saveBest();
    bestPation = best;
    if (best.getFitness() < 0) {
        cout << "Blya";
    }
    trackBest[0] = best.getFitness();
    trackValidationFitness[0] = 99999999999999999;
    // bestPation = best;
    if (cl.getComputingLimitation() == 0)
    {
        return;
    }

    // Начало работы основной части программы
    IndividualDiffEvolution newInd;
    for (int i = 1; i < generations; i++)
    {
        for (int j = 0; j < N; j++)
        {

            newInd = mutation.getDonor(arrIndividuals, best, N);
            newInd = crossover(newInd);
            newInd.calcFitness();

            surviveCrossover(newInd);
        }
        saveBest();
        trackBest[i] = best.getFitness();
        if (cl.getComputingLimitation() == 0 or !networkQualityCheck(i) or
            overFittingCheck(i)) // If amount of computing is over or best dont change then stop
        {
            return;
        }
    }
}
