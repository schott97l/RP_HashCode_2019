#ifndef __INSTANCE_H__
#define __INSTANCE_H__

#include<iostream>
#include<string>
#include<list>
#include<vector>

using namespace std;

class Photo{
    public:
        char ori;
        list<string> tags;
};

class Instance{
    public:
        int nbphot;
        vector<Photo> V;

        Instance(istream & f, double p);
        void aff(ostream &o);
        void sort_tags();

        int search_closer(list<string>&l,vector<bool> &mark);
        int search_closer(list<string>&l,vector<bool> &mark,int start, int length);
        int search_closerV(list<string>&l,vector<bool> &mark);
        int search_closerV(list<string>&l,vector<bool> &mark,int start, int length);

};

class Slide{
    public:
        int p1,p2;  // p2==-1 if no photo
};

class Sol{
    public:
        Instance *I;
        int nbslides;
        vector<Slide> vsol;
        vector<int> eval_vect;
        int evaluation;

        Sol();
        Sol(Instance *II,istream & f);
        void aff(ostream &o);
        int eval();
        int eval_transition(int idx1, int idx2);
        int eval_slide(int idx);
        void swap_slides(int idx1,int idx2);
        void swap_verticals(int idx1, int photo_idx1, int idx2, int photo_idx2);
        Sol * deep_copy();

};


#endif
