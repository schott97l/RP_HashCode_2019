#ifndef __STOCH_DESCENT_H__
#define __STOCH_DESCENT_H__

#include"Solver.h"

class Stoch_descent : public Solver{
    public:
        Stoch_descent();
        void solve();
};

#endif
