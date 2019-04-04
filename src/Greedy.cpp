#include"Greedy.h"

using namespace std;

Greedy::Greedy(){
}

void Greedy::solve(){

    this->sol = Sol();
    this->sol.I = this->instance;
    vector<Slide> slides;
    Slide slide;
    slide.p1=-1;
    slide.p2=-1;

    std::vector<bool> mark;
    mark.resize(this->instance->nbphot);
    for (int j=0;j<this->instance->nbphot;j++)
        mark[j]=false;

    int i = 0;

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

    this->sol.vsol = slides;
    this->sol.nbslides = this->sol.vsol.size();

}
