#ifndef __SOLVER_H__
#define __SOLVER_H__

#include<unistd.h>
#include<limits>
#include<iostream>
#include<fstream>
#include <sstream>
#include<string>
#include<list>
#include<vector>
#include<cstdlib>
#include<cmath>
#include<tuple>
#include<algorithm>
#include<ctime>
#include <cassert>
#include"Instance.h"

string itos(int i);

class Solver{
    public:
        Instance * instance=NULL;
        Sol * sol=NULL;

        Solver();
        void load(Instance *instance);
        void load(Instance *instance, Sol *sol);
        virtual Sol * solve()=0;
        void save(ostream &o);
        int eval();
};

#endif
