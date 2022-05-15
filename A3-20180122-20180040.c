#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int id, np;
    int i, j ;
    int Row, Column;
    char ch;
    printf("Enter number of Rows :" );
    scanf("%d",&Row );
    printf("\nEnter number of Columns :" );
    scanf("%d",&Column );
    char *matrixArr = malloc (sizeof (char) * Row *Column);
    printf("Enter Matrix Element\n" );
    for ( i = 0; i < Row; i++)
    {
        printf("Row %d ", i);
        for ( j = 0; j < Column; j++)
        {
            getchar();
            scanf("%c",&ch);
            if (ch !='x'&& ch !='o')
            {
              printf("Invaild input\n" );
              exit(1);
            }
            matrixArr[i*Column+j]=ch;
        }
    }
    printf("Input: \n");
    for ( i = 0; i < Row; i++)
    {
        for ( j = 0; j < Column; j++)
        {
            printf("%c ",matrixArr[i*Column+j]  );
        }
        printf("\n");
    }
    #pragma omp parallel private(i,j) shared(matrixArr,Row,Column)
    {
        np = omp_get_num_threads();
        id = omp_get_thread_num();
        #pragma omp for schedule(dynamic)
        for (i = 0; i < Row; i++)
        {
            for ( j = 1; j < Column-1; j++)
            {
                if (matrixArr[i*Column+j]=='o')
                {
                    if(matrixArr[i*Column+(j-1)] == 'x' && matrixArr[i*Column+(j+1)]=='x')
                    {
                        matrixArr[i*Column+j]='x';
                        printf("Thread %d out of %d change ceil [%d][%d]\n", id,np,i,j);
                    }
                }
            }
        }
    }
    printf("Solve: \n");
    for ( i = 0; i < Row; i++)
    {
        for ( j = 0; j < Column; j++)
        {
            printf("%c ",matrixArr[i*Column+j]  );
        }
        printf("\n");
    }
}
