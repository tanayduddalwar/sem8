#include <iostream>
#include <omp.h>
#include <ctime>
#include <cstdlib>
#include <vector>
using namespace std;

void min_func(vector<int>arr){
    int n=arr.size();
    double min_val=10000;
    #pragma omp parallel for reduction(min:min_val)
    for(int i=0;i<n;i++){
        cout<<"thread id="<<omp_get_thread_num()<<"and i="<<i<<endl;
        if(arr[i]<min_val){
            min_val=arr[i];
        }
    }
    cout<<"Minimum value = "<<min_val<<endl;
}

void max_func(vector<int>arr){
     int n=arr.size();
    double max_val=0.0;
    #pragma omp parallel for reduction(max:max_val)
    for(int i=0;i<n;i++){
        cout<<"thread id="<<omp_get_thread_num()<<"and i="<<i<<endl;
        if(arr[i]>max_val){
            max_val=arr[i];
        }
    }
    cout<<"Maximum value = "<<max_val<<endl;
}

void sum_func(vector<int>arr){
    int n=arr.size();
    float sum=0;
    #pragma omp parallel for reduction(+:sum)
    for(int i=0;i<n;i++){
        cout<<"thread id="<<omp_get_thread_num()<<"and i="<<i<<endl;
        sum=sum+arr[i];
    }
    cout<<"Sum = "<<sum<<endl;
}

void avg_func(vector<int>arr){
    int n=arr.size();
    float sum=0,avg=0;
    #pragma omp parallel for reduction(+:sum)
    for(int i=0;i<n;i++){
        cout<<"thread id="<<omp_get_thread_num()<<"and i="<<i<<endl;
        sum=sum+arr[i];
    }
    cout<<"Sum = "<<sum<<endl;
    avg=sum/n;
    cout<<"Average = "<<avg<<endl;
}

int main()
{
    int n;
    omp_set_num_threads(4);
    cout<<"Enter the number of elements in the array"<<endl;
    cin>>n;
    vector<int>arr;
    for(int i=0;i<n;i++){
        int element;
        cout<<"Enter element "<<i+1<<endl;
        cin>>element;
        arr.push_back(element);
    }

    for(int i=0;i<n;i++){
        cout<<arr[i]<<" ";
    }

    min_func(arr);
    // max_func(arr);
    // sum_func(arr);
    // avg_func(arr);

   return 0;
}
