#include"Ilp.h"

using namespace std;

Ilp::Ilp(){
}

void Ilp::solve(){

    this->sol = Sol();
    this->sol.I = this->instance;
    vector<Slide> hori_vector;
    vector<Slide> verti_vector;
    Slide verti_slide;
    Slide hori_slide;
    verti_slide.p1=-1;
    verti_slide.p2=-1;
    hori_slide.p1=-1;
    hori_slide.p2=-1;

    for(int i=0;i<this->instance->nbphot;i++){
        if (this->instance->V[i].ori == 'V'){
            if (verti_slide.p1==-1){
                verti_slide.p1=i;
            }else{
                verti_slide.p2=i;
                verti_vector.push_back(verti_slide);
                verti_slide.p1=-1;
                verti_slide.p2=-1;
            }
        }else{
            hori_slide.p1=i;
            hori_vector.push_back(hori_slide);
        }
    }

    this->sol.vsol = hori_vector;
    this->sol.vsol.insert( this->sol.vsol.end(), verti_vector.begin(), verti_vector.end() );
    this->sol.nbslides = this->sol.vsol.size();

}
