#ifndef __INSTANCE_H__
#define __INSTANCE_H__

#include<iostream>
#include<string>
#include<list>
#include<vector>

using namespace std;

class Photo{
    public:
        int index;
        char ori;
        list<string> tags;
};

class Slide{
    public:
        int p1,p2;  // p2==-1 if no photo
        list<string> tags;
};

class Instance{
    public:
        int nb_phot;
        vector<Photo> V;
        vector<bool> mark;
        int nb_hori;
        vector<Photo> hori_V;
        vector<bool> hori_mark;
        int nb_verti;
        vector<Photo> verti_V;
        vector<bool> verti_mark;

        Instance(istream & f, double p);
        void aff(ostream &o);

        Slide search_best_next_slide(const Slide &s);
        Slide search_best_next_slide(const Slide &s,int start, int length);
};

int min(int a, int b, int c);
int eval_transition(const Slide &s1, const Slide &s2);
void concat_sorted_without_double(const list<string>&l1,const list<string>&l2, list<string>&l);

class Sol{
    public:
        Instance *I;
        int nb_slides;
        vector<Slide> vsol;
        vector<int> veval;

        Sol();
        Sol(Instance *II,istream & f);
        void aff(ostream &o);
        int eval();
        void compute_eval_vector();
        Sol * deep_copy();
        void swap_slides(int idx1,int idx2);
        void swap_verticals(int idx1, int photo_idx1, int idx2, int photo_idx2);
        void eval_swap_slides(int idx1, int idx2);
};


#endif
