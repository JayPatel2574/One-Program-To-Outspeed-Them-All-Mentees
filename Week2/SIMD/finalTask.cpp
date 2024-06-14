#include <iostream>
#include <immintrin.h>
#include <iomanip>
#include <chrono>
using namespace std;
/*

COMPUTE DOT PRODUCT OF TWO VECTORS
INPUTS :n => dimension of vectors
        vec1 => the first vector
        vec2 => the second vector

OUTPUT:dotProduct = vec1.vec2

dotProduct of two vectors is defined as summation_{i=0}^{i=n} [vec1[i]*vec2[i]];

*/
int main () {

    int n;
    cin>>n;
    float vec1[n];
    float vec2[n];
/*     for (int i=0; i<n; i++) {
        cin>>vec1[i];
    }
    for (int i=0; i<n; i++) {
        cin>>vec2[i];
    } */

        for (int i=0; i<n; i++) {
            vec1[i]=i;
            vec2[i]=2*i;
        }

    float naiveTimeTaken = 0;
    float SIMDTimeTaken = 0;
    float dotProduct = 0;
    for (int i=0; i<1000; i++) {
        dotProduct = 0;
        auto start = chrono::high_resolution_clock::now();
        //NAIVE IMPLEMENTATION FOR COMPARISION PURPOSES
        for (int i=0; i<n; i++) {
            dotProduct += vec1[i]*vec2[i];
        }
        //NAIVE IMPLEMENTATION ENDS
        auto end = chrono::high_resolution_clock::now();
        auto elapsed = chrono::duration_cast<chrono::duration<double>>(end - start);
        naiveTimeTaken += (elapsed.count())/1000;
    }

    cout<<dotProduct<<" "<<naiveTimeTaken<<" "<<endl;
    
    for (int i=0; i<1000; i++) {
        dotProduct = 0;
        auto start2 = chrono::high_resolution_clock::now();
        //STUDENT CODE BEGINS HERE
       /*  cout<<"STUDENT CODE NOT IMPLEMENTED!\n";
        exit(-1); */
       __m256 dotProduct1 = _mm256_setzero_ps();

        int k = int(n/8)*8;

       for (int i=0; i<k; i+=8) {
        __m256 v1,v2,mul;
        v1 = _mm256_loadu_ps(vec1+i);
        v2 = _mm256_loadu_ps(vec2+i);
        mul = _mm256_mul_ps(v1, v2);
        dotProduct1 = _mm256_add_ps(dotProduct1, mul);
       }

       __m128 upper = _mm256_extractf128_ps(dotProduct1, 1);
       __m128 lower = _mm256_extractf128_ps(dotProduct1, 0);

       __m128 sum = _mm_hadd_ps(lower, upper);
              sum = _mm_hadd_ps(sum, sum); 
              sum = _mm_hadd_ps(sum, sum);

              dotProduct = _mm_cvtss_f32(sum);

        for (int i=k; i<n; i++) {
            dotProduct += vec1[i]*vec2[i];
        }  


        //END OF STUDENT CODE
        auto end2 = chrono::high_resolution_clock::now();
        auto elapsed2 = chrono::duration_cast<chrono::duration<double>>(end2 - start2);
        SIMDTimeTaken += (elapsed2.count())/1000;
    }
    cout<<dotProduct<<" "<<SIMDTimeTaken<<" "<<endl;
}