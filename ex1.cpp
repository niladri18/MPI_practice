#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include "mpi.h"


using namespace std;


/* Global variables */
const int n = 20;
int **A;
int **up;
int **dn;
int ***half;
const int nbrx[4] = {1,0,-1,0};
const int nbry[4] = {0,1,0,-1};



void initialize(int **A)
{
	
	/* Initialize A */
	double x;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			x = (double) rand()/(RAND_MAX);
			if (x<0.5)
			{
				A[i][j]= 0;
			}
			else
			{
				A[i][j] = 1;
			}
		}
	}

}


void show(int **arr, int row, int col)
{
	/* Print a 2D array with dimension row x col */
        for (int i = 0; i < row; i++)
        {
                for (int j = 0; j < col; j++)
		{
                       	cout << arr[i][j]<<" ";
                }
                        cout << "" <<endl;
        }
}                       

void decompose(int **A, int **up, int **dn)
{
	/* Non parallel domain decomposition */
	for (int i= 0; i < n/2 +1; i++)
	{
		for (int j = 0; j < n; j++)	
		{
			up[i][j] = A[i][j];
			dn[i][j] = A[n/2-1+i][j];
		}
	}

}


void decompose_mpi(int **A, int ***arr, int rank)
{
	/* Parallel domain decomposition */
        for (int i= 0; i < n/2 +1; i++)
        {
                for (int j = 0; j < n; j++)
                {
			int k = rank*(n/2-1);
                        arr[rank][i][j] = A[k + i][j];
                }
        }
	/* Show the two domains */
	//show(arr[rank], n/2+1, n);
}


int update(int **A)
{

	/* Allocate newA */
	int **newA;
	newA = new int *[n];
	for (int i = 0; i < n; i++)
	{
		newA[i] = new int [n];	
	}
	/* Game of life algorithm */
	int k1;
	int k2;
	int live_cells;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			//printf("%d, %d \n",i,j);
			int s = 0;
			/* Generate neighbors */
			for (int k = 0; k < 4; k++)
			{
				k1 = (i + n + nbrx[k])%n;
				k2 = (j + n + nbry[k])%n;
				s = s + A[k1][k2];
			}
			if (s == 3)
			{
				newA[i][j] = 1;
			}
			else if (s == 2)
			{
				newA[i][j] = A[i][j];
			}
			else
			{
				newA[i][j] = 0;
			}
		}
	}

	
	
	
	/* update A  and calculate number of living cells*/
	live_cells = 0;
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			A[i][j] = newA[i][j];
			if (newA[i][j] > 0)
			{
				live_cells += 1;
			}
		}
	}
	
	return live_cells;
}


void clean(int **arr, int row, int col)
{
	for(int i = 0; i < col; i++)
	{
		delete[] arr[i];
	}
	delete arr;
}


int main()
{
	int myrank, size;
	/* Initialize MPI */
	MPI_Init( NULL , NULL );

	/* Get total number of processors */
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	/* Get the rank of current processor */
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);


        /* Allocate A */
        A = new int *[n];
        for (int i = 0; i < n; i++)
        {
                A[i] = new int[n];
        }

        /* Allocate upper half of domain */
	half = new int **[2];
        up = new int *[n/2 + 1];
	half[0] = new int *[n/2 + 1];
	half[1] = new int *[n/2 + 1];
        for (int i = 0; i < n; i++)
        {
                up[i] = new int[n];
		half[0][i] = new int[n];
		half[1][i] = new int[n];
        }


        /* Allocate lower half of domain */
        dn = new int *[n/2 + 1];
        for (int i = 0; i < n; i++)
        {
                dn[i] = new int[n];
        }


	
	printf("Processor %d : Hello world! \n",myrank, size);	
	/* Initialize the population */
	initialize(A);
	if (myrank == 0) 
	{
		show(A,n,n);
	}
	//decompose(A,up,dn);

	/* Domain decomposition */	
	if (size > 1)
	{
		printf("halfdomain[%d]\n",myrank);
		decompose_mpi( A, half, myrank);
		/* Print the two domains */
		show(half[myrank], n/2+1, n);
		cout <<""<<endl;
	}
	else
	{	
		decompose( A, half[0] , half[1] );
		/* Print the two domains */
		for (int k = 0; k < 2; k++)
		{
			printf("halfdomain[%d]\n",k);
			show(half[k], n/2+1, n);	
		}
	}

	

	/* Show domain up and dn 
	show(up, n/2+1, n);
	cout <<""<<endl;
	show(dn, n/2+1, n);
	*/
	int live_cells;


	/* Iterate for updates */
	if (size < 2)
	{
		
		for (int iter = 0; iter < 5; iter++)
		{
			live_cells = update(A);
			cout<<live_cells<<endl;
				
		}
	}
	/* show final configuration */
	//show(A,n,n);
	
	/* Must clean the array memory */
	clean(A,n,n);
	clean(half[0], n/2+1, n);
	clean(half[0], n/2+1, n);

	/* Finalize MPI */
	MPI_Finalize();
	return 0;
}
