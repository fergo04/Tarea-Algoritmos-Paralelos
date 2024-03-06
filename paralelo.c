//Estudiantes: 
//Fernando González - 2021075114
//Emmanuel López - 2018077125

//Implementacion de Merge Sort paralelo

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 4  //Hilos utilizados

//Estructura del hilo
typedef struct {
    int *array;
    int left;
    int right;
    int *temp_array;
} SegmentThread;

int cmpfunc(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

void *thread_sort(void *arg) {
    SegmentThread *args = (SegmentThread *)arg;
    qsort(args->array + args->left, args->right - args->left + 1, sizeof(int), cmpfunc); //Uso del qsort para ordenar el segmento
    return NULL;
}

//Fusiona dos segmentos ordenados del arreglo
void merge(int *array, int left, int mid, int right, int *temp_array) {
    int i = left, j = mid, k = left;
    while (i < mid && j <= right) {
        temp_array[k++] = array[i] <= array[j] ? array[i++] : array[j++];
    }
    while (i < mid) {
        temp_array[k++] = array[i++];
    }
    while (j <= right) {
        temp_array[k++] = array[j++];
    }
    for (i = left; i <= right; i++) {
        array[i] = temp_array[i];
    }
}

//Ejecuta los hilos
void *thread_merge(void *arg) {
    SegmentThread *args = (SegmentThread *)arg;
    int mid = (args->left + args->right) / 2;
    if (mid < args->right) {
        merge(args->array, args->left, mid + 1, args->right, args->temp_array);
    }
    return NULL;
}

int main() {
    int X;
    printf("X = ");
    scanf("%d", &X);

    if (X <= 0) {
        printf("X debe ser positivo.\n");
        return 1;
    }

    //Asigna la cantidad de hilos dependiendo del número de elementos, esto en caso de que la X sea menor que la cantidad por default
    //Permite probar con valores de X que sean menores que los hilos utilizados
    int num_threads = MAX_THREADS > X ? X : MAX_THREADS;

    int *original_array = malloc(X * sizeof(int));
    int *array_to_sort = malloc(X * sizeof(int));
    int *temp_array = malloc(X * sizeof(int));

    srand(time(NULL));
    for (int i = 0; i < X; ++i) {
        original_array[i] = rand() % 2001 - 1000;
        array_to_sort[i] = original_array[i];
    }

    pthread_t threads[num_threads];
    SegmentThread thread_args[num_threads];
    int segment_size = X / num_threads;

    clock_t start_time, end_time;

    start_time = clock();

    for (int i = 0; i < num_threads; ++i) {
        thread_args[i].array = array_to_sort;
        thread_args[i].temp_array = temp_array;
        thread_args[i].left = i * segment_size;
        thread_args[i].right = (i == num_threads - 1) ? X - 1 : (i + 1) * segment_size - 1;
        pthread_create(&threads[i], NULL, thread_sort, &thread_args[i]);
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    for (int size = segment_size; size < X; size *= 2) {
        for (int left_start = 0; left_start < X; left_start += 2 * size) {
            int right_end = left_start + 2 * size - 1 < X ? left_start + 2 * size - 1 : X - 1;
            thread_args[left_start / (2 * size)].left = left_start;
            thread_args[left_start / (2 * size)].right = right_end;
            if (left_start / (2 * size) < num_threads) {
                pthread_create(&threads[left_start / (2 * size)], NULL, thread_merge, &thread_args[left_start / (2 * size)]);
            }
        }
        for (int i = 0; i < num_threads && thread_args[i].left < X; ++i) {
            if (i < num_threads) {
                pthread_join(threads[i], NULL);
            }
        }
    }

    end_time = clock();
    for (int i = 0; i < X; ++i) {
        printf("%d ", array_to_sort[i]);
    }
    printf("\n");

    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Tiempo de ordenamiento paralelo: %f segundos\n", time_taken);

    free(original_array);
    free(array_to_sort);
    free(temp_array);

    return 0;
    //Last update ..
}

