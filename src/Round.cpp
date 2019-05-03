#include<limits>
#include"Round.h"
#include <sstream>
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <tuple>

using namespace std;

bool path(int J, int I, vector<int> edges){
    while(J!=I && J!=-1)
        J = edges[J];
    if (J == I)
        return true;
    else if(J == -1)
        return false;
}

// Functor to compare by the Mth element
template<int M, template<typename> class F = std::less>
struct TupleCompare
{
    template<typename T>
    bool operator()(T const &t1, T const &t2)
    {
        return F<typename tuple_element<M, T>::type>()(std::get<M>(t1), std::get<M>(t2));
    }
};

Round::Round(string subtour,int timelimit){
    this->subtour = subtour;
    this->timelimit = timelimit;
}

Sol * Round::solve(){
    int i,j,k,eval;
    string s;
    vector<bool> mark;
    mark.resize(instance->nbphot);
    for (i=0;i<instance->nbphot;i++)
        mark[i]=false;
    GRBVar ** X;
    GRBVar * U;
    GRBVar ** Z;
    GRBVar *** Zk;
    
    try {
        
        cout << "create env" << endl;
        //create an environment
        GRBEnv env = GRBEnv(true);
        env.start();

        cout << "create model" << endl;
        //create an empty model
        GRBModel model = GRBModel(env);
        model.set(GRB_DoubleParam_TimeLimit, timelimit);

        cout << "create variables" << endl;
        //  Create  variables
        X = new GRBVar*[instance->nbphot];
        for (i = 0; i < instance->nbphot; i++)
            X[i] = new GRBVar[instance->nbphot];
        for (i = 0; i < instance->nbphot; i++) {
            for (j = 0; j < instance->nbphot; j++) {
                s = "X_"+itos(i)+"_"+itos(j);
                X[i][j] = model.addVar(0.0, 1.0, 0, GRB_CONTINUOUS, s);
            }
        }

        cout << "set objective" << endl;
        // Set  objective
        GRBLinExpr obj = 0.0;
        for (i = 0; i < instance->nbphot; i++) {
            for (j = 0; j < instance->nbphot; j++) {
                obj += X[i][j]*eval_slide(instance->V[i].tags, instance->V[j].tags);
            }
        }
        model.setObjective(obj, GRB_MAXIMIZE);

        cout << "add constraints" << endl;
        cout << "1" << endl;
        // Add  constraints
        // Frame
        for (i = 0; i < instance->nbphot; i++) {
            GRBLinExpr expr = 0;
            for (j = 0; j < instance->nbphot; j++) {
                expr += X[i][j];
            }
            s = "X_i_" + itos(i);
            model.addConstr(expr, GRB_EQUAL, 1.0, s);
        }
        cout << "2" << endl;
        for (j = 0; j < instance->nbphot; j++) {
            GRBLinExpr expr = 0;
            for (i = 0; i < instance->nbphot; i++) {
                expr += X[i][j];
            }
            s = "X_j_" + itos(j);
            model.addConstr(expr, GRB_EQUAL, 1.0, s);
        }
        for (i = 0; i < instance->nbphot; i++) {
            for (j = 0; j < instance->nbphot; j++) {
                GRBLinExpr expr = X[i][j] + X[j][i];
                s = "X_" + itos(i) + "-" + itos(j) + "_" + itos(j) + "-" + itos(i);
                model.addConstr(expr, GRB_LESS_EQUAL, 1.0, s);
            }
        }

        cout << "optimize" << endl;
        //  Optimize  model
        model.optimize ();

        cout << "extract solution" << endl;
        // Extract solution
        sol = new Sol();
        sol->I = this->instance;
        sol->vsol.clear();
        sol->eval_vect.clear();
        Slide slide;
        slide.p2=-1;
        
        j=0;
        i=0;
        cout << "sol count " << model.get(GRB_IntAttr_SolCount) << endl;
        cout << "build solution" << endl;

        vector<tuple<double, int>> v;
        for (i=0; i<instance->nbphot; i++){
            for (j=0; j<instance->nbphot; j++){
                v.push_back(make_tuple(X[i][j].get(GRB_DoubleAttr_X), i*instance->nbphot+j));
            }
        }
        std::sort(begin(v), end(v), TupleCompare<0>());

        vector<bool> from;
        from.resize(instance->nbphot);
        for (i=0;i<instance->nbphot;i++)
            from[i]=false;
        vector<bool> to;
        to.resize(instance->nbphot);
        for (i=0;i<instance->nbphot;i++)
            to[i]=false;
        vector<int> edges;
        edges.resize(instance->nbphot);
        for (i=0;i<instance->nbphot;i++)
            edges[i]=-1;


        int eval;
        int edge;
        int I,J;
        int max = pow(instance->nbphot,2);
        k = 0;
        for (i=0; i<instance->nbphot; i++){
            for (j=0; j<instance->nbphot; j++){
                eval = get<0>(v[max-(i*instance->nbphot+j)]);
                edge = get<1>(v[max-(i*instance->nbphot+j)]);
                I=edge/instance->nbphot;
                J=edge%instance->nbphot;
                if ( !from[I] && !to[J] && !path(J,I,edges) ) {
                    cout << "variable value " << eval << endl;
                    cout << "I " << I << endl;
                    cout << "J " << J << endl;
                    from[I]=true;
                    to[J]=true;
                    edges[I]=J;
                    k++;
                }
                if(k>=instance->nbphot-1)
                    break;
            }
            if(k>=instance->nbphot-1)
                break;
        }

        k=0;
        int next_k;
        while(edges[k]==-1)
            k++;
        slide.p1=k;
        sol->vsol.push_back(slide);
        sol->nbslides=1;
        next_k = edges[k];

        for (i=0; i<instance->nbphot; i++){
            slide.p1=k;
            sol->vsol.push_back(slide);
            sol->nbslides += 1;
            k=next_k;
            next_k = edges[k];
            if(next_k==-1)
                break;
        }
        
        sol->eval();
        if (is_file_exist("round_evals.csv")){
            ofstream outfile;
            outfile.open("round_evals.csv", std::ios_base::app);
            if (!outfile.fail())
                outfile << sol->evaluation << endl;
        }

    } catch(GRBException e) {
        cout << "Error code = " << e.getErrorCode() << endl;
        cout << e.getMessage() << endl;
    } catch(...) {
        cout << "Exception during optimization" << endl;
    }

    return sol;
}
