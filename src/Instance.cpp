#include"Instance.h"
#include<iostream>
#include<sstream>
#include<fstream>
#include<iomanip>
#include<cstdlib>
#include <vector>

int min(int a, int b, int c){
    if (a<=b)
        if (a<=c) return a;
        else return c; 
    else
        if (c<=b) return c;
        else return b;
}

int eval_transition(const Slide &s1, const Slide &s2){
    int a=0,b=0,c=0;
    list<string>::const_iterator it1,it2;

    it1=s1.tags.begin();
    it2=s2.tags.begin();
    while ((it1!=s1.tags.end())&&(it2!=s2.tags.end())){
        if (*it1<*it2) {
            a++;
            it1++;
        }
        else
            if (*it2<*it1){
                c++;
                it2++;
            }
            else {
                b++;
                it1++;
                it2++;
            }
    }
    if (it1==s1.tags.end())
        while (it2!=s2.tags.end()){
            c++;
            it2++;
        }
    if (it2==s2.tags.end())
        while (it1!=s1.tags.end()){
            a++;
            it1++;
        }        

    return min(a,b,c);
}

void concat_sorted_without_double(const list<string>&l1,const list<string>&l2, list<string>&l){
    l.clear();
    list<string>::const_iterator it1,it2;

    it1=l1.begin();
    it2=l2.begin();
    while ((it1!=l1.end())&&(it2!=l2.end())){
        if (*it1<*it2) {
            l.push_back(*it1);
            it1++;
        }
        else
            if (*it2<*it1){
                l.push_back(*it2);
                it2++;
            }
            else {
                l.push_back(*it1);
                it1++;
                it2++;
            }
    }
    if (it1==l1.end())
        while (it2!=l2.end()){
            l.push_back(*it2);
            it2++;
        }
    if (it2==l2.end())
        while (it1!=l1.end()){
            l.push_back(*it1);
            it1++;
        }        
}


///////////////////////////
///////////////////////////


Instance::Instance(istream &f, double p){
    int i,q,j;
    string s;
    char ori;
    Photo phot;

    f>>nb_phot;
    nb_phot=p*nb_phot/100;

    for (i=0;i<nb_phot;i++){
        phot.index=i;
        f>>phot.ori;
        f>>q;
        for (j=0;j<q;j++){
            f>>s;
            phot.tags.push_back(s);
        }
        phot.tags.sort();
        if (phot.ori=='H')
            hori_V.push_back(phot);
        else
            verti_V.push_back(phot);
        V.push_back(phot);
    }
    nb_hori = hori_V.size();
    nb_verti = verti_V.size();
    nb_phot = V.size();
}

void Instance::aff(ostream &o){
    int i;
    list<string>::const_iterator it;
    o<<nb_phot<<endl;

    for (i=0;i<nb_phot;i++){
        o<<V[i].ori<<" "<<V[i].tags.size()<<" ";
        for (it=V[i].tags.begin();it!=V[i].tags.end();it++)
            o<<*it<<" ";
        o<<endl;
    }
}

Slide Instance::search_best_next_slide(const Slide &s1){
    int i, j, k, max,val,maxi;
    Slide best_slide;
    Slide s2;
    max=-1;
    for (i=0;i<nb_phot;i++){
        if (!(mark[i])){
            j = V[i].index;
            if (V[i].ori == 'H'){
                s2.p1 = hori_V[j].index;
                s2.tags = hori_V[j].tags;
                val=eval_transition(s1,s2);
                if (max<val){
                    max=val;
                    best_slide = s2;
                }
            }
            else{
                for (k=0;k<nb_verti;k++){
                    if (j!=k){
                        if (!(verti_mark[k])){
                            s2.p1 = verti_V[j].index;
                            s2.p2 = verti_V[k].index;
                            concat_sorted_without_double(verti_V[j].tags,
                                    verti_V[k].tags,s2.tags);
                            val=eval_transition(s1,s2);
                            if (max<val){
                                max=val;
                                best_slide = s2;
                            }
                        }
                    }
                }
            }
        }
    }
    return best_slide;
}

Slide Instance::search_best_next_slide(const Slide &s1, int start, int length){
    int i, j, k, count1, count2, max,val,maxi;
    Slide best_slide;
    Slide s2;
    max=-1;
    count1=0;
    for (i=start%nb_phot;i<length;i++){
        if (!(mark[i%nb_phot])){
            count1++;
            if (count1>=length)
                break;
            j = V[i%nb_phot].index;
            if (V[i%nb_phot].ori == 'H'){
                s2.p1 = hori_V[j].index;
                s2.tags = hori_V[j].tags;
                val=eval_transition(s1,s2);
                if (max<val){
                    max=val;
                    best_slide = s2;
                }
            }
            else{
                count2=0;
                for (k=start%nb_phot;k<nb_verti;k++){
                    if (j!=k%nb_phot){
                        if (!(verti_mark[k%nb_phot])){
                            count2++;
                            if (count2>=length)
                                break;
                            s2.p1 = verti_V[j].index;
                            s2.p2 = verti_V[k%nb_phot].index;
                            concat_sorted_without_double(verti_V[j].tags,
                                    verti_V[k%nb_phot].tags,s2.tags);
                            val=eval_transition(s1,s2);
                            if (max<val){
                                max=val;
                                best_slide = s2;
                            }
                        }
                    }
                }
            }
        }
    }
    return best_slide;
}

