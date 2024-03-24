 #include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern float asm_domath(float a, float x, float y);

#define RANDMAX 100

int main(int argc, char* argv[]) {
    int m = 30; // 20, 24, 30
    long long n = 1LL << m;
    int max = 30;
    int count = 0;
    int temp;
    int time[30][2];
    do {
        float A = 0.0f;

        A = (float)rand();

        float* X = (float*)malloc(n * sizeof(float));
        float* Y = (float*)malloc(n * sizeof(float));
        float* Z = (float*)malloc(n * sizeof(float));
        float* Z2 = (float*)malloc(n * sizeof(float));
        Y[0] = 0.0f;
        Z[0] = 0.0f;
        Z2[0] = 0.0f;
        clock_t start, end, total = 0;
        printf("Generating random numbers...\n");
        start = clock();
        for (long long i = 0; i < n; i++) {
            X[i] = (float)rand() / (RANDMAX*100);
            Y[i] = (float)rand() / (RANDMAX*100);
        }
        end = clock();
        printf("Done generating random numbers\n");
        printf("Generated in %ld ms\n\n", (end - start) * 1000 / CLOCKS_PER_SEC);

        printf("Processing C kernel\n");
        start = clock();
        for (long long i = 0; i < n; i++) {
            Z[i] = A * X[i] + Y[i];
        }
        end = clock();
        printf("Total C Kernel time: %ld ms\n\n", (end - start) * 1000 / CLOCKS_PER_SEC);
        time[count][0] = end - start;

        printf("Processing ASM kernel\n");
        start = clock();
        for (long long i = 0; i < n; i++) {
            Z2[i] = asm_domath(A, X[i], Y[i]);
        }
        end = clock();
        printf("Total ASM Kernel time: %ld ms\n\n", (end - start) * 1000 / CLOCKS_PER_SEC);
        time[count][1] = end - start;

        int error = -1;
        // check if correct
        for (long long i = 0; i < n; i++) {
            if (Z[i] != Z2[i]) {
                printf("Error: Z[%lld] = %f, Z2[%lld] = %f\n", i, Z[i], i, Z2[i]);
                error = 1;
                break;
            }
        }
        if (error == -1) {
            printf("No error found\n");
            for (int i = 0; i < 10; i++) {
                printf("Z[%d] = %f, Z2[%d] = %f\n", i, Z[i], i, Z2[i]);
            }
        }
        printf("\n");

        free(X);
        free(Y);
        free(Z);
        free(Z2);
        count++;
    } while (count < max);

    long aveTime1 = 0;
    long aveTime2 = 0;
    for (int i = 0; i < max; i++) {
        aveTime1 += time[i][0];
        aveTime2 += time[i][1];
        FILE* file = fopen("C_Kernel.txt", "a");
        fprintf(file, "%d,",  time[i][0]);
        fclose(file);
        FILE* file2 = fopen("ASM_Kernel.txt", "a");
        fprintf(file2, "%d,", time[i][1]);
        fclose(file2);
    }
    aveTime1 = (aveTime1 * 1000) / (max * CLOCKS_PER_SEC);
    aveTime2 = (aveTime2 * 1000) / (max * CLOCKS_PER_SEC);

    FILE* file = fopen("C_Kernel.txt", "a");
    fprintf(file, "Average time: %ld ms\n", aveTime1);
    fclose(file);

    FILE* file2 = fopen("ASM_Kernel.txt", "a");
    fprintf(file2, "Average time: %ld ms\n", aveTime2);
    fclose(file2);

    printf("Average time for C kernel: %ld ms\n", aveTime1);
    printf("Average time for ASM kernel: %ld ms\n", aveTime2);

    return 0;
}
