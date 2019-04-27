#include"Naive_greedy.h"

using namespace std;

Naive_greedy::Naive_greedy(){
}

Sol * Naive_greedy::solve(){
    int value;

    sol = new Sol();
    sol->I = instance;
    sol->vsol.clear();
    sol->eval_vect.clear();
    sol->evaluation=0;

    Slide slide;
    slide.p1=-1;
    slide.p2=-1;

    std::vector<bool> mark;
    mark.resize(instance->nbphot);
    for (int j=0;j<instance->nbphot;j++)
        mark[j]=false;

    srand(time(0));
    int idx = rand() % instance->nbphot;
    mark[idx] = true;
    slide.p1=idx;
    int i;

    if (instance->V[idx].ori == 'H'){
        sol->vsol.push_back(slide);
        i=1;
    } else {
        idx = instance->search_closerV(instance->V[idx].tags,mark);
        mark[idx] = true;
        slide.p2=idx;
        sol->vsol.push_back(slide);
        slide.p2=-1;
        i=2;
    }

    sol->eval_vect.push_back(0);
    int j=1;

    for(i;i<instance->nbphot;i++){
        idx = instance->search_closer(instance->V[idx].tags,mark);
        mark[idx] = true;
        slide.p1=idx;
        if (instance->V[idx].ori == 'H'){
            sol->vsol.push_back(slide);
        } else {
            idx = instance->search_closerV(instance->V[idx].tags,mark);
            mark[idx] = true;
            slide.p2=idx;
            sol->vsol.push_back(slide);
            slide.p2=-1;
            i++;
        }
        value = sol->eval_transition(j-1,j);
        sol->eval_vect.push_back(value);
        sol->evaluation += value;
        j++;
    }

    sol->nbslides = sol->vsol.size();
    return sol;
}