//////////////////////////////////////////
//////////////////////////////////////////

Sol::Sol(){}

Sol::Sol(Instance *II,istream & f){
    int i;
    std::vector<bool> mark;

    I=II;

    mark.resize(I->nb_phot);
    for (i=0;i<I->nb_phot;i++)
        mark[i]=false;

    f>>nb_slides;
    vsol.resize(nb_slides);

    if (nb_slides<=0){
        cerr<<"Error first line: at least 1 slide needed"<<endl;
        exit(1);
    }

    for (i=0;i<nb_slides;i++){
        f>>vsol[i].p1;
        if ((vsol[i].p1>I->nb_phot)||(mark[vsol[i].p1]==true)){
            cerr<<"Ligne "<<i+1<<" : "<<"wrong number or photo used twice"<<endl;
            exit(1);
        }
        mark[vsol[i].p1]=true;
        if (I->V[vsol[i].p1].ori=='H'){
            vsol[i].p2=-1;
        }else{
            f>>vsol[i].p2;
            if ((vsol[i].p2>I->nb_phot)||(mark[vsol[i].p2]==true)){
                cerr<<"Ligne "<<i+1<<" : "<<"wrong number or photo used twice"<<endl;
                exit(1);
            }
            mark[vsol[i].p2]=true;

        }

    }
}

void Sol::aff(ostream &o){
    int i;
    o<<nb_slides<<endl;
    for (i=0;i<nb_slides;i++){
        o<<vsol[i].p1<<" ";
        if (vsol[i].p2!=-1) o<<vsol[i].p2;
        o<<endl;
    }

}

int Sol::eval(){
    int i;
    int eval=0;
    for (i=0;i<nb_slides;i++){
        eval+=veval[i];
    }
    return eval;
}

void Sol::compute_eval_vector(){
    int i,eval;
    veval.clear();
    veval.push_back(0);
    for (i=1;i<nb_slides;i++){
        eval = eval_transition(vsol[i-1],vsol[i]);
        veval.push_back(eval);
    }
}

void Sol::swap_slides(int idx1, int idx2){
    Slide tmp;
    tmp = this->vsol[idx1];
    this->vsol[idx1] = this->vsol[idx2];
    this->vsol[idx2] = tmp;
}

void Sol::eval_swap_slides(int idx1, int idx2){
    if (idx1>=1)
        veval[idx1] = eval_transition(vsol[idx1-1],vsol[idx1]);
    else
        veval[idx1]=0;
    if (idx1<nb_slides-1)
        veval[idx1+1] = eval_transition(vsol[idx1],vsol[idx1+1]);
    if (idx2>=1)
        veval[idx1] = eval_transition(vsol[idx2-1],vsol[idx2]);
    else
        veval[idx2]=0;
    if (idx2<nb_slides-1)
        veval[idx1] = eval_transition(vsol[idx2],vsol[idx2+1]);
}

void Sol::swap_verticals(int idx1, int photo_idx1, int idx2, int photo_idx2){
    int tmp;
    if(photo_idx1==1){
        tmp = this->vsol[idx1].p1;
        if(photo_idx2==1){
            this->vsol[idx1].p1 = this->vsol[idx2].p1;
            this->vsol[idx2].p1 = tmp;
        }else{
            this->vsol[idx1].p1 = this->vsol[idx2].p2;
            this->vsol[idx2].p2 = tmp;
        }
    }else{
        tmp = this->vsol[idx1].p2;
        if(photo_idx2==1){
            this->vsol[idx1].p2 = this->vsol[idx2].p1;
            this->vsol[idx2].p1 = tmp;
        }else{
            this->vsol[idx1].p2 = this->vsol[idx2].p2;
            this->vsol[idx2].p2 = tmp;
        }
    }
    concat_sorted_without_double(I->V[vsol[idx1].p1].tags,
            I->V[vsol[idx1].p2].tags,vsol[idx1].tags);
    concat_sorted_without_double(I->V[vsol[idx2].p1].tags,
            I->V[vsol[idx2].p2].tags,vsol[idx2].tags);
}

Sol * Sol::deep_copy(){
    Sol * sol = new Sol();
    sol->I = I;
    sol->nb_slides = nb_slides;
    sol->vsol = vsol;
    sol->veval = veval;
    return sol;
}
