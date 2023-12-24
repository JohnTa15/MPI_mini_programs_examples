#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <stdbool.h>
#include <math.h>

int main(int argc, char** argv) {
    bool sdd = false; //sdd strictly diagonally dominant
    int *a, *b, i, j, n, x, size, rank = 0, dest, elem_diagon, max = -999;
    char answer[3];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0){
            printf("How many rows do you want = ");
            scanf("%d", &n);
            printf("How many columns do you want = ");
            scanf("%d", &x);

            a = (int *)malloc(n * x * sizeof(int));
	    b = (int *)malloc(n * x * sizeof(int));

            for (i = 0; i < n; i++) {
                for (j = 0; j < x; j++) {
                    printf("Element [%d][%d]: ", i, j);
                    scanf("%d", &a[i * x + j]);
		    a[i * x + j] = fabs(a[i * x + j]); //all the elements now they are positive ONLY
                }
            }

            for (i = 0; i < n; i++) {
    		elem_diagon = a[i * x + i]; //this is for the main diagonal..
		max = elem_diagon; 
		if(max < elem_diagon)
			max = elem_diagon;
		int sum = 0;
		for (j = 0; j < x; j++) {
		    if (j != i) {
		       sum += a[i * x + j]; //the sum that we will compare it with the main diagonal
        	    }
		}

	        if (elem_diagon >= sum) {
        	   sdd = true; 
		   for(i = 0; i < n; i++){ 
		  	for(j = 0; j < x; j ++){
		  		if(i != j){
					b[i * x + j] = max - a[i * x + j];
				}
				else{
					b[i * x + j] = max;
				}
			}
	  	   }
	   	} 
	   }	

	    for(dest = 1; dest < size; dest++){
                MPI_Send(&n, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
                MPI_Send(&x, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
                MPI_Send(a, n * x, MPI_INT, dest, 0, MPI_COMM_WORLD); //n*x the loop to send all the array
		//MPI_Send(b, n * x, MPI_INT, dest, 0, MPI_COMM_WORLD);
            }
            
            while (1) {
                printf("Do you want to show the current data (YES OR NO): ");
                scanf("%s", &answer);

                if (strcmp(answer, "YES") == 0 || strcmp(answer, "yes") == 0){
			if(sdd){
				printf("Yes the array is strictly diagonally dominant.\n");
				for(i = 0; i < n; i++){
					for(j = 0; j < x; j++){
						printf("a[%d][%d] = %d  ",i,j, a[i * x + j]);
						printf("b[%d][%d] = %d  ",i,j, b[i * x + j]);
					}
					printf("\n");
				}
				printf("\n");
			} else {
				printf("The array is not strictly diagonally dominant..\n");
				}
                } else if (strcmp(answer, "NO") == 0 || strcmp(answer, "no") == 0){ 
                    printf("Bye!\n");
                    free(a);
		    free(b);
                    MPI_Finalize();
                    return(0);
                } else {
                    printf("Wrong Input..\n");
                }
            }

            free(a);
    }
    else{ 
        MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	int *received_a = (int *)malloc(n * x * sizeof(int));
	int *received_b = (int *)malloc(n * x * sizeof(int));

        MPI_Recv(received_a, n * x, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Recv(received_b, n * x, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	free(received_a);
	free(received_b);
       	MPI_Finalize();
    }
    return 0;
}
