#ifndef __ANNEALING_H__
#define __ANNEALING_H__

#include"Solver.h"
#include"Random.h"

class Annealing : public Solver{
    public:
        int nb_iter;
        int nb_neighbours;
        int timelimit;

        Annealing(int nb_iter, int nb_neighbours,int timelimit);
        Sol * solve();
        Sol * swap_slides_neighbour(int idx_slide,double T);
        Sol * swap_verticals_neighbour(int idx_slide, int idx_photo, double T);
};

#endif
