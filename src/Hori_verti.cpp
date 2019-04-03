#include"Hori_verti.h"

Hori_verti::Hori_verti(Instance * instance){
    this->instance = instance;
}

void Hori_verti::solve(){

    this->sol = Sol();
    this->sol.I = this->instance;
    vector<Slide> hori_vector;
    vector<Slide> verti_vector;
    Slide verti_slide;
    Slide hori_slide;
    verti_slide.p1=-1;
    verti_slide.p2=-1;
    hori_slide.p1=-1;
    hori_slide.p2=-1;

    for(int i=0;i<this->instance->nbphot;i++){
        if (this->instance->V[i].ori == 'V'){
            if (verti_slide.p1==-1){
                verti_slide.p1=i;
            }else{
                verti_slide.p2=i;
                verti_vector.push_back(verti_slide);
                verti_slide.p1=-1;
                verti_slide.p2=-1;
            }
        }else{
            hori_slide.p1=i;
            hori_vector.push_back(hori_slide);
        }
    }

    this->sol.vsol = hori_vector;
    this->sol.vsol.insert( this->sol.vsol.end(), verti_vector.begin(), verti_vector.end() );
    this->sol.nbslides = this->sol.vsol.size();

}

void Hori_verti::save(ostream &o){
    this->sol.aff(o);
}

double Hori_verti::eval(){
    return this->sol.eval();
}

int main(int argc, char **argv){

    string nameext, namesol;
    double percent;

    if(argc!=4){
        cerr<<"usage: "<<argv[0]<<" <file percent sol>"<<endl;
        cerr<<"    Instance file of the Google Hash Code PhotoSlideShow (*.txt)"<<endl;
        cerr<<"    Percent of the nuber of photo to be read"<<endl;
        cerr<<"    Solution file to check"<<endl;
        return 1;
    }

    nameext=argv[1];
    percent=atof(argv[2]);
    namesol=argv[3];

    ifstream fic(nameext.c_str());
    if (fic==NULL){
        cerr<<"file "<<nameext<<" "<<" not found"<<endl;
        return 1;
    }

    Instance I(fic,percent);
    fic.close();
    I.sort_tags();

    ofstream fic2(namesol.c_str());
    if (fic2==NULL){
        cerr<<"file "<<namesol<<" "<<" not found"<<endl;
        return 1;
    }

    Hori_verti Model(&I);
    Model.solve();
    Model.save(fic2);

    cout << "eval : " << Model.eval() << endl;

    return 0;
}
