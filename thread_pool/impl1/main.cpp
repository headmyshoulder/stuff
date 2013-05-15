/*
 * main.cpp
 * Date: 2013-04-05
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <iostream>

#include "ThreadPool.h"

#define tab "\t"

using namespace std;



struct CalculationDataStruct
{
int inputVal;
double outputVal;
};

void LongCalculation( void *theSums )
{
    CalculationDataStruct *sums = (CalculationDataStruct *)theSums;

    int terms = sums->inputVal;
    double sum;
    for(int i=1; i<terms; i++)
        sum += 1.0/( double(i)*double(i) );
    sums->outputVal = sum;
}


int main(int argc, char** argv)
{ 
    int numThreads = 10;

    // Create pool
    ThreadPool threadPool;
    threadPool.CreateThreads(numThreads);

    // Create thread workspace
    CalculationDataStruct sums[3*numThreads];

    // Set up jobs
    for(int i=0; i<3*numThreads; i++)
    {
        sums[i].inputVal = 3000*(i+1);
        threadPool.SubmitJob(LongCalculation, &sums[i]);
    }

    // Run the jobs
    threadPool.StartJobs();
    threadPool.WaitForJobsToComplete();

    // Print results
    for(int i=0; i<3*numThreads; i++)
        std::cout << "Sum of " << sums[i].inputVal << " terms is " << sums[i].outputVal << std::endl;

    return 0;
}
