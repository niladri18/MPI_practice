#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include "mpi.h"


using namespace std;

const int n = 10;
int **A;
//int **newA;
int **up;
int **dn;
const int nbrx[4] = {1,0,-1,0};
const int nbry[4] = {0,1,0,-1};
//int rank,size;



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

	//return A;	
}


void decompose()
{
	for (int i= 0; i < n/2 +1; i++)
	{
		for (int j = 0; j < n; j++)	
		{
			up[i][j] = A[i][j];
			dn[i][j] = A[n/2-1+i][j];
		}
	}

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

void show(int **A)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
{
			//printf(" %d,",A[i][j]);
			cout << A[i][j];
		}
		//printf("\n");
		cout << "" <<endl;
	}
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

	
	printf("Processor %d : Hello world!\n",myrank, size);	
	/* Initialize the population */
	initialize(A);
	show(A);
	/*
	decompose();
	*/
	int live_cells;
	
	/* Iterate for updates */
	
	for (int iter = 0; iter < 5; iter++)
	{
		live_cells = update(A);
		cout<<live_cells<<endl;
				
	}

	/* show final configuration */
	show(A);

	/* Finalize MPI */
	MPI_Finalize();
	return 0;
}
