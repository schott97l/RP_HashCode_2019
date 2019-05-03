#ifndef __ILP_H__
#define __ILP_H__

#include"Solver.h"
#include"gurobi_c++.h"

class Ilp : public Solver{
    public:
        string subtour;
        int timelimit;

        Ilp(string subtour, int timelimit);
        Sol * solve();
};

#endif
