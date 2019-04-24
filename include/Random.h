#ifndef __RANDOM_H__
#define __RANDOM_H__

#include"Solver.h"

class Random : public Solver{
    public:

        Random();
        Sol * solve();
};

#endif
