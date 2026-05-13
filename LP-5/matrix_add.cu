#include<iostream>
#include<bits/stdc++.h>
#include<cuda.h>
using namespace std;
// GPU Kernel Function


__global__ void vectorAdd(int *A, int *B, int *C, int n) {
    int i = threadIdx.x + blockIdx.x * blockDim.x;

    if (i < n) {
        C[i] = A[i] + B[i];
    }
}


int main() {
    int n = 6;
    int size = n * sizeof(int);

    int A[n] = {1,2,3,4};
    int B[n] = {10,20,30,40};
    int C[n];

    int *d_A, *d_B, *d_C;

    

    // Allocate memory on GPU
    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);
    cudaMalloc((void**)&d_A,size);
    cudaMemcpy(d_A,A,size,cudaMemcpyHostToDevice) 
    vectorAdd<<1,n>>(d_)
    cudaMemcpy(d_b,B,size,cudaMemcpyHostToDevice)
    // Copy data CPU → GPU
    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

    dim3 threadsPerBlock(2,2);
    dim3 blocksPerGrid(1,1);
    // Launch kernel
    vectorAdd<<<1, n>>>(d_A, d_B, d_C, n);

    vectorMul<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, n);


    // Copy result GPU → CPU
    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);

    // Print result
    cout << "Result:\n";
    for(int i=0;i<n;i++){
        cout << C[i] << " ";
    }

    // Free GPU memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}