#include"Genetic.h"

using namespace std;

Genetic::Genetic(int nb_generation, int population_size, int timelimit){
    this->nb_generation = nb_generation;
    this->population_size = population_size;
    this->timelimit = timelimit;
}

Sol * Genetic::solve(){
    int i,j,k,argmax;
    int random_int;
    Sol * tmp_sol;
    Sol * best;
    double max,eval;
    Sol * parent_A, * parent_B;
    vector<Sol*> new_generation;

    int timestep=0;

    time_t endwait;
    time_t last = 0;
    time_t current = time(NULL);
    time_t seconds = timelimit; // end loop after this time has elapsed
    endwait = current + seconds;

    ofstream outfile;
    string file_name = "genetic_evals_" + itos(population_size) + ".csv";
    if (is_file_exist(file_name.c_str())){
        outfile.open(file_name, std::ios_base::app);
    }


    srand(time(0));
    Random r;
    r.load(this->instance);

    sol = r.solve();
    best = r.solve();
    
    for(j=0;j<this->population_size;j++){
        tmp_sol = r.solve();
        population.push_back(tmp_sol);
    }

    for(i=0;i<this->nb_generation;i++){

        max=0;
        for(j=0;j<population_size;j++){
            eval = population[j]->evaluation;
            if(eval>max){
                max=eval;
                sol = population[j];
            }
            if(eval>best->evaluation)
                best = population[j];
        }

        current = time(NULL);
        if (current-last >= timelimit*0.05){
            max=0;
            for(j=0;j<population_size;j++){
                eval = population[j]->evaluation;
                cout << eval << endl;
                if(eval>max){
                    max=eval;
                    sol = population[j];
                }
                if(eval>best->evaluation)
                    best = population[j];
            }

            cout << "eval " << best->evaluation << endl;
            timestep++;
            last = time(NULL);
            if (is_file_exist(file_name.c_str()))
                outfile << timelimit*0.05*timestep << "," << best->evaluation << endl;
            if(timestep>=20)
                break;
        }

        new_generation.clear();
        for(j=0;j<this->population_size;j++){

            parent_A = select();
            //tmp_sol = parent_A->deep_copy();
            k=0;
            do{
                parent_B = select();
                k++;
            }
            while(parent_A == parent_B && k<population_size*2);

            if (k<population_size*2)
                tmp_sol = cross_over(parent_A,parent_B);
            else
                tmp_sol = parent_A->deep_copy();
            tmp_sol = mutation(tmp_sol);
            new_generation.push_back(tmp_sol);
        }
        for(j=0;j<int(this->population_size*0.1);j++){
            tmp_sol = r.solve();
            new_generation[j]=tmp_sol;
        }
        for(j=0;j<this->population_size;j++){
            population[j] = new_generation[j];
        }
    }
    max=0;
    for(j=0;j<population_size;j++){
        eval = population[j]->evaluation;
        if(eval>max){
            max=eval;
            sol = population[j];
        }
        if(eval>best->evaluation)
            best = population[j];
    }

    cout << "final : " << best->eval() << endl;
    sol = best;

    return sol;
}

vector<double> Genetic::softmax(){
    int j;
    double val;
    double sum = 0;
    double exp_sum = 0;
    vector<double> min_evals;
    vector<double> std_evals;
    vector<double> soft_evals;
    double min=9999999;

    for(j=0;j<this->population_size;j++){
        if (population[j]->evaluation<min)
            min = population[j]->evaluation;
    }
    for(j=0;j<this->population_size;j++){
        val = population[j]->evaluation-min;
        min_evals.push_back(val);
    }
    for(j=0;j<this->population_size;j++){
        sum += min_evals[j];
    }
    for(j=0;j<this->population_size;j++){
        val = min_evals[j]/sum;
        std_evals.push_back(val);
    }
    /*
    for(j=0;j<this->population_size;j++){
        exp_sum += exp(std_evals[j]);
    }
    for(j=0;j<this->population_size;j++){
        val = exp(std_evals[j])/exp_sum;
        cout << val << endl;
        soft_evals.push_back(val);
    }*/
    return std_evals;
}

Sol * Genetic::select(){
    double r,sum;
    int i;
    vector<double> soft_evals = softmax();
    vector<double> sum_soft_evals;

    sum=0;
    for(i=0;i<population_size;i++){
        sum += soft_evals[i];
        sum_soft_evals.push_back(sum);
    }

    r = randfrom(0,1);
    for(i=0;i<this->population_size;i++){
        if(sum_soft_evals[i]>r){
            return population[i];
        }
    }

    return population[(int) r*this->population_size];
}

