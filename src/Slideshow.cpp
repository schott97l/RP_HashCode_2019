#include <unistd.h>

#include"Solver.h"
#include"Hori_verti.h"
#include"Naive_greedy.h"
#include"Greedy.h"
#include"Stoch_descent.h"
#include"Genetic.h"
#include"Ilp.h"

using namespace std;

typedef struct {
    bool help;
    int length;
} args_t;

void usage(char * argv0){
    cerr<<"usage: "<<argv0<<" <solver instance percent solution> [options]"<<endl;
    cerr<<"    Solver in [ hori_verti | naive_greedy | greedy | stoch_descent | genetic | ilp ]"<<endl;
    cerr<<"    Instance file of the Google Hash Code PhotoSlideShow (*.txt)"<<endl;
    cerr<<"    Percent of the nuber of photo to be read"<<endl;
    cerr<<"    Solution file to check"<<endl;
    cerr<<"    [ -l <length> -h ]"<<endl;
}


args_t arg_parse(int argc, char * argv[]){
    args_t args;
    args.help = false;
    args.length = 1000;
    int c_arg;
    extern char *optarg;
    extern int optind;
    while( (c_arg = getopt(argc, argv, "hl:")) != EOF )
    {
        switch(c_arg)
        {
            case 'h':
                //help
                args.help = true;
                break;
            case 'l':
                //length
                args.length = atoi(optarg);
                if(args.length<0)
                {
                    fprintf(stderr,"invalide negative length");
                    exit(EXIT_FAILURE);
                }
                break;
            case '?':
                break;
            default:
                break;
        }
    }
    return args;
}

int main(int argc, char **argv){

    string name_solver, name_instance, name_solution;
    double percent;

    args_t args = arg_parse(argc, argv);

    if(args.help){
        usage(argv[0]);
        return 0;
    }

    if(argc-optind!=4){
        usage(argv[0]);
        return 1;
    }

    name_solver=argv[optind];
    name_instance=argv[optind+1];
    percent=atof(argv[optind+2]);
    name_solution=argv[optind+3];

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
    else if (name_solver.compare("naive_greedy") == 0)
        solver = new Naive_greedy;
    else if (name_solver.compare("greedy") == 0)
        solver = new Greedy(args.length);
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
