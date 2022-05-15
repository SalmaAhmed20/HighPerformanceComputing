#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <stdbool.h>
#include "mpi.h" //extension
bool isprime(int n)
{
    int x = 0;
    if (n == 1 || n == 0) //1 and 0 not primes
        return 0;
    for (x = 2; x <n; x++)
    {
        if (n % x == 0)
            return 0;
    }
    return 1;
}
int main(int argc, char * argv[])
{
    int my_rank;    /* rank of process */
    int p;          /* number of process */
    MPI_Status status; /* return status for */

    int upperBound;
    int lowerBound;
    int i,total = 0,my_count=0 ;
    int portion;
    /* Start up MPI */
    MPI_Init( &argc, &argv );
    /* Find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    /* Find out number of process */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (my_rank == 0)
    {
        /*first phase calculate the number in range*/
        /*calculate size of data  assigned to each core*/
        /*send the boundaries*/
        printf("Enter Lower Bound: " );
        scanf("%d",&lowerBound);
        printf("\nEnter Upper Bound: " );
        scanf("%d",&upperBound);

        portion = (upperBound - lowerBound) / p;
        for (i = 1 ; i < p ; i++)
        {
            MPI_Send( &portion, 1, MPI_INT, i, 0,MPI_COMM_WORLD);
            MPI_Send( &lowerBound, 1, MPI_INT, i, 0,MPI_COMM_WORLD);
        }
        printf("\nP %d  ",my_rank);
        for(i=lowerBound ; i <= lowerBound+portion; i++)
        {

            if(isprime(i))
            {
                printf("%d  ",i);
                total++;
            }
        }
        printf("\nTotal number of prime numbers in %d are : %d ",my_rank, total);
        for(i = 1 ; i < p ; i++)
        {
            MPI_Recv(&my_count, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            total +=my_count;
        }
        printf("\nTotal number of prime numbers are %d\n", total);
    }
    else
    {
        //Recive boundaries
        MPI_Recv(&portion, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&lowerBound, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("\nP %d ",my_rank);

        for (i = (my_rank) * portion + lowerBound+1; i <= (my_rank) * portion + lowerBound + portion; i++)
        {
            if (isprime(i))
            {
                printf("%d  ",i);
                my_count++;
            }
        }
        printf("\nTotal number of prime numbers in %d are : %d ",my_rank, my_count);
        MPI_Send(&my_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
