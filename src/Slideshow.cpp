#include <unistd.h>

#include"Solver.h"
#include"Hori_verti.h"
#include"Naive_greedy.h"
#include"Greedy.h"
#include"Random.h"
#include"Stoch_descent.h"
#include"Genetic.h"
#include"Ilp.h"

using namespace std;

typedef struct {
    bool help;
    int length;
    int nb_iter;
    int nb_neigh;
    string input_sol;
} args_t;

void usage(char * argv0){
    cerr<<"usage: "<<argv0<<" <solver instance percent solution> [options]"<<endl;
    cerr<<"    Solver in [ hori_verti | naive_greedy | greedy | random | stoch_descent | genetic | ilp ]"<<endl;
    cerr<<"    Instance file of the Google Hash Code PhotoSlideShow (*.txt)"<<endl;
    cerr<<"    Percent of the instance to be read"<<endl;
    cerr<<"    Output solution"<<endl;
    cerr<<"    [ -l <length> -s <input solution> -h ]"<<endl;
}


args_t arg_parse(int argc, char * argv[]){
    args_t args;
    args.help = false;
    args.length = 1000;
    args.input_sol = "";
    args.nb_iter = 100;
    args.nb_neigh = 100;
    int c_arg;
    extern char *optarg;
    extern int optind;
    while( (c_arg = getopt(argc, argv, "hl:i:n:s:")) != EOF )
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
            case 'i':
                //n iter
                args.nb_iter = atoi(optarg);
                if(args.nb_iter<0)
                {
                    fprintf(stderr,"invalide negative nb iter");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'n':
                //nb neighbours
                args.nb_neigh = atoi(optarg);
                if(args.nb_neigh<0)
                {
                    fprintf(stderr,"invalide negative nb neighbours");
                    exit(EXIT_FAILURE);
                }
                break;
            case 's':
                //input solution
                args.input_sol=optarg;
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

    ifstream fic_instance(name_instance.c_str());
    if (fic_instance.fail()){
        cerr<<"file "<<name_instance<<" "<<" not found"<<endl;
        return 1;
    }
    Instance I(fic_instance,percent);
    fic_instance.close();
    I.sort_tags();
    
    Sol * solution=NULL;
    if (args.input_sol.compare("")!=0){
        ifstream fic_solution_in(args.input_sol.c_str());
        if (fic_solution_in.fail()){
            cerr<<"file "<<args.input_sol<<" "<<" not found"<<endl;
            return 1;
        }
        solution = new Sol(&I,fic_solution_in);
    }

    Solver * solver= NULL;

    if (name_solver.compare("hori_verti") == 0)
        solver = new Hori_verti;
    else if (name_solver.compare("naive_greedy") == 0)
        solver = new Naive_greedy;
    else if (name_solver.compare("greedy") == 0)
        solver = new Greedy(args.length);
    else if (name_solver.compare("random") == 0)
        solver = new Random();
    else if (name_solver.compare("stoch_descent") == 0)
    {
        if (args.input_sol.compare("")!=0)
            solver = new Stoch_descent(args.nb_iter, args.nb_neigh);
        else
            solver = new Stoch_descent(args.nb_iter, args.nb_neigh, args.length);
    }
    else if (name_solver.compare("genetic") == 0)
        solver = new Genetic(args.nb_iter,args.nb_neigh,args.length);
    else if (name_solver.compare("ilp") == 0)
        solver = new Ilp;
    else
        solver = new Hori_verti;

    if (solution==NULL)
        solver->load(&I);
    else
        solver->load(&I,solution);

    solver->solve();

    ofstream fic_solution_out(name_solution.c_str());
    if (fic_solution_out.fail()){
        cerr<<"file "<<name_solution<<" "<<" not found"<<endl;
        return 1;
    }

    solver->save(fic_solution_out);

    cout << "eval : " << solver->eval() << endl;

    return 0;
}
