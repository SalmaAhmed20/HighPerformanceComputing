#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "math.h"
#include "mpi.h"
//Sum of Convergent Series algorithm
int main(int argc, char  *argv[])
{
    int my_rank;    /* rank of process */
    int p;          /* number of process */
    MPI_Status status; /* return status for */

    /* Start up MPI */
    MPI_Init( &argc, &argv );
    /* Find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    /* Find out number of process */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    int np,id;
    /*n is interval*/
    int n,i;
    double term,sum_local,Total_sum,my_sum=0.0,sum=0.0;
    while (n !=0)
    {
Total_sum=0.0;my_sum=0.0;sum=0.0;
        if(my_rank ==0)
        {
            printf("Enter Number of Interval:-(0 to exit) " );
            scanf ("%d",&n );
        }
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD );
        if(n==0)
            break;
        else
        {
            sum_local = 0.0;
            #pragma omp parallel private (i, sum_local)
            {

                np = omp_get_num_threads();
                id = omp_get_thread_num();
                #pragma omp for
                for ( i = my_rank + 1; i <= n ; i += p)
                {
                    term = (double) 1/(pow(2,i));
                    sum_local += term;
                    printf("my rank %d, Thread %d , out of thread %d : Sum = %lf \n", my_rank,id, omp_get_thread_num(),sum_local);
                }
                # pragma omp critical
                sum += sum_local ;

            }
            my_sum=sum;
            MPI_Reduce(& my_sum, &Total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );
            if ( my_rank == 0)
                printf ("Sum of Convergent Serie is approximatly :\n%.16f Error is: %.16f\n", Total_sum, fabs( 1.0 - Total_sum ));
        }

    }
    MPI_Finalize();
    return 0;
}
