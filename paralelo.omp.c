#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <limits.h>
#define MIN_(a,b) ((a)<(b)?(a):(b))
#define MAX_(a,b) ((a)>(b)?(a):(b))

// Função que aplica o filtro (kernel) em um elemento de uma submatriz
void apply_filter_element(int N, int M, float** kernel, int** img, int** result, int i, int j) {
    int half_M = M / 2;
    double sum = 0.0;

    // Aplicação do kernel em torno do elemento [i][j]
    for (int ki = 0; ki < M; ki++) {
        for (int kj = 0; kj < M; kj++) {
            int img_x = i + ki - half_M;
            int img_y = j + kj - half_M;

            // Verifica se o ponto da imagem está dentro dos limites
            if (img_x >= 0 && img_x < N && img_y >= 0 && img_y < N) {
                sum += img[img_x][img_y] * kernel[ki][kj];
            }
        }
    }

    // Truncar o resultado e limitar ao intervalo [0, 255]
    int truncated_result = (int)round(sum);
    result[i][j] = MIN_(MAX_(truncated_result, 0), 255);
}

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

    float sum = 0;
    #pragma omp parallel for collapse(2) private(sum)
    for(int i = paddingSize; i < N + paddingSize; i++) {
        for(int j = paddingSize; j < N + paddingSize; j++) {
            // #pragma omp parallel for collapse(2) reduction(+:sum)
            for(int k = 0; k < M; k++) {
                for(int l = 0; l < M; l++) {
                    sum += matrix[i - paddingSize + k][j - paddingSize + l] * kernel[k][l];
                }
            }
            newMatrix[i - paddingSize][j - paddingSize] = sum < 255 ? (int) sum : 255;
        }
    }

    // Calcular o valor mínimo e máximo da matriz resultante
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
    printf("\n");
    printf("\n");
    printf("\n");
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