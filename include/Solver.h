#ifndef __SOLVER_H__
#define __SOLVER_H__

#include<iostream>
#include<fstream>
#include<string>
#include<list>
#include<vector>
#include<cstdlib>
#include<cmath>
#include<tuple>
#include<algorithm>
#include"Instance.h"


class Solver{
    public:
        Instance * instance=NULL;
        Sol * sol=NULL;

        Solver();
        void load(Instance *instance);
        void load(Instance *instance, Sol *sol);
        virtual Sol * solve()=0;
        void save(ostream &o);
        double eval();
};

#endif
