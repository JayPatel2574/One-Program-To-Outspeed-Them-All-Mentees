#include <iostream>
#include <math.h>
using namespace std;

/*

TASK : Implement Radix Sort

Binary radix sort is a non-comparative integer sorting algorithm that sorts data by processing individual bits of the numbers.
The algorithm sorts the numbers by examining each bit from the most significant bit (MSB) to the least significant bit (LSB).
For each bit position, it partitions the numbers into two groups: those with the bit set to 0 and those with the bit set to 1.
This process is repeated for each bit position, and the numbers are merged back together after each bit pass. 
Binary radix sort is efficient for fixed-length integer keys, offering a time complexity of O(n) per pass, where n is the number of elements, 
making it suitable for sorting large datasets of integers.
EXPECTED TIME COMPLEXITY : O(n*log(q)) where q = max(arr)

*/
int max(int *arr, int size){
    int max = arr[0];
    for(int i =1;i<size;i++){
        if(arr[i]>max){
            max = arr[i];
        }
    }
    int i=1;
    while(max>pow(2,i-1)){
        i++;
    }
    return i;
}

void countsort(int *arr, int size, int i,int t){
    int count[2]={0,0};
    int sfted[size];
    for(int k=0;k<size;k++){
        sfted[k]=arr[k]>>t;
    }
    int j=1;
    for(int k=0;k<size;k++){
        if(sfted[k]&j){
            count[1]++;
        }
        else {
            count[0]++;
        }
    }
    int output[size];
    count[1] += count[0];
    for(int k=size-1;k>=0;k--){
        output[count[(sfted[k]&j)]-1]=arr[k];
        count[(sfted[k]&j)]--;
    }
    for(int k =0;k<size;k++){
        arr[k]=output[k];
    }
}

void radixSort (int *arr, int size) {
    int n = max(arr,size);
    int i=1;
    for(int t=0; t<n;t++){
        countsort(arr,size,i,t);
        i= i<<1;
    }
}

int main () {

    int n;
    cin>>n;
    int arr[n];
    for (int i=0; i<n; i++) {
        arr[i] = rand();
    }
    radixSort(arr,n);
    for (int i=0; i<n-1; i++) {
        if (arr[i] > arr[i+1]) {
            cout<<"Sorting not done correctly!\n";
            exit(1);
        }
    }
    cout<<"Good job, array is sorted\n";
    return 0;
}