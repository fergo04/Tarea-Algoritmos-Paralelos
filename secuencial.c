//Estudiantes: 
//Fernando González - 2021075114
//Emmanuel López - 2018077125

//Implementacion de Merge Sort secuencial

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    //Elementos restantes del primer subarray
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    //Elementos restantes del segundo subarray
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

//Implementacion recursiva Merge Sort
void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

int main() {
    int X, i;
    printf("X = ");
    scanf("%d", &X);

    if (X <= 0) {
        printf("X debe ser positivo.\n");
        return 1;
    }

    int vector[X];
    srand(time(NULL));
    for (i = 0; i < X; i++) {
        vector[i] = (rand() % 2001) - 1000;
    }

    int vector_ordenado[X];
    for (i = 0; i < X; i++) {
        vector_ordenado[i] = vector[i]; //Vector clonado (el que se va a ordenar)
    }

    clock_t inicio, fin;
    double tiempo;

    inicio = clock();
    mergeSort(vector_ordenado, 0, X - 1);
    fin = clock();

    tiempo = ((double) (fin - inicio)) / CLOCKS_PER_SEC;

    printf("\nVector ordenado: ");
    for (i = 0; i < X; i++) {
        printf("%d ", vector_ordenado[i]);
    }
    printf("\nTiempo de ordenamiento secuencial: %f segundos\n", tiempo);

    return 0;
    //Last update ..
}

