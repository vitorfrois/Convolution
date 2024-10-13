/**
    Convolução - Trabalho 1
    
    Ana Cristina 
    Davi Fagundes
    Eduardo Fernandes - 12681733
    Lucas Claros - 12682592
    Vítor Fróis - 12543440
**/

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int main(){
    int **matrix;
    float **kernel;
    float **newMatrix;
    int N, M, S;
    scanf("%d %d %d", &N, &M, &S);
    srand(S);

    int paddingSize = (int) (M - 1) / 2;
    int paddedMatrixSize = N + (2 * paddingSize);

    // Alloc and init matrix with calloc
    matrix = (int **)malloc(paddedMatrixSize * sizeof(int *));
    for (int i = 0; i < paddedMatrixSize; i++)
        matrix[i] = (int *)calloc(paddedMatrixSize, sizeof(int)); 

    for(int i = paddingSize; i < N + paddingSize; i++){
        for(int j = paddingSize; j < N + paddingSize; j++){
            matrix[i][j] = rand() % 256;        
        }
    }

    // Alloc and init kernel
    kernel = (float **)malloc(M * sizeof(float *));
    for (int i = 0; i < M; i++)
        kernel[i] = (float *)malloc(M * sizeof(float)); 

    for(int i = 0; i < M; i++){
        for(int j = 0; j < M; j++){
            kernel[i][j] = (float) (rand() % 10) / 10;        
        }
    }
    
    // Alloc new matrix
    newMatrix = (float **)malloc(N * sizeof(float *));
    for (int i = 0; i < N; i++)
        newMatrix[i] = (float *)malloc(N * sizeof(float)); 


    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        int is_last_thread = thread_id == num_threads - 1;

        // Start block decomposition
        // Determine the row range that this thread will process
        int block_height = N / num_threads;
        int start_row = thread_id * block_height;
        int end_row = is_last_thread ? N : start_row + block_height;

        for (int i = start_row; i < end_row; i++) {
            for (int j = 0; j < N; j++) {
                float sum = 0;
                #pragma omp simd reduction(+: sum)
                for (int k = 0; k < M; k++) {
                    for (int l = 0; l < M; l++) {
                        sum += matrix[i + k][j + l] * kernel[k][l];
                    }
                }
                newMatrix[i][j] = sum < 255 ? (int)sum : 255;
            }
        }
    }

    // Calculate the minimum and maximum value using reduction clause
    int min_value = 255, max_value = 0;
    #pragma omp parallel for reduction(min: min_value) reduction(max: max_value)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (newMatrix[i][j] < min_value) {
                min_value = newMatrix[i][j];
            }else if (newMatrix[i][j] > max_value) {
                max_value = newMatrix[i][j];
            }
        }
    }

    printf("%d %d\n", max_value, min_value);

    // Free allocated space
    for(int i = 0; i < paddedMatrixSize; i++)
        free(matrix[i]);
    free(matrix);

    for(int i = 0; i < M; i++)
        free(kernel[i]);
    free(kernel);

    for(int i = 0; i < N; i++)
        free(newMatrix[i]);
    free(newMatrix);
}