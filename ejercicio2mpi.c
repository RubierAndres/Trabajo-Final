#include <stdio.h>
#include <mpi.h>

#define RECTAS 20000000

int main(int argc, char** argv) {
    int rango, tamano, rectas;
    double medio, alto, ancho, area, total_area, sum = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rango);
    MPI_Comm_size(MPI_COMM_WORLD, &tamano);

    if (rango == 0) {
        rectas = RECTAS;
    }

    MPI_Bcast(&rectas, 1, MPI_INT, 0, MPI_COMM_WORLD); 
    ancho = 1.0 / (double) rectas;
    for (long i = rango; i < rectas; i += tamano) {
        medio = (i + 0.5) * ancho;
        alto = 4.0 / (1.0 + medio * medio);
        sum += alto;
    }
    
    area = ancho * sum;
    printf("Resultado del procesador %d: %f\n", rango, area);
    MPI_Reduce(&area, &total_area, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rango == 0) {
        printf("Resultado final: %f\n", total_area);
    }

    MPI_Finalize();
    return 0;
}