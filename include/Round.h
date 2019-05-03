#ifndef __ROUND_H__
#define __ROUND_H__

#include"Solver.h"
#include"gurobi_c++.h"

class Round : public Solver{
    public:
        string subtour;
        int timelimit;

        Round(string subtour, int timelimit);
        Sol * solve();
};

#endif
