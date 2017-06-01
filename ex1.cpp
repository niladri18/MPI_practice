#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;


int main()
{

	int n = 4;
	int A[n][n];
	int newA[n][n];
	double x;

	//Initialize
	
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j ++)
		{
			x = (double) rand()/RAND_MAX;
			if (x > 0.5)
			{
				A[i][j] = 1;
			}
			else
			{
				A[i][j] = 0;
			}
		}
	}
	
	
	// Game of life algorithm
	int nbrx[4] = {1,0,-1,0};
	int nbry[4] = {0,1,0,-1};
	int k1;
	int k2;
	
	for (int iter = 0; iter < 100; iter++)
	{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			//printf("%d, %d \n",i,j);
			int s = 0;
			for (int k = 0; k < 4; k++)
			{
				k1 = (i + n + nbrx[k])%n;
				k2 = (j + n + nbry[k])%n;
				//printf("\t %d, %d \n",k1,k2);
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
			//cout << A[i][j] << endl;
		}
	}
	
	
	//update A
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			A[i][j] = newA[i][j];
		}
	}
	
	cout << **(A+3)  << endl;
	
	}
	
	// Show A after equilibrium
	
	for (int i = 0;  i<n; i ++)
	{
		for (int j = 0; j < n; j ++)
		{
			//cout << A[i][j] << endl;
		}
	}
	
	
	return 0;
}
