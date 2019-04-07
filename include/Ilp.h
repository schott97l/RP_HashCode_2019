#ifndef __ILP_H__
#define __ILP_H__

#include"Solver.h"

class Ilp : public Solver{
    public:
        Ilp();
        Sol * solve();
};

#endif
