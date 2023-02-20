#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

int compare(const void *a, const void *b) {
int x = *(int *)a;
int y = *(int *)b;
return x - y;
}

int main(int argc, char *argv[]) {
int i, num, sum = 0, totalSum = 0;
int numeros[20000];
char nombre_archivo[] = "test.txt";
int rango, tamano;
srand(time(NULL));
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rango);
MPI_Comm_size(MPI_COMM_WORLD, &tamano);

for (i = 0; i < 20000; i++) {
num = rand()%40000;
sum += num;
numeros[i] = num;
}

MPI_Reduce(&sum, &totalSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

if (rango == 0) {
FILE *fp;
fp = fopen(nombre_archivo, "w");
if (fp == NULL) {
printf("No se pudo abrir el archivo.\n");
MPI_Finalize();
return 1;
}

qsort(numeros, 20000, sizeof(int), compare);

for (i = 0; i < 20000; i++) {

fprintf(fp, "%d\n", numeros[i]);
}
fprintf(fp, "El resultado de cada procesador es: %d\n", sum);
fprintf(fp, "La suma de los números es: %d\n", totalSum);
fclose(fp);
printf("El archivo se escribió correctamente.\n");
}
MPI_Finalize();
return 0;
}