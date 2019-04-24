#include"Random.h"

using namespace std;

Random::Random(){
}

Sol * Random::solve(){
    int idx;
    Slide hori_slide;
    hori_slide.p1=-1;
    hori_slide.p2=-1;
    Slide verti_slide;
    verti_slide.p1=-1;
    verti_slide.p2=-1;


    this->sol = new Sol;
    this->sol->I = this->instance;

    vector<bool> mark;
    mark.resize(this->instance->nbphot);
    for (int i=0;i<this->instance->nbphot;i++){
        mark[i]=false;
    }

    srand(time(0));

    for (int i=0;i<this->instance->nbphot;i++){
        idx = rand()%this->instance->nbphot;
        while(mark[idx]==true)
            idx++%this->instance->nbphot;
        if(this->instance->V[idx].ori=='H'){
            hori_slide.p1 = idx;
            mark[idx]=true;
            this->sol->vsol.push_back(hori_slide);
        }
        else if(this->instance->V[idx].ori=='V'){
            if(verti_slide.p1==-1){
                verti_slide.p1 = idx;
                mark[idx]=true;
            }else{
                verti_slide.p2 = idx;
                mark[idx]=true;
                this->sol->vsol.push_back(verti_slide);
                verti_slide.p1=-1;
                verti_slide.p2=-1;
            }
        }
    }

    this->sol->nbslides = this->sol->vsol.size();
    return this->sol;
}