Sol * Genetic::cross_over(Sol * parent_A, Sol * parent_B){
    if (parent_A==parent_B)
        return parent_A;

    int k=0;
    int r = rand() % (parent_A->nbslides-1) + 1;
    int tmp_eval;
    Sol * child = new Sol();
    child->I = parent_A->I;
    child->eval_vect.clear();
    child->evaluation=0;
    child->vsol.clear();
    Slide tmp_slide;
    tmp_slide.p1=-1;
    tmp_slide.p2=-1;

    vector<bool> mark;
    mark.resize(this->instance->nbphot);
    for (int i=0;i<this->instance->nbphot;i++)
        mark[i]=false;

    for(int i=0;i<r;i++){
        child->vsol.push_back(parent_A->vsol[i]);
        k++;
        tmp_eval = parent_A->eval_vect[i];
        child->eval_vect.push_back(tmp_eval);
        child->evaluation+=tmp_eval;
        mark[parent_A->vsol[i].p1]=true;
        if(parent_A->vsol[i].p2!=-1)
            mark[parent_A->vsol[i].p2]=true;
    }
    for(int i=0;i<parent_B->nbslides;i++){
        if(parent_B->vsol[i].p2==-1){
            if (mark[parent_B->vsol[i].p1]==false){
                child->vsol.push_back(parent_B->vsol[i]);
                tmp_eval = child->eval_transition(k-1,k);
                child->eval_vect.push_back(tmp_eval);
                child->evaluation += tmp_eval;
                k++;
                mark[parent_B->vsol[i].p1]=true;
            }
        } else {
            if (mark[parent_B->vsol[i].p1]==false && mark[parent_B->vsol[i].p2]==false){
                child->vsol.push_back(parent_B->vsol[i]);
                tmp_eval = child->eval_transition(k-1,k);
                child->eval_vect.push_back(tmp_eval);
                child->evaluation += tmp_eval;
                k++;
                mark[parent_B->vsol[i].p1]=true;
                mark[parent_B->vsol[i].p2]=true;
            } else if (mark[parent_B->vsol[i].p1]==true && mark[parent_B->vsol[i].p2]==false){
                if(tmp_slide.p1==-1){
                    tmp_slide.p1 = parent_B->vsol[i].p2;
                } else {
                    tmp_slide.p2 = parent_B->vsol[i].p2;
                    child->vsol.push_back(tmp_slide);
                    tmp_eval = child->eval_transition(k-1,k);
                    child->eval_vect.push_back(tmp_eval);
                    child->evaluation += tmp_eval;
                    k++;
                    tmp_slide.p1=-1;
                    tmp_slide.p2=-1;
                }
            } else if (mark[parent_B->vsol[i].p1]==false && mark[parent_B->vsol[i].p2]==true){
                if(tmp_slide.p1==-1){
                    tmp_slide.p1 = parent_B->vsol[i].p1;
                } else {
                    tmp_slide.p2 = parent_B->vsol[i].p1;
                    child->vsol.push_back(tmp_slide);
                    tmp_eval = child->eval_transition(k-1,k);
                    child->eval_vect.push_back(tmp_eval);
                    child->evaluation += tmp_eval;
                    k++;
                    tmp_slide.p1=-1;
                    tmp_slide.p2=-1;
                }
                mark[parent_B->vsol[i].p1]=true;
            }
        }
    }
    child->nbslides = child->vsol.size();
    return child;
}

Sol * Genetic::mutation(Sol * solution){
    int r,idx,photo_idx,j;
    idx = rand() % solution->nbslides;
    if (solution->vsol[idx].p2==-1){
        solution = swap_slides_neighbour(solution,idx);
    }else{
        photo_idx = rand()%2;
        solution = swap_verticals_neighbour(solution,idx,photo_idx);
    }
    return solution;
}

Sol * Genetic::swap_slides_neighbour(Sol * solution, int idx1){
    int idx2;
    int k=0;
    idx2 = rand() % solution->nbslides;
    solution->swap_slides(idx2,idx1);
    return solution;
}

Sol * Genetic::swap_verticals_neighbour(Sol * solution,int idx1, int photo_idx1){
    int k=0;
    int idx2;
    int photo_idx2;
    do{
        idx2 = rand() % solution->nbslides;
        k++;
    }while(solution->vsol[idx2].p2==-1 && k<instance->nbphot);
    if(k<instance->nbphot){
        photo_idx2 = rand()%2 +1;
        solution->swap_verticals(idx1,photo_idx1,idx2,photo_idx2);
    }
    return solution;
}
