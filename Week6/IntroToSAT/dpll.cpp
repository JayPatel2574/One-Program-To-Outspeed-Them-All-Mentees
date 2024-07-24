#include <bits/stdc++.h>
using namespace std;

/*

The Naive SAT Solver algorithm is far too slow, thus there is a better algorithm for SAT solving called DPLL
Davis–Putnam–Logemann–Loveland (DPLL) Algorithm:
* First perform unit propogation to get rid of unit clauses (clauses that have only one literal)
* Then, decide the value of some variable and check if the formula becomes SAT (by for example, recursively calling DPLL)
* If it does, return the assignment
* If not, then change the value of that variable 

*/

#define UNDEFINED -1
#define TRUE 1
#define FALSE 0

class dpll {
    private:
    uint numVariables;
    uint numClauses;
    vector<vector<int>> clauses;
    bool unitProp (vector<int>& partialModel);
    bool doPll (vector<int>& partialModel);
    
    public:
    vector<int> finalModel;
    void getInput ();
    bool solve();

};



void dpll::getInput () {

    char c = cin.get(); // CNF comments
    while (c == 'c') {
        while (c != '\n') {
            c = cin.get();
        }
        c = cin.get();
    }
    string t;
    cin>>t;

    cin>>numVariables;
    cin>>numClauses;
    for (uint i=0; i<numVariables+1; i++) {
        finalModel.push_back(UNDEFINED);
    }
    for (uint i=0; i<numClauses; i++) {
        vector<int> a;
        int c;
        cin>>c;
        while (c != 0) {
            a.push_back(c);
            cin>>c;
        }
        // a.value = UNDEFINED;
        // a.size = a.elements.size();
        clauses.push_back(a);
    }
}

bool dpll::unitProp (vector<int>& partialModel) {
/*

Implement unit propogation!
while (true) {

    if (there exists some clause C, such that there is only one undecided variable in C) {
        decide the variable so as to make C true; (i.e. if C is x, then set x to true, else if C is -x, set x to false)
    }
    if (some clause becomes false) {
        return false;
    }
    if (no such clause exists) {
        break;
    }

}

*/

    while (true) {
        bool unitClauseFound = false;
        int unitClauseCounter = 0;
        for (int i=0 ; i<numClauses ; ++i) {
            int undecidedCount = 0;
            int undecidedVar = 0;
            bool clauseIsTrue = false;
            
            for (int j=0 ; j<clauses[i].size() ; ++j ) {
                if (partialModel[abs(clauses[i][j])] == (clauses[i][j] > 0 ? TRUE : FALSE)) {
                    clauseIsTrue = true;
                    break;
                }
            }

            if(clauseIsTrue){
                continue;
            }            

            for (int j=0 ; j<clauses[i].size() ; ++j ) {
                if (partialModel[abs(clauses[i][j])] == UNDEFINED) {
                    undecidedCount++;
                    undecidedVar = clauses[i][j];
                }
            }
            if (undecidedCount == 1) {
                partialModel[abs(undecidedVar)] = (undecidedVar > 0) ? TRUE : FALSE;
                unitClauseFound = true;
                unitClauseCounter++;
                }
            
        }
        if (unitClauseCounter == 0){
            break;
        }
        if (!unitClauseFound) {
            break;
        }
        if(unitClauseCounter>0){
            for (int i=0 ; i<numClauses ; ++i) {
                bool clauseTrue = false;
                for (int j=0 ; j<clauses[i].size() ; ++j ) {
                    if (partialModel[abs(clauses[i][j])] == (clauses[i][j] > 0 ? TRUE : FALSE)) {
                        clauseTrue = true;
                        break;
                    }
                }
                if (!clauseTrue) {
                    return false;
                }
            }
        }
    }
    return true;      
}

bool dpll::doPll (vector<int>& partialModel) {



/* Implement the dpll algorithm
unitProp(partialModel)
if (formula is SAT) {
    finalModel = partialModel;
    return true;
} */

if(!unitProp(partialModel)){
    return false;
}
    bool SAT2=true;
    for(uint i=0; i<numClauses; ++i){
        bool SAT1 = false; 
        for(uint j=0; j<clauses[i].size(); ++j){
            if ( partialModel[abs(clauses[i][j])] == (clauses[i][j]>0 ? TRUE : FALSE ) ){
                SAT1 = true;
                break;
            }
        }
        if (!SAT1){
            SAT2 = false;
            break;
        }
    }
    if (SAT2){
        finalModel = partialModel;
        return true;    
    }

/* else {
    if (there exists variable x, partialModel[x] == UNDECIDED) {
        posMod = partialModel;
        posMod[x] = 1;
        negMod = partialModel;
        negMod[x] = 0;
        return (doPll(posMod) || doPll(negMod));
    }
    else {
        return false;
    }
} */

    else{
        for(uint x=1; x<=numVariables; ++x){
            if(partialModel[x] == UNDEFINED){
                vector<int> Mod = partialModel;
                for(int j=0; j<2; ++j){
                    Mod[x]=FALSE;
                    if(doPll(Mod)){
                        return true;
                    }
                    Mod[x]=TRUE;
                }
            }
        }
        return false;
    }

}

bool dpll::solve() {
    vector<int> m(numVariables,UNDEFINED);
    return doPll(m);
}

int main () {
    
    dpll d;
    d.getInput();

    if (d.solve()) {
        cout<<"SAT\n";
        for (int i=1; i<d.finalModel.size(); i++) {
            cout<<i<<" : "<<d.finalModel[i]<<endl;
        }
    }
    else {
        cout<<"UNSAT\n";
    }
}