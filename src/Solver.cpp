#include"Solver.h"

using namespace std;

Solver::Solver(){
}

string itos(int i) {stringstream s; s << i; return s.str(); }

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

int Solver::eval(){
    return this->sol->eval();
}
