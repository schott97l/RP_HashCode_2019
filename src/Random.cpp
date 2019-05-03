#include"Random.h"

using namespace std;

Random::Random(){
    srand(time(0));
}

Sol * Random::solve(){
    int idx,j;
    Slide hori_slide;
    hori_slide.p1=-1;
    hori_slide.p2=-1;
    Slide verti_slide;
    verti_slide.p1=-1;
    verti_slide.p2=-1;
    int eval;


    sol = new Sol;
    sol->I = instance;
    sol->vsol.clear();
    sol->eval_vect.clear();
    sol->evaluation=0;

    vector<bool> mark;
    mark.resize(instance->nbphot);
    for (int i=0;i<instance->nbphot;i++){
        mark[i]=false;
    }

    j=0;

    for (int i=0;i<instance->nbphot;i++){
        idx = rand()%instance->nbphot;
        while(mark[idx]==true){
            idx++;
            idx %= instance->nbphot;
        }
        if(this->instance->V[idx].ori=='H'){
            hori_slide.p1 = idx;
            mark[idx]=true;
            sol->vsol.push_back(hori_slide);
            if (j==0)
                sol->eval_vect.push_back(0);
            else{
                eval = sol->eval_transition(j-1,j);
                sol->eval_vect.push_back(eval);
                sol->evaluation+=eval;
            }
            j++;
        }
        else if(this->instance->V[idx].ori=='V'){
            if(verti_slide.p1==-1){
                verti_slide.p1 = idx;
                mark[idx]=true;
            }else{
                verti_slide.p2 = idx;
                mark[idx]=true;
                sol->vsol.push_back(verti_slide);
                verti_slide.p1=-1;
                verti_slide.p2=-1;
                if (j==0)
                    sol->eval_vect.push_back(0);
                else{
                    eval = sol->eval_transition(j-1,j);
                    sol->eval_vect.push_back(eval);
                    sol->evaluation+=eval;
                }
                j++;
            }
        }
    }

    sol->nbslides = sol->vsol.size();
    sol->eval();
    return sol;
}
