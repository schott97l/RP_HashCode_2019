#ifndef __GENETIC_H__
#define __GENETIC_H__

#include"Solver.h"
#include"Random.h"
#include"Greedy.h"

class Genetic : public Solver{
    public:
        int nb_generation;
        int population_size;
        int length;
        int timelimit;
        vector<Sol*> population;

        Genetic(int nb_generation, int population_size, int timelimit);
        Sol * solve();
        Sol * select();
        Sol * cross_over(Sol * parent_A, Sol * parent_B);
        Sol * mutation(Sol * solution);
        vector<double> softmax();
        Sol * swap_slides_neighbour(Sol * solution, int idx1);
        Sol * swap_verticals_neighbour(Sol * solution, int idx1, int photo_idx1);
};

#endif
