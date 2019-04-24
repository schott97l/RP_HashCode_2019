#include"Genetic.h"

using namespace std;

Genetic::Genetic(int nb_generation, int population_size){
    this->nb_generation = nb_generation;
    this->population_size = population_size;
}

Sol * Genetic::solve(){
    int i,j,argmax;
    Sol * tmp_sol;
    double max,eval;
    Sol * parent_A, * parent_B;
    vector<Sol*> new_generation;

    srand(time(0));

    Random r;
    r.load(this->instance);
    
    for(j=0;j<this->population_size;j++){
        tmp_sol = r.solve();
        this->population.push_back(tmp_sol);
    }

    for(i=0;i<this->nb_generation;i++){

        max=0;
        this->evals.clear();
        for(j=0;j<this->population_size;j++){
            eval = this->population[j]->eval();
            this->evals.push_back(eval);
            if(eval>max)
                max=eval;
        }
        cout << "best : " << max << endl;

        new_generation.clear();

        for(j=0;j<this->population_size;j++){
            parent_A = select(this->population, this->evals);
            parent_B = select(this->population, this->evals);
            while(parent_A==parent_B){
                parent_B = select(this->population, this->evals);
            }
            new_generation.push_back(mutation(cross_over(parent_A,parent_B)));
        }
        population.clear();
        population = new_generation;
    }

    max=-1;
    argmax=-1;

    for(i=0;i<this->population_size;i++){
        if(evals[i]>max){
            max=evals[i];
            argmax=i;
        }
    }

    return this->population[argmax];
}

vector<double> Genetic::softmax(vector<double> evals){
    int j;
    double sum_exp = 0;
    vector<double> soft_evals;
    for(j=0;j<this->population_size;j++){
        sum_exp += this->evals[j];
    }
    for(j=0;j<this->population_size;j++){
        soft_evals.push_back(this->evals[j]/sum_exp);
    }
    return soft_evals;
}

double randfrom(double min, double max)
{
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

Sol * Genetic::select(vector<Sol*> population, vector<double> evals){
    double r,sum;
    int i;
    vector<double> soft_evals = softmax(evals);
    vector<double> sum_soft_evals;

    sum=0;
    for(i=0;i<evals.size();i++){
        sum += soft_evals[i];
        sum_soft_evals.push_back(sum);
    }

    r = randfrom(0,1);
    for(i=0;i<this->population_size;i++){
        if(sum_soft_evals[i]>r)
            return population[i];
    }

    return population[(int) r*this->population_size];
}

Sol * Genetic::cross_over(Sol * parent_A, Sol * parent_B){

    int r = rand() % parent_A->nbslides;
    Sol * child = new Sol();
    child->I = parent_A->I;
    Slide tmp_slide;
    tmp_slide.p1=-1;
    tmp_slide.p2=-1;

    vector<bool> mark;
    mark.resize(this->instance->nbphot);
    for (int i=0;i<this->instance->nbphot;i++)
        mark[i]=false;

    for(int i=0;i<r;i++){
        child->vsol.push_back(parent_A->vsol[i]);
        mark[parent_A->vsol[i].p1]=true;
        if(parent_A->vsol[i].p2!=-1)
            mark[parent_A->vsol[i].p2]=true;
    }
    for(int i=0;i<parent_B->nbslides;i++){
        if(parent_B->vsol[i].p2==-1)
            if (mark[parent_B->vsol[i].p1]==false)
                child->vsol.push_back(parent_B->vsol[i]);
        else if(parent_B->vsol[i].p2!=-1){
            if (mark[parent_B->vsol[i].p1]==false && mark[parent_B->vsol[i].p2]==false)
                child->vsol.push_back(parent_B->vsol[i]);
            else if (mark[parent_B->vsol[i].p1]==true && mark[parent_B->vsol[i].p2]==false)
                if(tmp_slide.p1==-1)
                    tmp_slide.p1 = mark[parent_B->vsol[i].p2];
                else {
                    tmp_slide.p2 = mark[parent_B->vsol[i].p2];
                    child->vsol.push_back(tmp_slide);
                    tmp_slide.p1=-1;
                    tmp_slide.p2=-1;
                }
            else if (mark[parent_B->vsol[i].p1]==false && mark[parent_B->vsol[i].p2]==true)
                if(tmp_slide.p1==-1)
                    tmp_slide.p1 = mark[parent_B->vsol[i].p1];
                else {
                    tmp_slide.p2 = mark[parent_B->vsol[i].p1];
                    child->vsol.push_back(tmp_slide);
                    tmp_slide.p1=-1;
                    tmp_slide.p2=-1;
                }
        }
    }
    child->nbslides = child->vsol.size();
    return child;
}

Sol * Genetic::mutation(Sol * solution){
    int r, idx1, idx2, photo_idx1, photo_idx2;
    idx1 = -1;
    idx2 = -1;
    r = rand()%2;
    if(r==0){
        idx1 = rand() % solution->nbslides;
        idx2 = rand() % solution->nbslides;
        solution->swap_slides(idx1,idx2);
    } else {
        idx1 = rand() % solution->nbslides;
        while(solution->vsol[idx1].p2==-1)
            idx1 = rand() % solution->nbslides;
        idx2 = rand() % solution->nbslides;
        while(solution->vsol[idx2].p2==-1)
            idx2 = rand() % solution->nbslides;
        photo_idx1 = rand()%2;
        photo_idx2 = rand()%2;
        solution->swap_verticals(idx1,photo_idx1,idx2,photo_idx2);
    }
    return solution;
}
