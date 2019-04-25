#include"Hori_verti.h"

using namespace std;

Hori_verti::Hori_verti(){
}

Sol * Hori_verti::solve(){
    int i;

    sol = new Sol;
    sol->I = instance;
    Slide slide;
    slide.p1=-1;
    slide.p2=-1;
    
    cout << "hori " << instance->nb_hori << endl;
    cout << "verti " << instance->nb_verti << endl;

    for(i=0;i<instance->nb_hori;i++){
        slide.p1 = instance->hori_V[i].index;
        slide.tags =  instance->hori_V[i].tags;
        sol->vsol.push_back(slide);
    }
    for(i=0;i<instance->nb_verti;i++){
        slide.p1 = instance->verti_V[i].index;
        slide.tags =  instance->verti_V[i].tags;
        i++;
        slide.p2 = instance->verti_V[i].index;
        if (i>0)
            concat_sorted_without_double(instance->verti_V[i-1].tags,
                    instance->verti_V[i].tags,slide.tags);
        sol->vsol.push_back(slide);
        for (string s : slide.tags)
            cout << s << endl;
    }
    
    sol->nb_slides = sol->vsol.size();
    cout << "size " << sol->nb_slides << endl;
    sol->compute_eval_vector();

    return sol;
}
