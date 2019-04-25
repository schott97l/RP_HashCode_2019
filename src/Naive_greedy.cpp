#include"Naive_greedy.h"

using namespace std;

Naive_greedy::Naive_greedy(){
}

Sol * Naive_greedy::solve(){

    sol = new Sol();
    sol->I = instance;
    vector<Slide> slides;
    Slide slide;
    slide.p1=-1;
    slide.p2=-1;

    std::vector<bool> mark;
    mark.resize(instance->nb_phot);
    for (int j=0;j<instance->nb_phot;j++)
        mark[j]=false;

    int i = 0;

    srand(time(0));
    int idx = rand() % this->instance->nbphot;
    mark[idx] = true;
    slide.p1=idx;
    if (this->instance->V[idx].ori = 'H'){
        slides.push_back(slide);
        i++;
    } else {
        idx = this->instance->search_closerV(this->instance->V[idx].tags,mark);
        mark[idx] = true;
        slide.p2=idx;
        slides.push_back(slide);
        slide.p2=-1;
        i++;
    }

    for(;i<this->instance->nbphot;i++){
        idx = this->instance->search_closer(this->instance->V[idx].tags,mark);
        mark[idx] = true;
        slide.p1=idx;
        i++;
        if (this->instance->V[idx].ori = 'H'){
            slides.push_back(slide);
        } else {
            idx = this->instance->search_closerV(this->instance->V[idx].tags,mark);
            mark[idx] = true;
            slide.p2=idx;
            slides.push_back(slide);
            slide.p2=-1;
            i++;
        }
    }

    this->sol->vsol = slides;
    this->sol->nbslides = this->sol->vsol.size();

    return this->sol;
}
