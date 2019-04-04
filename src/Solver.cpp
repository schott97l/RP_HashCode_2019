#include"Solver.h"

using namespace std;

Solver::Solver(){
}

void Solver::load(Instance * instance){
    this->instance = instance;
}

void Solver::save(ostream &o){
    this->sol.aff(o);
}

double Solver::eval(){
    return this->sol.eval();
}
