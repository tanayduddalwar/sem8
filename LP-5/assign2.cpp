#include<iostream>
#include<omp.h>
#include<vector>


using namespace std;

void sequential_bubble_sort(vector<int>arr1)
{
    vector<int>arr = arr1;
    int n=arr.size();
    double start_time=omp_get_wtime();
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-i-1;j++){
            if(arr[j]>arr[j+1]){
                swap(arr[j],arr[j+1]);
            }
        }
    }

    double end_time=omp_get_wtime();
    cout<<"Time taken by sequential bubble sort: "<<end_time-start_time<<" seconds"<<endl;
}

// void sequential_bubble_sort(int arr[],int size){
//     int array[size];
//     for(int i = 0 ; i < size; i++){
//         array[i] = arr[i];
//     }

//     double start = omp_get_wtime();
//     for(int i = 0; i < size - 1; i ++){
//         for(int j = 0; j < size - i - 1; j++){
//             if(array[j] > array[j+1]){
//                 swap(array[j],array[j+1]);
//             }
//         }
//     }
//     double end = omp_get_wtime();
//     cout << "Sequential Bubble Sort:\n";
//     // for(int i = 0 ; i < size; i++){
//     //     cout << array[i] << " ";
//     // }
//     cout << endl;
//     cout << "Time Required: " << end - start << endl;

// }

void parallel_bubble_sort(vector<int>arr1){
    int n=arr1.size();
    vector<int>arr = arr1;
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
    double endtime=omp_get_wtime();
    cout<<"Time taken by parallel bubble sort: "<<endtime-start_time<<" seconds"<<endl;

}



// void parallel_bubble_sort(int arr[],int size){
//     int array[size];
//     for(int i = 0 ; i < size; i++){
//         array[i] = arr[i];
//     }
//     double start = omp_get_wtime();
//     for(int k = 0; k < size;k ++){
//         if(k % 2 == 0){
//             #pragma omp parallel for
//                 for(int i = 1; i < size - 1; i += 2){
//                     if(array[i] > array[i+1]){
//                         swap(array[i],array[i+1]);
//                     }
//                 }
//         }
//         else{
//             #pragma omp parallel for
//                 for(int i = 0; i < size - 1; i += 2){
//                     if(array[i] > array[i+1]){
//                         swap(array[i],array[i+1]);
//                     }
//                 }
//         }
//     }
//     double end = omp_get_wtime();
//     cout << "Parallel Bubble Sort:\n";
//     // for(int i = 0 ; i < size; i++){
//     //     cout << array[i] << " ";
//     // }
//     cout << endl;
//     cout << "Time Required: " << end - start << endl;
// }

void merge(vector<int>arr1,int low, int high,int mid){
    int n=arr1.size();
    vector<int>temp(n,0);
    int i=low;
    int j=mid+1;
    int k=0;
    while(i<=mid && j<=high){
        if(arr1[i]>arr1[j]){
            temp[k]=arr1[j];
            j++;
            k++;
        }
        else{
            temp[k]=arr1[i];
            i++;
            k++;
        }

    }

    while(i<=low){
        temp[k]=arr1[i];
        i++;
        k++;
    }
    while(j<=high){
        temp[k]=arr1[j];
        j++;
        k++;
    }
    k=0;
    for(int i=low;i<high;i++){
        arr1[i]=temp[k];
        k++;
    }

}

void mergesort(vector<int>arr1,int low,int high){
    int mid=(low+high)/2;
    if(low<high){
        mergesort(arr1,low,mid);
        mergesort(arr1,mid+1,high);
        merge(arr1,low,high,mid);
    }
}


void perform_mergesort(vector<int>arr1){
    int n=arr1.size();
    vector<int>arr = arr1;
    double start_time=omp_get_wtime();
    mergesort(arr,0,n-1);
    double end_time=omp_get_wtime();
    cout<<"Time taken by sequential merge sort: "<<end_time-start_time<<" seconds"<<endl;
}


void parallel_merge_sort(vector<int>arr1){
    int n=arr1.size();
    vector<int>arr=arr1;
    double start_time=omp_get_wtime();
    #pragma omp parallel sections
    {
        #pragma omp section
            mergesort(arr,0,n/2);
        #pragma omp section
            mergesort(arr,n/2+1,n-1);    
    }
    merge(arr,0,n-1,n/2);
    double end_time=omp_get_wtime();
    cout<<"Time taken by parallel merge sort: "<<end_time-start_time<<" seconds"<<endl;
}
// g++ assign2.cpp -o assign -fopenmp
// ./assign



int main()
{
    int SIZE;
    int MAX = 1000;
    cout << "Enter size of array: ";
    cin >> SIZE;
    vector<int> array(SIZE);
    for(int i = 0 ; i < SIZE; i ++){
        array[i] = rand() % MAX;
    }
    cout << endl;
    sequential_bubble_sort(array);
    parallel_bubble_sort(array);
    perform_mergesort(array);
    parallel_merge_sort(array);
    return 0;
}