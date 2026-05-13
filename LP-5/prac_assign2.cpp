#include <iostream>
#include<vector>
#include<omp.h>

using namespace std;

void sequential_bubble_sort(vector<int>arr){
    vector<int>arr1=arr;
    int n=arr.size();
    double start_time=omp_get_wtime();
    for(int i=0;i<n;i++){
        for(int j=0;j<n-i-1;j++){
            if(arr1[j]>arr1[j+1]){
                swap(arr1[j],arr1[j+1]);
            }
        }
    }
    double end_time=omp_get_wtime();
    cout<<"Time taken by sequential bubble sort: "<<end_time-start_time<<" seconds"<<endl;
    cout<<"Sorted array: ";
    for(int i=0;i<n;i++){
        cout<<arr1[i]<<" ";
    }

}

void parallel_bubble_sort(vector<int>arr){
    int n=arr.size();
    vector<int>arr1=arr;
    double start_time=omp_get_wtime();
    for(int k=0;k<n;k++){
        if(k%2==0){
            #pragma omp parallel for
            for(int i=0;i<n-1;i=i+2){
                if(arr[i]>arr[i+1]){
                    swap(arr[i],arr[i+1]);
                }
            }
        }
        else{
            #pragma omp parallel for
            for(int i=1;i<n-1;i=i+2){
                if(arr[i]>arr[i+1]){
                    swap(arr[i],arr[i+1]);
                }
            }


        }
    }
}

void merge(vector<int>&arr,int low, int mid,int high){
    int l=low;
    int r=mid+1;
    int k=0;
    int n=arr.size();
    vector<int>sorted(n,0);
    while(l<=mid && r<=high){
        if(arr[l]>arr[r]){
            sorted[k]=arr[r];
            r++;
            k++;
        }
        else{
            sorted[k]=arr[l];
            l++;
            k++;
        }
    }
    while(l<=mid){
        sorted[k]=arr[l];
        l++;
        k++;
    }
    while(r<=high){
        sorted[k]=arr[r];
        r++;
        k++;
    }
    k=0;
    for(int i=low;i<=high;i++){
        arr[i]=sorted[k];
        k++;
    }
}

void seq_merge_sort(vector<int>&arr,int low, int high){
    int mid=(low+high)/2;
    if(low<high){
        seq_merge_sort(arr,low,mid);
        seq_merge_sort(arr,mid+1,high);
        merge(arr,low,mid,high);
    }
}

void parallel_merge_sort(vector<int>&arr,int low,int high){
    int mid=(low+high)/2;
    if(low<high){
        #pragma omp parallel sections
    {
        #pragma omp section 
        {
            parallel_merge_sort(arr,low,mid);
        }

        #pragma omp section
        {
            parallel_merge_sort(arr,mid+1,high);

        }
    }
        merge(arr,low,mid,high);
    }
}
int main(int argc, char const *argv[])
{
    int n;
    cout<<"Enter the number of elements in array"<<endl;
    cin>>n;
    vector<int>arr(n);
    cout<<"Enter the elements of array"<<endl;
    for(int i=0;i<n;i++){
        cin>>arr[i];
    }
    vector<int>arr1=arr;
    double start_time=omp_get_wtime();
    sequential_bubble_sort(arr);
    double end_time=omp_get_wtime();
    cout<<"Time taken by sequential bubble sort: "<<end_time-start_time<<" seconds"<<endl;

    double start_time1=omp_get_wtime();
    parallel_bubble_sort(arr);
    double end_time1=omp_get_wtime();
    cout<<"Time taken by parallel bubble sort: "<<end_time1-start_time1<<" seconds"<<endl;
    int low=0;
    int high=n-1;
    double start_time2=omp_get_wtime();
    seq_merge_sort(arr1,low,high);
    double end_time2=omp_get_wtime();
    cout<<"Time taken by sequential merge sort: "<<end_time2-start_time2<<" seconds"<<endl;
    double start_time3=omp_get_wtime();
    for(int i=0;i<n;i++){
        cout<<arr1[i]<<" ";
    }
    parallel_merge_sort(arr1,low,high);
    double end_time3=omp_get_wtime();
    cout<<"Time taken by parallel merge sort: "<<end_time3-start_time3<<" seconds"<<endl;


    // /* code */
    return 0;
}
