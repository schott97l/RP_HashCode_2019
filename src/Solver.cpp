#include"Solver.h"

using namespace std;

Solver::Solver(){
}

void Solver::load(Instance * instance){
    this->instance = instance;
}

void Solver::load(Instance * instance, Sol * sol){
    this->instance = instance;
    this->sol = sol;
}

void Solver::save(ostream &o){
    this->sol->aff(o);
}

double Solver::eval(){
    return this->sol->eval();
}
