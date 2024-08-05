#include <iostream>
#include <thread>
#include <xmmintrin.h>
#include <chrono>
#include <cmath>
#include <bits/stdc++.h>

using namespace std;

/*

TASK:
Given a tree consisting of nodes and a special integer k, find the secret of the node with value k
A secret of a node t is defined as a string s, such that s[i] is the bit of the ith node occurring on the path from root to t 
Each node contains 3 members : 
1] value - the integer value of the node (unique to each node)
2] bit - a character of each node (not necessarily unique)
3] children - a vector containing pointers to all children of the node
YOU MUST OUTPERFORM THE NAIVE IMPLEMENTATION

*/

class node {
public:
    int value;
    vector<node*> children;
    char bit;
    node* parent;
};

class tree {
private:
    int n;
    vector<node *> listOfNodes;
public:
    vector<node *> listforme = listOfNodes; //added extra variable for me
    node* root;
    void addConnection (node *a, node *b) { // adds b as child of a
        a->children.push_back(b);
    }
    tree(int a) {
        n = a;
    }
    void setListOfNodes (vector<node*> list) {
        if (list.size() != n) {
            cout<<"ERROR, PROVIDED LIST SIZE DOES NOT MATCH SIZE OF TREE!\n";
            exit(1);
        }
        listOfNodes = list;
    }
    ~tree() {
        for (int i=0; i<n; i++) {
            delete listOfNodes[i];
        }
    }
    int getSize () {
        return n;
    }
};

string naiveSearch (tree &T, int k) {
    queue<node*> q;
    q.push(T.root);
    while (!q.empty()) {
        node *tmp = q.front();
        q.pop();
        if (tmp->value == k) { // found it!
            vector<node*> seq; // vector to construct the string
            seq.push_back(tmp);
            node *u = tmp->parent;
            while (u != NULL) {
                seq.push_back(u);
                u = u->parent;    
            }
            string t;
            for (int i=seq.size() - 1; i >= 0; i--) {
                t += seq[i]->bit;
            }
            return t;
        }
        for (auto x : tmp->children) {
            q.push(x);
        }
    }
    return "\0";
}

int Find(unsigned long start, unsigned long end, vector<node*> q,int k){
    for(int i=start; i<end; ++i){
                      
        node *tmp = q[i];
                
        if (tmp->value == k) { // found it!
            return (i+1);
        }    
    }    
    return 0;
}

string optim(tree &T, int k) {
/*

STUDENT CODE BEGINS HERE, ACHIEVE A SPEEDUP OVER NAIVE IMPLEMENTATION
YOU MAY EDIT THIS FILE HOWEVER YOU WANT (as long as you don't touch main or naiveSearch)
HINT : USE MULTITHREADING TO SEARCH IN SUBTREES THEN RETURN THE MOMENT U FIND IT
(Note we do not expect to see a speedup for low values of n, but for n > 10000)

*/  
    
    vector<node*> q = T.listforme;
    
    int tempNo;

    long unsigned int n=T.getSize();

    const unsigned int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads; 
    unsigned long chunkSize = (n + numThreads - 1) / numThreads;  

    for (unsigned long t = 0; t < numThreads; ++t) {
        unsigned long start = t * chunkSize;
        unsigned long end = std::min((t + 1) * chunkSize, n);
        threads.push_back(thread(Find, start, end, q, k));
        if (Find(start,end,q,k)){
            tempNo = Find(start,end,q,k) - 1;
        } 
    }
    for (auto& thread : threads) {
        thread.join();
    }
    
    node *temp = q[tempNo];
    vector<node*> seq;
    seq.push_back(temp);
    node *u = temp->parent;
    while (u != NULL) {
        seq.push_back(u);
        u = u->parent;
    }
    string t;
    for (int i=seq.size() - 1; i >= 0; i--) {
        t += seq[i]->bit;
    }
    return t;

    return "\0";

/*     cout<<"Student code not implemented\n";
    exit(1); */

}

int main() {
    int n;
    cin >> n;
    tree T(n);
    vector<node*> list;
    node* u = new node;
    u->value = 0;
    u->bit = 'a';
    u->parent = NULL;
    T.root = u;
    list.push_back(u);
    char currChar = 'b';
    for (int i = 1; i < n; i++) {
        node *tmp = new node;
        tmp->value = i;
        tmp->bit = currChar;
        if (currChar == 'z') {
            currChar = 'a';
        } else {
            currChar++;
        }
        if (i % 3 == 0) {
            T.addConnection(list[i / 3 - 1], tmp);
            tmp->parent = list[i/3-1];
        } else {
            T.addConnection(list[i / 3], tmp);
            tmp->parent = list[i/3];
        }
        list.push_back(tmp);
    }
    T.setListOfNodes(list);

    auto startNaive = chrono::high_resolution_clock::now();
    string naive = naiveSearch(T,n-1);
    auto endNaive = chrono::high_resolution_clock::now();
    auto naiveTime = chrono::duration_cast<chrono::duration<double>>(endNaive - startNaive);

    auto startOptim = chrono::high_resolution_clock::now();
    string optimSearch = optim(T, n - 1);
    auto endOptim = chrono::high_resolution_clock::now();
    auto optimTime = chrono::duration_cast<chrono::duration<double>>(endOptim - startOptim);

    cout<<"Naive string : "<<naive<<endl;
    cout<<"Optim string : "<<optimSearch<<endl;
    cout<<"Naive time : "<<naiveTime.count()<<endl;
    cout<<"Optim time : "<<optimTime.count()<<endl;

    return 0;
}
