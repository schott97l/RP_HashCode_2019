#ifndef __HORI_VERTI_H__
#define __HORI_VERTI_H__

#include<iostream>
#include<fstream>
#include<string>
#include<list>
#include<vector>
#include<stdlib.h>
#include"Instance.h"

using namespace std;

class Hori_verti{
    public:
        Instance * instance;
        Sol sol;

        Hori_verti(Instance *instance);
        void solve();
        void save(ostream &o);
        double eval();
};

#endif
