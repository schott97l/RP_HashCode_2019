#ifndef __SOLVER_H__
#define __SOLVER_H__

#include<iostream>
#include<fstream>
#include<string>
#include<list>
#include<vector>
#include<cstdlib>
#include"Instance.h"


class Solver{
    public:
        Instance * instance;
        Sol sol;

        Solver();
        void load(Instance *instance);
        virtual void solve()=0;
        void save(ostream &o);
        double eval();
};

#endif
