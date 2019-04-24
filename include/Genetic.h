#ifndef __GENETIC_H__
#define __GENETIC_H__

#include"Solver.h"
#include"Random.h"

class Genetic : public Solver{
    public:
        int nb_generation;
        int population_size;
        vector<Sol*> population;
        vector<double> evals;

        Genetic(int nb_generation, int population_size);
        Sol *  solve();
        Sol * select(vector<Sol*> population, vector<double> evals);
        Sol * cross_over(Sol * parent_A, Sol * parent_B);
        Sol * mutation(Sol * solution);
        vector<double> softmax(vector<double> evals);
};

#endif
