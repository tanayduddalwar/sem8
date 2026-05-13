#include <iostream>
#include <cuda_runtime.h>
using namespace std;


// Kernel function
__global__ void matrixMul(int *A, int *B, int *C,
                         int rowsA, int colsA, int colsB) {

    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    // Ensure thread is within output matrix bounds
    if (row < rowsA && col < colsB) {

        int sum = 0;

        // Loop over common dimension
        for (int k = 0; k < colsA; k++) {
            sum += A[row * colsA + k] * B[k * colsB + col];
//             A[row][k]+B[k][col]
// C[row][col]=sum
        }

        C[row * colsB + col] = sum;
    }
}

int main() {

    // Example: A(2×3), B(3×2)
    int rowsA = 2, colsA = 3, colsB = 2;

    int sizeA = rowsA * colsA * sizeof(int);
    int sizeB = colsA * colsB * sizeof(int);
    int sizeC = rowsA * colsB * sizeof(int);

    // Host matrices
    int A[] = {1,2,3,
               4,5,6};

    int B[] = {7,8,
               9,10,
               11,12};

    int C[4]; // 2×2 result

    // Device pointers
    int *d_A, *d_B, *d_C;

    // Allocate GPU memory
    cudaMalloc((void**)&d_A, sizeA);
    cudaMalloc((void**)&d_B, sizeB);
    cudaMalloc((void**)&d_C, sizeC);

    // Copy data to GPU
    cudaMemcpy(d_A, A, sizeA, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, sizeB, cudaMemcpyHostToDevice);

    // Thread and block configuration
    dim3 threadsPerBlock(16,16);

    dim3 blocksPerGrid(
        (colsB + 15) / 16,
        (rowsA + 15) / 16
    );

    // Launch kernel
    matrixMul<<<blocksPerGrid, threadsPerBlock>>>(
        d_A, d_B, d_C,
        rowsA, colsA, colsB
    );

    // Copy result back
    cudaMemcpy(C, d_C, sizeC, cudaMemcpyDeviceToHost);

    // Print result matrix
    cout << "Result Matrix:\n";
    for(int i = 0; i < rowsA; i++) {
        for(int j = 0; j < colsB; j++) {
            cout << C[i * colsB + j] << " ";
        }
        cout << endl;
    }

    // Free GPU memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}