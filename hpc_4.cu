#include <iostream>
#include <vector>
using namespace std;

// CUDA code to multiply matrices
__global__ void multiply(int* A, int* B, int* C, int size) {
    // Uses thread indices and block indices to compute each element
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < size && col < size) {
        int sum = 0;
        for (int i = 0; i < size; i++) {
            sum += A[row * size + i] * B[i * size + col];
        }
        C[row * size + col] = sum;
    }
}

void initialize(vector<vector<int>>& matrix, int size) {
    cout << "Enter the elements of the matrix (" << size << "x" << size << "):\n";
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            cin >> matrix[row][col];
        }
    }
}

void print(int* matrix, int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            cout << matrix[row * size + col] << " ";
        }
        cout << '\n';
    }
    cout << '\n';
}

int main() {
    int* A, * B, * C;

    int N;
    cout << "Enter the size of the square matrices: ";
    cin >> N;

    vector<vector<int>> A_vec(N, vector<int>(N));
    vector<vector<int>> B_vec(N, vector<int>(N));
    vector<vector<int>> C_vec(N, vector<int>(N));

    initialize(A_vec, N);
    initialize(B_vec, N);
    cout << "Matrix A: \n";
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            cout << A_vec[row][col] << " ";
        }
        cout << '\n';
    }
    cout << '\n';

    cout << "Matrix B: \n";
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            cout << B_vec[row][col] << " ";
        }
        cout << '\n';
    }
    cout << '\n';

    int matrixSize = N * N;
    size_t matrixBytes = matrixSize * sizeof(int);

    A = new int[matrixSize];
    B = new int[matrixSize];
    C = new int[matrixSize];

    // Copy data from vectors to arrays
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = A_vec[i][j];
            B[i * N + j] = B_vec[i][j];
        }
    }

    int* X, * Y, * Z;
    // Allocate space
    cudaMalloc(&X, matrixBytes);
    cudaMalloc(&Y, matrixBytes);
    cudaMalloc(&Z, matrixBytes);

    // Copy values from A to X
    cudaMemcpy(X, A, matrixBytes, cudaMemcpyHostToDevice);

    // Copy values from A to X and B to Y
    cudaMemcpy(Y, B, matrixBytes, cudaMemcpyHostToDevice);

    // Threads per CTA dimension
    int THREADS = 2;

    // Blocks per grid dimension (assumes THREADS divides N evenly)
    int BLOCKS = N / THREADS;

    // Use dim3 structs for block  and grid dimensions
    dim3 threads(THREADS, THREADS);
    dim3 blocks(BLOCKS, BLOCKS);

    // Launch kernel
    multiply<<<blocks, threads>>>(X, Y, Z, N);

    cudaMemcpy(C, Z, matrixBytes, cudaMemcpyDeviceToHost);
    cout << "Multiplication of matrix A and B: \n";
    print(C, N);

    delete[] A;
    delete[] B;
    delete[] C;

    cudaFree(X);
    cudaFree(Y);
    cudaFree(Z);

    return 0;
}