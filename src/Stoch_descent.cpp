#include"Stoch_descent.h"
#include<tuple>

using namespace std;

Stoch_descent::Stoch_descent(int nb_iter, int nb_neighbours,int greedy_length){
    this->nb_iter = nb_iter;
    this->nb_neighbours = nb_neighbours;
    this->greedy_length = greedy_length;
}

Stoch_descent::Stoch_descent(int nb_iter, int nb_neighbours){
    this->nb_iter = nb_iter;
    this->nb_neighbours = nb_neighbours;
}

Sol * Stoch_descent::solve(){
    int r, idx, photo_idx;

    if (this->sol==NULL){
        Greedy greedy(this->greedy_length);
        greedy.load(this->instance);
        this->sol = greedy.solve();
    }

    srand(time(0));
    //stoch descent
    for (int i=0;i<this->nb_iter;i++){
        r = rand()%2;
        if (r==0){
            idx = rand() % this->sol->nbslides;
            this->sol = swap_slides_neighbour(idx);
        }else{
            photo_idx = rand()%2;
            while (this->sol->vsol[idx].p2==-1){
                idx = rand() % this->sol->nbslides;
            }
            this->sol = swap_verticals_neighbour(idx,photo_idx);
        }
    }

    return this->sol;
}

Sol * Stoch_descent::swap_slides_neighbour(int idx1){
    vector<tuple<int,double>> neigh_scores;
    int argmax =-1;
    int max = this->sol->eval();
    int idx;
    double score;
    Sol * new_sol = this->sol->deep_copy();

    for (int i=0;i<this->nb_neighbours;i++){
        int idx2 = rand() % this->sol->nbslides;
        new_sol->swap_slides(idx1,idx2);
        neigh_scores.push_back(make_tuple(idx2,new_sol->eval()));
        new_sol->swap_slides(idx2,idx1);
    }
    for(int i=0;i<this->nb_neighbours;i++){
        score = get<1>(neigh_scores[i]);
        if(score>max){
            max=score;
            argmax=get<0>(neigh_scores[i]);
        }
    }
    cout << max << endl;
    if (max>this->sol->eval())
        this->sol->swap_slides(idx1,argmax);
    return this->sol;
}

Sol * Stoch_descent::swap_verticals_neighbour(int idx1, int photo_idx1){
    vector<tuple<int,int,double>> neigh_scores;
    int argmax =-1;
    int photo_argmax =-1;
    double max = this->sol->eval();
    int idx;
    double score;
    Sol * new_sol = this->sol->deep_copy();

    for (int i=0;i<this->nb_neighbours;i++){
        int idx2 = rand() % this->sol->nbslides;
        int photo_idx2 = rand()%2 +1;
        if (this->sol->vsol[idx2].p2==-1){
            i--;
            continue;
        }
        new_sol->swap_verticals(idx1,photo_idx1,idx2,photo_idx2);
        neigh_scores.push_back(make_tuple(idx2,photo_idx2,new_sol->eval()));
        new_sol->swap_verticals(idx2,photo_idx2,idx1,photo_idx1);
    }
    for(int i=0;i<neigh_scores.size();i++){
        score = get<2>(neigh_scores[i]);
        if(score>max){
            max=score;
            argmax=get<0>(neigh_scores[i]);
            photo_argmax=get<1>(neigh_scores[i]);
        }
    }
    cout << max << endl;
    if (max>this->sol->eval())
        this->sol->swap_verticals(idx1,photo_idx1,argmax,photo_argmax);
    return this->sol;
}
