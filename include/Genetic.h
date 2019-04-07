#ifndef __GENETIC_H__
#define __GENETIC_H__

#include"Solver.h"

class Genetic : public Solver{
    public:
        Genetic();
        Sol *  solve();
};

#endif
