#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char** argv)
{
    int i, t, *m, sum = 0, max = -999, dest, answer;
    double average, var = 0, temp = 0, *d;
    int size; // number of processors
    int rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        printf("How many numbers you want to find: ");
        scanf("%d", &t);
        while (t <= 0)
        {
            printf("Invalid input. Please enter a positive number: ");
            scanf("%d", &t);
        }

        m = (int *)malloc(t * sizeof(int));
        d = (double *)malloc(t * sizeof(double));

        for (i = 0; i < t; i++)
        {
            printf("\nGive me a number: ");
            scanf("%d", &m[i]);
            if (m[i] < 0)
            {
                printf("You gave a negative number!! This number will change into positive..\n");
                m[i] *= -1;
            }

            sum = sum + m[i];
            if (m[i] > max)
            {
                max = m[i];
            }
        }

        average = (double)sum / t;
        for (i = 0; i < t; i++)
        {
            temp = m[i] - average;
            var = ((temp * temp) + var) / t;
        }
        for (i = 0; i < t; i++)
        {
            d[i] = (m[i] - max) * (m[i] - max);
        }

        for (dest = 1; dest < size; dest++)
        {
            MPI_Send(&t, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
            MPI_Send(&average, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
            MPI_Send(&max, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
            MPI_Send(&var, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
        }

        while (1)
        {
            printf("---MENU---\n");
            printf("1. Press 1 to get the average\n");
            printf("2. Press 2 to get the max value\n");
            printf("3. Press 3 to get the var value\n");
            printf("4. Press 4 to get the vector\n");
            printf("5. Press 5 to Exit\n");
            printf("\nYou typed:");
            scanf("%d", &answer);

            switch (answer)
            {
            case 1:
                printf("\nThis is the average: %.2lf\n", average);
                break;
            case 2:
                printf("The max of the numbers you gave is: %d\n", max);
                break;
            case 3:
                printf("The var equals to: %.2lf\n", var);
                break;
            case 4:
                for (i = 0; i < t; i++)
                {
                    printf("The vector is: %.2lf\n", d[i]);
                }
                break;
            case 5:
                free(m);
                free(d);
                printf("Bye\n");
                MPI_Finalize();
                return 0;

            default:
                printf("Wrong Input..\n");
            }
        }
    }
    else
    {
        MPI_Recv(&t, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&average, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&max, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&var, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        MPI_Finalize();
    }

    return 0;
}
