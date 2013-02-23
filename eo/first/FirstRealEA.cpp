#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdexcept>
#include <iostream>

#include <eo>
#include <es.h>

#include "real_value.h"

// REPRESENTATION
typedef eoReal<double> Indi;

using namespace std;

void main_function(int argc, char **argv)
{
    const unsigned int SEED = 42;	// seed for random number generator
    const unsigned int T_SIZE = 3; // size for tournament selection
    const unsigned int VEC_SIZE = 8; // Number of object variables in genotypes
    const unsigned int POP_SIZE = 20; // Size of population

    const unsigned int MAX_GEN = 500; // Maximum number of generation before STOP
    const unsigned int MIN_GEN = 10;  // Minimum number of generation before ...
    const unsigned int STEADY_GEN = 50; // stop after STEADY_GEN gen. without improvelent

    const float P_CROSS = 0.8;	// Crossover probability
    const float P_MUT = 0.5;	// mutation probability

    const double EPSILON = 0.01;	// range for real uniform mutation
    double SIGMA = 0.3;	    	// std dev. for normal mutation

    // some parameters for chosing among different operators
    const double hypercubeRate = 0.5;     // relative weight for hypercube Xover
    const double segmentRate = 0.5;  // relative weight for segment Xover
    const double uniformMutRate = 0.5;  // relative weight for uniform mutation
    const double detMutRate = 0.5;      // relative weight for det-uniform mutation
    const double normalMutRate = 0.5;   // relative weight for normal mutation

    rng.reseed(SEED);

    // Fitness function
    eoEvalFuncPtr<Indi, double, const vector<double>& > eval(  real_value );

    // Initilisation of population
    eoUniformGenerator<double> uGen(-1.0, 1.0);
    eoInitFixedLength<Indi> random(VEC_SIZE, uGen);
    eoPop<Indi> pop(POP_SIZE, random);

    // and evaluate it in one loop
    apply<Indi>(eval, pop);	// STL syntax

    // OUTPUT

    pop.sort();
    cout << "Initial Population" << endl;
    cout << pop;

    // selection and replacement
    eoDetTournamentSelect<Indi> selectOne(T_SIZE);
    eoSelectPerc<Indi> select(selectOne);// by default rate==1
    eoGenerationalReplacement<Indi> replace;

    // The variation operators
    eoSegmentCrossover<Indi> xoverS;
    eoHypercubeCrossover<Indi> xoverA;
    eoPropCombinedQuadOp<Indi> xover(xoverS, segmentRate);
    xover.add(xoverA, hypercubeRate);

    eoUniformMutation<Indi>  mutationU(EPSILON);
    eoDetUniformMutation<Indi>  mutationD(EPSILON);
    eoNormalMutation<Indi>  mutationN(SIGMA);
    eoPropCombinedMonOp<Indi> mutation(mutationU, uniformMutRate);
    mutation.add(mutationD, detMutRate);
    mutation.add(mutationN, normalMutRate, true);

    // STOP
    eoGenContinue<Indi> genCont(MAX_GEN);
    eoSteadyFitContinue<Indi> steadyCont(MIN_GEN, STEADY_GEN);
    eoFitContinue<Indi> fitCont(0);
    eoCombinedContinue<Indi> continuator(genCont);
    continuator.add(steadyCont);
    continuator.add(fitCont);

    eoSGATransform<Indi> transform(xover, P_CROSS, mutation, P_MUT);

    // the algorithm
    eoEasyEA<Indi> gga(continuator, eval, select, transform, replace);

    cout << "\n        Here we go\n\n";
    gga(pop);

    pop.sort();
    cout << "FINAL Population\n" << pop << endl;
}

int main(int argc, char **argv)
{
    try
    {
	main_function(argc, argv);
    }
    catch(exception& e)
    {
	cout << "Exception: " << e.what() << '\n';
    }

    return 1;
}
