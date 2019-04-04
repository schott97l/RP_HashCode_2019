#include"Solver.h"
#include"Hori_verti.h"
#include"Greedy.h"
#include"Stoch_descent.h"
#include"Genetic.h"
#include"Ilp.h"

using namespace std;

int main(int argc, char **argv){

    string name_solver, name_instance, name_solution;
    double percent;

    if(argc!=5){
        cerr<<"usage: "<<argv[0]<<" <file percent sol>"<<endl;
        cerr<<"    Solver [ hori_verti | greedy | stoch_descent | genetic | ilp ]"<<endl;
        cerr<<"    Instance file of the Google Hash Code PhotoSlideShow (*.txt)"<<endl;
        cerr<<"    Percent of the nuber of photo to be read"<<endl;
        cerr<<"    Solution file to check"<<endl;
        return 1;
    }

    name_solver=argv[1];
    name_instance=argv[2];
    percent=atof(argv[3]);
    name_solution=argv[4];

    ifstream fic(name_instance.c_str());
    if (fic==NULL){
        cerr<<"file "<<name_instance<<" "<<" not found"<<endl;
        return 1;
    }

    Instance I(fic,percent);
    fic.close();
    I.sort_tags();

    ofstream fic2(name_solution.c_str());
    if (fic2==NULL){
        cerr<<"file "<<name_solution<<" "<<" not found"<<endl;
        return 1;
    }

    Solver * solver= NULL;

    if (name_solver.compare("hori_verti") == 0)
        solver = new Hori_verti;
    else if (name_solver.compare("greedy") == 0)
        solver = new Greedy;
    else if (name_solver.compare("stoch_descent") == 0)
        solver = new Stoch_descent;
    else if (name_solver.compare("genetic") == 0)
        solver = new Genetic;
    else if (name_solver.compare("ilp") == 0)
        solver = new Ilp;
    else
        solver = new Hori_verti;

    solver->load(&I);
    solver->solve();
    solver->save(fic2);

    cout << "eval : " << solver->eval() << endl;

    return 0;
}
