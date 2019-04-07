#ifndef __STOCH_DESCENT_H__
#define __STOCH_DESCENT_H__

#include"Solver.h"
#include"Greedy.h"

class Stoch_descent : public Solver{
    public:
        int nb_iter;
        int nb_neighbours;
        int greedy_length;

        Stoch_descent(int nb_iter, int nb_neighbours);
        Stoch_descent(int nb_iter, int nb_neighbours,int greedy_length);
        Sol * solve();
        Sol * swap_slides_neighbour(int idx_slide);
        Sol * swap_verticals_neighbour(int idx_slide, int idx_photo);
};

#endif
