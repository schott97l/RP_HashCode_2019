#include"Annealing.h"

using namespace std;

Annealing::Annealing(int nb_iter, int nb_neighbours, int timelimit){
    this->nb_iter = nb_iter;
    this->nb_neighbours = nb_neighbours;
    this->timelimit = timelimit;
}

Sol * Annealing::solve(){
    int r, j, idx, photo_idx;
    int timestep=0;
    double T;

    time_t endwait;
    time_t last = 0;
    time_t start = time(NULL);
    time_t current = time(NULL);
    time_t seconds = timelimit; // end loop after this time has elapsed
    endwait = current + seconds;

    ofstream outfile;
    string file_name = "annealing_evals.csv";
    if (is_file_exist(file_name.c_str())){
        outfile.open(file_name, std::ios_base::app);
    }

    if (sol==NULL){
        Random rd;
        rd.load(instance);
        sol = rd.solve();
    }

    srand(time(0));
    idx = rand() % sol->nbslides;

    //stoch descent
    for (int i=0;i<nb_iter;i++){

        current = time(NULL);
        if (current-last >= timelimit*0.05){
            //sol->eval();
            cout << sol->evaluation << endl;
            cout << "T " << T << endl;
            timestep++;
            last = time(NULL);
            if (is_file_exist(file_name.c_str()))
                outfile << timelimit*0.05*timestep << "," << sol->evaluation << endl;
            if(timestep>=20)
                break;
        }
        
        T = 1/((1-min( (double(nb_iter)-double(i))/double(nb_iter) , (double(timelimit)-(double(current)-double(start)))/double(timelimit) ))*10);
        if(T<0)
            T=0;
        else if (T>100)
            T=100;

        r = rand()%2;
        if (r==0){
            idx = rand() % sol->nbslides;
            sol = swap_slides_neighbour(idx,T);
        }else{
            photo_idx = rand()%2;
            j = 0;
            while (sol->vsol[idx].p2==-1 && j<instance->nbphot){
                idx = rand() % sol->nbslides;
                j++;
            }
            if(j<instance->nbphot)
                sol = swap_verticals_neighbour(idx,photo_idx,T);
        }
    }
    sol->eval();
    return this->sol;
}

Sol * Annealing::swap_slides_neighbour(int idx1, double T){
    int idx;
    int eval;
    double r = randfrom(0,1);
    double delta;

    int idx2 = rand() % sol->nbslides;
    sol->swap_slides(idx1,idx2);
    eval = sol->evaluation;
    sol->swap_slides(idx1,idx2);
    delta = eval - sol->evaluation;
    if ( delta > 0 || r < exp(delta/T)){
        sol->swap_slides(idx1,idx2);
    }
    return sol;
}

Sol * Annealing::swap_verticals_neighbour(int idx1, int photo_idx1, double T){
    int idx;
    double eval=0;
    double delta;
    int k=0;
    int photo_idx2 = rand()%2;
    int idx2 = 0;
    double r = randfrom(0,1);
    double e;

    do{
        idx2 = rand() % sol->nbslides;
        k++;
    }while (sol->vsol[idx2].p2==-1 && k<instance->nbphot);

    if(k<instance->nbphot){
        sol->swap_verticals(idx1,photo_idx1,idx2,photo_idx2);
        eval = sol->evaluation;
        sol->swap_verticals(idx2,photo_idx2,idx1,photo_idx1);
        delta = eval - sol->evaluation;
        e=exp(delta/T);
        if ( delta > 0 || r < e){
            sol->swap_verticals(idx1,photo_idx1,idx2,photo_idx2);
        }
    }
    return sol;
}
