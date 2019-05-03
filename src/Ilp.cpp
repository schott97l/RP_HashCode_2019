#include<limits>
#include"Ilp.h"

using namespace std;

Ilp::Ilp(string subtour,int timelimit){
    this->subtour = subtour;
    this->timelimit = timelimit;
}

Sol * Ilp::solve(){
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
        //env.set("LogFile","ILP.log");
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
                X[i][j] = model.addVar(0.0, 1.0, 0, GRB_BINARY, s);
            }
        }
        if (subtour.compare("MTZ")==0){
            U = new GRBVar[instance->nbphot];
            for (i = 0; i < instance->nbphot; i++){
                s = "U_"+itos(i);
                U[i] = model.addVar(0, instance->nbphot, instance->nbphot+1, GRB_CONTINUOUS, s);
            }
        }else if (subtour.compare("flow")==0){
            Z = new GRBVar*[instance->nbphot];
            for (i = 0; i < instance->nbphot; i++)
                Z[i] = new GRBVar[instance->nbphot];
            for (i = 0; i < instance->nbphot; i++) {
                for (j = 1; j < instance->nbphot; j++) {
                    if (i!=j){
                        s = "Z_"+itos(i)+"_"+itos(j);
                        Z[i][j] = model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, s);
                    }
                }
            }
        }else if (subtour.compare("desaggregated flow")==0){
            Zk = new GRBVar**[instance->nbphot];
            for (k = 1; k < instance->nbphot; k++){
                Zk[k] = new GRBVar*[instance->nbphot];
                for (i=0; i< instance->nbphot; i++){
                    Zk[k][i] = new GRBVar[instance->nbphot];
                }
            }
            for (k = 1; k < instance->nbphot; k++) {
                for (i = 0; i < instance->nbphot; i++) {
                    for (j = 1; j < instance->nbphot; j++) {
                        if (i!=j){
                            s = "Z_"+ itos(k) + "_" + itos(i)+"_"+itos(j);
                            Zk[k][i][j] = model.addVar(0, GRB_INFINITY, 1, GRB_CONTINUOUS, s);
                        }
                    }
                }
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
        //SubTour
        if (subtour.compare("MTZ")==0){
            cout << "3" << endl;
            GRBLinExpr expr = U[0];
            s = "U_0";
            model.addConstr(expr, GRB_EQUAL, 1, s);
            cout << "4" << endl;
            for (i = 1; i < instance->nbphot; i++) {
                expr += U[i];
                s = "U_" + itos(i) + "_<=";
                model.addConstr(expr, GRB_LESS_EQUAL, instance->nbphot, s);
                s = "U_" + itos(i) + "_>=";
                model.addConstr(expr, GRB_GREATER_EQUAL, 2, s);
            }
            cout << "5" << endl;
            GRBLinExpr expr1;
            GRBLinExpr expr2;
            for (i = 1; i < instance->nbphot; i++){
                for (j = 1; j < instance->nbphot; j++)
                    if (j!=i){
                        expr1 = U[i] - U[j] + 1;;
                        expr2 = instance->nbphot * (1-X[i][j]);
                        s = "U_" + itos(i) + "_" + itos(j);
                        model.addConstr(expr1, GRB_LESS_EQUAL, expr2, s);
                    }
            }
        }else if (subtour.compare("flow")==0){
            cout << "3" << endl;
            GRBLinExpr expr = 0;
            for (j = 1; j < instance->nbphot; j++) {
                expr += Z[0][j];
            }
            s = "Z_0j";
            model.addConstr(expr, GRB_EQUAL, instance->nbphot - 1, s);
            cout << "4" << endl;
            for (i = 1; i < instance->nbphot; i++) {
                GRBLinExpr expr1 = 0;
                GRBLinExpr expr2 = 0;
                for (j = 1; j < instance->nbphot; j++) {
                    if (j!=i)
                        expr1 += Z[i][j];
                }
                expr1 += 1;
                for (j = 0; j < instance->nbphot; j++) {
                    if (j!=i)
                        expr2 += Z[j][i];
                }
                s = "Z_" + itos(i) + "-" + itos(j) + "_" + itos(j) + "-" + itos(i);
                model.addConstr(expr1, GRB_EQUAL, expr2, s);
            }
            cout << "5" << endl;
            for (i = 0; i < instance->nbphot; i++) {
                for (j = 1; j < instance->nbphot; j++) {
                    if(j!=i){
                        if (i!=0){
                            GRBLinExpr expr1 = Z[i][j]+Z[j][i];
                            GRBLinExpr expr2 = (instance->nbphot-1)*(X[i][j]+X[j][i]);
                            s = "Z_"+ itos(i) + "-" + itos(j) + "_X_" + itos(i) + "-" + itos(j);
                            model.addConstr(expr1, GRB_LESS_EQUAL, expr2, s);

                        }else{
                            GRBLinExpr expr1 = Z[i][j];
                            GRBLinExpr expr2 = (instance->nbphot-1)*(X[i][j]);
                            s = "Z_"+ itos(i) + "-" + itos(j) + "_X_" + itos(i) + "-" + itos(j);
                            model.addConstr(expr1, GRB_LESS_EQUAL, expr2, s);

                        }
                    }
                }
            }
        }else if (subtour.compare("desaggregated flow")==0){
            cout << "3" << endl;
            GRBLinExpr expr=0;
            for (k = 1; k < instance->nbphot; k++)
                for (j = 1; j < instance->nbphot; j++)
                    expr += Zk[k][0][j];
            s = "Z_" + itos(k) + "_0_" + itos(j);
            model.addConstr(expr, GRB_EQUAL, 1, s);
            cout << "4" << endl;
            for (k = 1; k < instance->nbphot; k++)
                for (i = 1; i < instance->nbphot; i++)
                    if (i!=k){
                        GRBLinExpr expr1 = 0;
                        GRBLinExpr expr2 = 0;
                        for (j = 1; j < instance->nbphot; j++) {
                            if (j!=i)
                                expr1 += Zk[k][i][j];
                        }
                        for (j = 0; j < instance->nbphot; j++) {
                            if (j!=i)
                                expr2 += Zk[k][j][i];
                        }
                        s = "Z_" + itos(i) + "-" + itos(j) + "_" + itos(j) + "-" + itos(i);
                        model.addConstr(expr1, GRB_EQUAL, expr2, s);
                    }
            cout << "5" << endl;
            for (k = 1; k < instance->nbphot; k++) {
                GRBLinExpr expr1 = 0;
                GRBLinExpr expr2 = 0;
                for (j = 1; j < instance->nbphot; j++) {
                    if (j!=k)
                        expr1 += Zk[k][k][j];
                }
                expr1 += 1;
                for (j = 0; j < instance->nbphot; j++) {
                    if (j!=k)
                        expr2 += Zk[k][j][k];
                }
                s = "Z_" + itos(k) + "-" + itos(k) + "-" + itos(j) + "_" + itos(k) + "-"+ itos(j) + "-" + itos(k);
                model.addConstr(expr1, GRB_EQUAL, expr2, s);
            }
            cout << "6" << endl;
            for (k = 1; k < instance->nbphot; k++) 
                for (i = 0; i < instance->nbphot; i++) {
                    for (j = 1; j < instance->nbphot; j++) {
                        if(j!=i){
                            if (i!=0){
                                GRBLinExpr expr1 = Zk[k][i][j]+Zk[k][j][i];
                                GRBLinExpr expr2 = X[i][j]+X[j][i];
                                s = "Z_" + itos(k) + "-" + itos(i) + "-" + itos(j) + "_X_" + itos(i) + "-" + itos(j);
                                model.addConstr(expr1, GRB_LESS_EQUAL, expr2, s);

                            }else{
                                GRBLinExpr expr1 = Zk[k][i][j];
                                GRBLinExpr expr2 = X[i][j];
                                s = "Z_" + itos(k) + "-" + itos(i) + "-" + itos(j) + "_X_" + itos(i) + "-" + itos(j);
                                model.addConstr(expr1, GRB_LESS_EQUAL, expr2, s);
                            }
                        }
                    }
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
        
        slide.p1=0;
        sol->vsol.push_back(slide);
        sol->eval_vect.push_back(0);
        sol->evaluation=0;
        sol->nbslides=1;
        j=0;
        i=0;
        cout << j<< endl;
        cout << "sol count" << endl;
        if (model.get(GRB_IntAttr_SolCount)>0)
        {
            while(X[i][j].get(GRB_DoubleAttr_X)!=1){
                j++;
                if(j>=instance->nbphot){
                    j=0;
                    i++;
                }
                if(i>=instance->nbphot)
                    break;
            }
            cout << "build solution" << endl;
            for (k=1; k<instance->nbphot; k++){
                slide.p1=j;
                sol->vsol.push_back(slide);
                eval = eval_slide(instance->V[i].tags,instance->V[j].tags);
                sol->eval_vect.push_back(eval);
                sol->evaluation+=eval;
                sol->nbslides++;
                mark[j]=true;
                i=j;
                j=0;
                while(X[i][j].get(GRB_DoubleAttr_X)!=1){
                    j++;
                    if(j>=instance->nbphot){
                        j=-1;
                        break;
                    }
                }
                if (j==-1){
                    j=0;
                    for (j=0;j<instance->nbphot; j++){
                        if (mark[j]==false)
                            break;
                    }
                    if(j>=instance->nbphot)
                        break;
                }
            }
            sol->eval();
        }

        if (is_file_exist("ilp_evals.csv")){
            ofstream outfile;
            outfile.open("ilp_evals.csv", std::ios_base::app);
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
