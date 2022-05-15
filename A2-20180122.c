/*
How to run
mount -t vboxsf ... /...
cd /...
mpd &
mpicc A2-20180122.c -lm
mpiexec -np .. ./a.out
*/
#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "mpi.h"
int main(int argc, char  *argv[]) {
  int my_rank;    /* rank of process */
  int p;          /* number of process */
  MPI_Status status; /* return status for */

  int n ;  /*number of elements per each process).*/
  int i ; //counter
  time_t t;
  int *local_Arr;
  int localSum=0, global_Sum=0;
  float localDifference=0,globalDifference=0;
  float global_Mean=0,Result=0;

  /* Start up MPI */
  MPI_Init( &argc, &argv );
  /* Find out process rank */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  /* Find out number of process */
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  if (my_rank == 0)
  {
    printf("Enter number of elements per each process: " );
    scanf("%d",&n );
  }
  MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
  if (my_rank !=0) {
    srand((unsigned) time(&t));
    printf("Processor %d generate :",my_rank );
    local_Arr = malloc (n*sizeof(int));
    for ( i = 0; i < n; i++) {
      local_Arr[i]= rand() % 100+1;
      printf("%d ", local_Arr[i] );
      localSum+=local_Arr[i];
    }
printf("\n");

  }
  MPI_Allreduce(&localSum,&global_Sum,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
  if(my_rank!=0)
  {
    global_Mean=global_Sum/(n*(p-1));
    for(i=0;i<n;i++){
            localDifference += pow(local_Arr[i]-global_Mean,2);
        }
  }
  MPI_Reduce (&localDifference,&globalDifference,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
  if(my_rank==0){
        Result = globalDifference / (n*(p-1));
        Result= sqrt((double)Result);
        printf(" Result is %f\n",Result);
    }
 MPI_Finalize();
 return 0;
}
