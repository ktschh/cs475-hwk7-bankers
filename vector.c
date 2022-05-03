#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

// TODO - Define vector and matrix operations

void printVector(int *vector, int length)
{
    int i;
    for (i = 0; i < length; i++)
    {
        printf("%d ", vector[i]);
    }
    printf("\n");
}

void printMatrix(int **matrix, int length, int width)
{
	int i, j;
	for (i = 0; i < length; i++)
	{
		for (j = 0; j < width; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}

void cloneMatrix(int **matrix, int length, int width, int **clone)
{
	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < width; j++)
		{
			clone[i][j] = matrix[i][j];
		}
	}
}

void cloneVector(int *vector, int length, int *clone)
{
	for (int i = 0; i < length; i++)
	{
		clone[i] = vector[i];
	}
}

// compares two vectors, returns 1 if vector one is less than or equal to vector two, 0 otherwise
int compareVectors(int *vector_one, int *vector_two, int length)
{
	for (int i = 0; i < length; i++)
	{
		if (vector_one[i] > vector_two[i])
		{
			// return false
			return 0;
		}
	}
	// return true
	return 1;
}

// checks if a vector contains all 1s
int isFinished(int *vector, int length)
{
	for (int i = 0; i < length; i++)
	{
		if (vector[i] == 0)
		{
			// return false
			return 0;
		}
	}
	// no 0's found, return true
	return 1;
}