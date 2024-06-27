#include<bits/stdc++.h>
typedef long long ll;
using namespace std;
#define pll pair<ll,ll>
#define pp pair<pll,pll>
#define mp make_pair
// #define f first
// #define s second 

ll naive (ll n) {
    ll dp[n+1];
    dp[1] = 1;
    dp[2] = 1;
    for (ll i=3; i<=n; i++) {
        dp[i] = dp[i-1] + dp[i-2];
    }
    return dp[n];
}

void multiply(ll F[2][2], ll M[2][2]) {
    ll a = F[0][0] * M[0][0] + F[0][1] * M[1][0];
    ll b = F[0][0] * M[0][1] + F[0][1] * M[1][1];
    ll c = F[1][0] * M[0][0] + F[1][1] * M[1][0];
    ll d = F[1][0] * M[0][1] + F[1][1] * M[1][1];

    F[0][0] = a;
    F[0][1] = b;
    F[1][0] = c;
    F[1][1] = d;
}

ll optim(ll n){

    ll ans[2][2]={1,0,0,1};
    ll m[2][2]={1,1,1,0};
    ll i=1;
    ll k=n-1;
    while(k>=i){
        if(k&i){
            multiply(ans,m);
        }
            multiply(m,m);
            i=i<<1;

    }

        return (ans[0][0]);


/*

STUDENT CODE BEGINS HERE, ACHIEVE A SPEEDUP OVER NAIVE IMPLEMENTATION
YOU MAY EDIT THIS FILE HOWEVER YOU WANT
HINT : Use the same principle behind fast exponentiation, to calculate the nth fibonacci number in O(logn) time complexity
You can view the tuple (f(n), f(n-1)) as a matrix multiplication of ((1,1),(1,0))*(f(n-1),f(n-2)) 
because f(n) = f(n-1)*1 + f(n-2)*1 while f(n-1) = 1*f(n-1) + 0*f(n-2)
Therefore, we can compute (f(n),f(n-1)) is equal to {((1,1),(1,0))^(n-1)} * (f(1),f(0))
This can be computed in O(logn) time
We expect your code to be faster (and completely inaccurate, as even naive method is inaccurate) for n >= 1000

*/

/* cout<<"Student code not implemented\n";
exit(1); */

}

int main(){
    ll n; cin >> n;
    auto startNaive = chrono::high_resolution_clock::now();
    ll slow = naive(n);
    auto endNaive = chrono::high_resolution_clock::now();
    auto naiveTime = chrono::duration_cast<chrono::duration<double>>(endNaive - startNaive);

    auto startOptim = chrono::high_resolution_clock::now();
    ll fast = optim(n);
    auto endOptim = chrono::high_resolution_clock::now();
    auto optimTime = chrono::duration_cast<chrono::duration<double>>(endOptim - startOptim);

    cout<<"Answer from naive technique : "<<slow<<endl;
    cout<<"Answer from optimal technique : "<<fast<<endl;
    cout<<"Time taken by naive technique : "<<naiveTime.count()<<endl;
    cout<<"Time taken by optimal technique : "<<optimTime.count()<<endl;

}