#include"Greedy.h"

using namespace std;

Greedy::Greedy(int length){
    this->length = length;
}

Sol * Greedy::solve(){
    int value;
    bool done;
    int idx_pred;

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
        idx = instance->search_closerV(instance->V[idx].tags,mark,idx,length);
        if (idx!=-1)
            mark[idx] = true;
        slide.p2=idx;
        sol->vsol.push_back(slide);
        slide.p2=-1;
        i=2;
    }

    sol->eval_vect.push_back(0);
    int j=1;

    for(i;i<instance->nbphot;i++){
        done=false;
        if (idx!=-1){
            idx = instance->search_closer(instance->V[idx].tags,mark,idx,length);
            if (idx==-1){
                for (int j=0;j<instance->nbphot;j++){
                    if(mark[j]==false){
                        idx=j;
                        break;
                    }
                }
            }
        } else {
            for (int j=0;j<instance->nbphot;j++){
                if(mark[j]==false){
                    idx=j;
                    break;
                }
            }
            if (j==instance->nbphot)
                break;
        }
        slide.p1=idx;
        if (instance->V[idx].ori == 'H'){
            mark[idx] = true;
            sol->vsol.push_back(slide);
            done = true;
        } else {
            idx_pred = idx;
            mark[idx_pred] = true;
            idx = instance->search_closerV(instance->V[idx].tags,mark,idx,length);
            if (idx!=-1)
            {
                mark[idx] = true;
                slide.p2=idx;
                sol->vsol.push_back(slide);
                slide.p2=-1;
                i++;
                done = true;
            }else
                mark[idx_pred] = false;
        }
        if (done)
        {
            value = sol->eval_transition(j-1,j);
            sol->eval_vect.push_back(value);
            sol->evaluation += value;
            j++;
        }
    }

    sol->nbslides = sol->vsol.size();
    sol->eval();

    if (is_file_exist("greedy_evals.csv")){
        ofstream outfile;
        outfile.open("greedy_evals.csv", std::ios_base::app);
        if (!outfile.fail())
            outfile << length << "," << sol->evaluation << endl;
    }

    return sol;
}
