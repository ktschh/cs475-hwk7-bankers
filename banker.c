#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "banker.h"

// TODO - Safety Algorithm goes here
int isSafe(int *available, int **allocated, int **need, int numThreads, int numResources)
{
    // initialize
    int *work = (int *)malloc(numResources * sizeof(int));
    cloneVector(available, numResources, work);
    int *finish = (int *)malloc(numThreads * sizeof(int));
    for (int i = 0; i < numThreads; i ++)
    {
        finish[i] = 0;
    }
    

    // algorithm
    int exists = 1;
    int *order = (int *)malloc(numThreads * sizeof(int));
    int counter = 0;
    while (exists)
    {
        // nothing found yet
        int nothing = 1;
        for (int i = 0; i < numThreads; i++)
        {
            if (finish[i] == 0)
            {
                if (compareVectors(need[i], work, numResources))
                {
                    // found an unfinished thread that is able to be run
                    nothing = 0;
                    finish[i] = 1;
                    order[counter] = i;
                    counter++;
                    for (int j = 0; j < numResources; j++)
                    {
                        work[j] += allocated[i][j];
                    }
                    // check if all threads are finished
                    if (isFinished(finish, numThreads))
                    {
                        exists = 0;
                        break;
                    }
                }
            }
        }
        // didn't find a thread to run in the for loop, must be deadlock
        if (nothing)
        {
            exists = 0;
        }
    }
    
    //printVector(finish, numThreads);

    // check for safe/unsafe results
    if (isFinished(finish, numThreads)) // safe!
    {
        printf("SAFE: ");
        for (int i = 0; i < numThreads; i++)
        {
            printf("T%d ", order[i]);
        }
        printf("\n");
    }
    else // unsafe!
    {
        printf("UNSAFE: ");
        for (int i = 0; i < numThreads; i++)
        {
            if (finish[i] == 0)
            {
                printf("T%d ", i);
            }
        }
        printf("can't finish\n");
    }

    free(work);
    free(finish);
    free(order);


    return 0;
}