#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "banker.h"


int main(int argc, char *argv[])
{
  // TODO: attempt to open scenario file and scan data into allocated structures

  if (argc != 2) {
    printf("Usage: ./bankers <scenario file>\n");
    return 0;
  }

  FILE * file;
  file = fopen(argv[1], "r");
  if (file == NULL) {
    printf("File not found: %s\n", argv[1]);
    return 0;
  }

  // allocate for contents of file
  int numResourceTypes;
  int numProcesses;
  fscanf(file, "%d", &numResourceTypes);
  fscanf(file, "%d", &numProcesses);

  //printf("Scanned resource types and processes\n");

  // vectors
  int *resource = (int *)malloc(numResourceTypes * sizeof(int));
  int *available = (int *)malloc(numResourceTypes * sizeof(int));

  // matrices
  int **maxDemand = (int **)malloc(numProcesses * sizeof(int *));
  int **allocation = (int **)malloc(numProcesses * sizeof(int *));
  int **need = (int **)malloc(numProcesses * sizeof(int *));

  for (int i = 0; i < numProcesses; i++) 
  {
    maxDemand[i] = (int *)malloc(numResourceTypes * sizeof(int));
    allocation[i] = (int *)malloc(numResourceTypes * sizeof(int));
    need[i] = (int *)malloc(numResourceTypes * sizeof(int));
  }

  //printf("Done allocating\n");
  
  // scan in contents
  int i, j; // val;
  for (i = 0; i < numResourceTypes; i++)
  {
    fscanf(file, "%d", &resource[i]);
    //resource[i] = val;
  }

  //printf("Scanned total resources\n");

  for (i = 0; i < numProcesses; i++)
  {
    for (int j = 0; j < numResourceTypes; j++)
    {
      fscanf(file, "%d", &maxDemand[i][j]);
      //fscanf(file, "%d", &val);
      //maxDemand[i][j] = val;
    }
  }

  //printf("Scanned max demand matrix\n");

  for (i = 0; i < numProcesses; i++)
  {
    for (int j = 0; j < numResourceTypes; j++)
    {
      fscanf(file, "%d", &allocation[i][j]);
      //fscanf(file, "%d", &val);
      //allocation[i][j] = val;
    }
  }

  /*
  printf("Scanned allocation matrix\n");

  printf("%d\n%d\n", numResourceTypes, numProcesses);
  printf("Total resources:\n");
  printVector(resource, numResourceTypes);
  printf("Max demand:\n");
  printMatrix(maxDemand, numProcesses, numResourceTypes);
  printf("Resources allocated:\n");
  printMatrix(allocation, numProcesses, numResourceTypes);
  */

  // sanity checks
  // check that currently allocated resources do not exceed total resources
  int sum;
  for (i = 0; i < numResourceTypes; i++) 
  {
    sum = 0;
    for (j = 0; j < numProcesses; j++)
    {
      sum += allocation[j][i];
    }
    if (sum > resource[i])
    {
      printf("Integrity test failed: allocated resources exceed total resources\n");
      free(resource);
      free(available);
      for (int i = 0; i < numProcesses; i++) 
      {
        free(maxDemand[i]);
        free(allocation[i]);
        free(need[i]);
      }
      free(maxDemand);
      free(allocation);
      free(need);
      return 0;
    }
  }

  // check that allocated resources do not exceed max demand
  for (i = 0; i < numProcesses; i++) 
  {
    for (j = 0; j < numResourceTypes; j++)
    {
      if (allocation[i][j] > maxDemand[i][j])
      {
        int diff = maxDemand[i][j] - allocation[i][j];
        printf("Integrity test failed: allocated resources exceed demand for Thread %d\n", i);
        printf("Need %d instances of resource %d\n", diff, j);
        free(resource);
        free(available);
        for (int i = 0; i < numProcesses; i++) 
        {
          free(maxDemand[i]);
          free(allocation[i]);
          free(need[i]);
        }
        free(maxDemand);
        free(allocation);
        free(need);
        return 0;
      }
    }
  }
  
  // calculate available vector and need matrix
  // available
  for (i = 0; i < numResourceTypes; i++) 
  {
    sum = 0;
    for (j = 0; j < numProcesses; j++)
    {
      sum += allocation[j][i];
    }
    available[i] = resource[i] - sum;
  }
  // need (max - alloc)
  for (i = 0; i < numProcesses; i++) 
  {
    for (j = 0; j < numResourceTypes; j++)
    {
      need[i][j] = maxDemand[i][j] - allocation[i][j];
    }
  }

  /*
  printf("Available resources:\n");
  printVector(available, numResourceTypes);
  printf("Resources needed:\n");
  printMatrix(need, numProcesses, numResourceTypes);
  */

  // TODO: Run banker's safety algorithm
  isSafe(available, allocation, need, numProcesses, numResourceTypes);
  

  // free stuff
  free(resource);
  free(available);
  for (int i = 0; i < numProcesses; i++) 
  {
    free(maxDemand[i]);
    free(allocation[i]);
    free(need[i]);
  }
  free(maxDemand);
  free(allocation);
  free(need);
  
  return 0;
}