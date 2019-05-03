#include"Stoch_descent.h"

using namespace std;

Stoch_descent::Stoch_descent(int nb_iter, int nb_neighbours, int timelimit){
    this->nb_iter = nb_iter;
    this->nb_neighbours = nb_neighbours;
    this->timelimit = timelimit;
}

Sol * Stoch_descent::solve(){
    int r, j, idx, photo_idx;
    int timestep=0;

    time_t endwait;
    time_t last = 0;
    time_t current = time(NULL);
    time_t seconds = timelimit; // end loop after this time has elapsed
    endwait = current + seconds;

    ofstream outfile;
    string file_name = "stoch_evals_" + itos(nb_neighbours) + ".csv";
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
            timestep++;
            last = time(NULL);
            if (is_file_exist(file_name.c_str()))
                outfile << timelimit*0.05*timestep << "," << sol->evaluation << endl;
            if(timestep>=20)
                break;
        }

        r = rand()%2;
        if (r==0){
            idx = rand() % sol->nbslides;
            sol = swap_slides_neighbour(idx);
        }else{
            photo_idx = rand()%2;
            j = 0;
            while (sol->vsol[idx].p2==-1){
                idx = rand() % sol->nbslides;
                j++;
                if(j>=instance->nbphot){
                    idx=-1;
                    break;
                }
            }
            if(idx!=-1)
                sol = swap_verticals_neighbour(idx,photo_idx);
        }
    }
    sol->eval();
    return this->sol;
}

Sol * Stoch_descent::swap_slides_neighbour(int idx1){
    vector<tuple<int,double>> neigh_scores;
    int argmax =0;
    int max = sol->evaluation;
    int idx;
    double score;

    for (int i=0;i<nb_neighbours;i++){
        int idx2 = rand() % sol->nbslides;
        sol->swap_slides(idx2,idx1);
        neigh_scores.push_back(make_tuple(idx2,sol->evaluation));
        sol->swap_slides(idx2,idx1);
    }
    for(int i=0;i<nb_neighbours;i++){
        score = get<1>(neigh_scores[i]);
        if(score>max){
            max=score;
            argmax=get<0>(neigh_scores[i]);
        }
    }
    if (max>sol->evaluation){
        sol->swap_slides(idx1,argmax);
    }
    return sol;
}

Sol * Stoch_descent::swap_verticals_neighbour(int idx1, int photo_idx1){
    vector<tuple<int,int,double>> neigh_scores;
    int argmax=0;
    int photo_argmax =0;
    double max = sol->evaluation;
    int idx;
    double score;
    int previous_eval=0;
    int next_eval=0;
    int k=0;

    for (int i=0;i<nb_neighbours;i++){
        int idx2 = rand() % sol->nbslides;
        int photo_idx2 = rand()%2 +1;
        if (sol->vsol[idx2].p2==-1){
            i--;
        } else {
            sol->swap_verticals(idx1,photo_idx1,idx2,photo_idx2);
            neigh_scores.push_back(make_tuple(idx2,photo_idx2,sol->evaluation));
            sol->swap_verticals(idx2,photo_idx2,idx1,photo_idx1);
        }
        k++;
        if (k>=instance->nbphot)
            break;
    }
    for(int i=0;i<neigh_scores.size();i++){
        score = get<2>(neigh_scores[i]);
        if(score>max){
            max=score;
            argmax=get<0>(neigh_scores[i]);
            photo_argmax=get<1>(neigh_scores[i]);
        }
    }
    if (max>sol->evaluation){
        sol->swap_verticals(idx1,photo_idx1,argmax,photo_argmax);
    }
    return sol;
}
