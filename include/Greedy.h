#ifndef __GREEDY_H__
#define __GREEDY_H__

#include"Solver.h"

class Greedy : public Solver{
    public:
        int length;
        Greedy(int length);
        Sol * solve();
};

#endif
