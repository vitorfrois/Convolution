/**
    Convolução - Trabalho 1
    
    Ana Cristina 
    Davi Fagundes
    Eduardo Fernandes
    Lucas Claros
    Vítor Fróis - 12543440
**/


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
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
    
    // Apply convolution
    float sum;
    for(int i = paddingSize; i < N + paddingSize; i++){
        for(int j = paddingSize; j < N + paddingSize; j++){
            sum = 0;
            for(int k = 0; k < M; k++){
                for(int l = 0; l < M; l++){
                    sum += matrix[i - paddingSize + k][j - paddingSize + l] * kernel[k][l];
                }
            }
            newMatrix[i - paddingSize][j - paddingSize] = sum < 255? (int) sum : 255;
        }
    }

    // Print matrix with padding
    for(int i = 0; i < paddedMatrixSize; i++){
        for(int j = 0; j < paddedMatrixSize; j++){
            printf("%3d ", matrix[i][j]);        
        }
        printf("\n");
    }
    printf("\n");


    // Print kernel
    for(int i = 0; i < M; i++){
        for(int j = 0; j < M; j++){
            printf("%1.1f ", kernel[i][j]);        
        }
        printf("\n");
    }
    printf("\n");

    // Print newMatrix
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%3.0f ", newMatrix[i][j]);        
        }
        printf("\n");
    }

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


    return 0;
}