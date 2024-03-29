#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "C_Kernel.h"

extern void asm_domath(long long int n, float a, float x[], float y[], float z[]);
//extern float asm_domath(float a, float x, float y);
#define RANDMAX 100

int main(int argc, char* argv[]) {
    int m[3] = { 20, 24, 30 };
    for (int j = 0; j < 3; j++) {
    
    //int m[1] = { 20 };
    //for (int j = 0; j < 1; j++) {
        long long n = 1LL << m[j];
        int correct[30];
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

            clock_t start, end, total = 0;
            printf("Generating random numbers...\n");
            start = clock();
            for (long long i = 0; i < n; i++) {
                X[i] = (float)rand() / (RANDMAX * 100);
                Y[i] = (float)rand() / (RANDMAX * 100);
                Z[i] = 0.0f;
                Z2[i] = 0.0f;
            }
            end = clock();
            printf("Done generating random numbers\n");
            printf("Generated in %ld ms\n\n", (end - start) * 1000 / CLOCKS_PER_SEC);

            printf("Processing C kernel\n");

            start = clock();
            /*
            for (long long i = 0; i < n; i++) {
                Z[i] = c_domath2(A, X[i], Y[i]);
            }
            */

            c_domath(n, A, X, Y, Z);
            
            end = clock();
            printf("Total C Kernel time: %ld ms\n\n", (end - start) * 1000 / CLOCKS_PER_SEC);
            time[count][0] = end - start;

            printf("Processing ASM kernel\n");
            start = clock();
            /*
            for (long long i = 0; i < n; i++) {
                Z2[i] = asm_domath(A, X[i], Y[i]);
            }
            */

            asm_domath(n, A, X, Y, Z2);
            end = clock();
            printf("Total ASM Kernel time: %ld ms\n\n", (end - start) * 1000 / CLOCKS_PER_SEC);
            time[count][1] = end - start;

            int error = -1;
            // check if correct
            for (long long i = 0; i < n; i++) {
                if (Z[i] != Z2[i]) {
                    printf("Error: Z[%lld] = %f, Z2[%lld] = %f\n", i, Z[i], i, Z2[i]);
                    correct[count] = 0;
                    error = 1;
                    break;
                }
            }
            if (error == -1) {
                printf("No error found\n");
                for (int i = 0; i < 10; i++) {
                    printf("Z[%d] = %f, Z2[%d] = %f\n", i, Z[i], i, Z2[i]);
                }
                correct[count] = 1;
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
        
        char filename_c[20]; 
        char filename_asm[20]; 
        snprintf(filename_c, sizeof(filename_c), "R_C_Kernel_%d.txt", m[j]);
        snprintf(filename_asm, sizeof(filename_asm), "RR_ASM_Kernel_%d.txt", m[j]);
        
        for (int i = 0; i < max; i++) {
            aveTime1 += time[i][0];
            aveTime2 += time[i][1];

            FILE* file_c = fopen(filename_c, "a");
            FILE* file_asm = fopen(filename_asm, "a");

            fprintf(file_c, "%d,%d,%d\n", i, correct[i], time[i][0]);
            fclose(file_c);

            fprintf(file_asm, "%d,%d\n",i, time[i][1]);
            fclose(file_asm);
        }
        aveTime1 = (aveTime1 * 1000) / (max * CLOCKS_PER_SEC);
        aveTime2 = (aveTime2 * 1000) / (max * CLOCKS_PER_SEC);

        FILE* file_c = fopen(filename_c, "a");
        FILE* file_asm = fopen(filename_asm, "a");

        fprintf(file_c, "\nAverage time: %ld ms\n", aveTime1);
        fclose(file_c);

        fprintf(file_asm, "\nAverage time: %ld ms\n", aveTime2);
        fclose(file_asm);

        printf("Average time for C kernel: %ld ms\n", aveTime1);
        printf("Average time for ASM kernel: %ld ms\n", aveTime2);
    }

    return 0;
}
