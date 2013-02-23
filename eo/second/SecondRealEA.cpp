#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <random>
#include <vector>

#include <eo>
#include <es.h>

using namespace std;

// define your individuals
typedef eoReal<eoMinimizingFitness> Indi;


template< class T >
T min3( T t1 , T t2 , T t3 )
{
    using std::min;
    return min( t1 , min( t2 , t3 ) );
}


template <class EOT>
class FitnessFunc : public eoEvalFunc< EOT >
{
    const std::vector< double > &m_x1;
    const std::vector< double > &m_x2;
    const std::vector< double > &m_y;

public:

    FitnessFunc( const std::vector< double > &x1 , const std::vector< double > &x2 , const std::vector< double > &y ) 
        : m_x1( x1 ) , m_x2( x2 ) , m_y( y )
    {
    }

    void operator()(EOT& _eo)
    {
        if (_eo.invalid())
        {
            double a1 = _eo[ 0 ];
            double a2 = _eo[ 1 ];
            double a3 = _eo[ 3 ];
            double a4 = _eo[ 4 ];
            double a5 = _eo[ 5 ];

            double chi2 = 0.0;
            size_t N = min3( m_x1.size() , m_x2.size() , m_y.size() );
            for( size_t i=0 ; i<N ; ++i )
            {
                double x1 = m_x1[i] , x2 = m_x2[i];
                double val = a1 * ( x1*x1 + x2*x2 ) + a2 * cos( a3 * x1 + a4 ) + a5;
                chi2 += ( val - m_y[i] ) * ( val - m_y[i] );
            }
            _eo.fitness( chi2 );
        }
    }
};

template< class Rng >
void create_data( std::vector< double > &x1 , std::vector< double > &x2 , std::vector< double > &y , size_t N , Rng &rng ,
                  double a1 = 0.25 , double a2 = 2.0 , double a3 = 1.0 , double a4 = 0.5 , double a5 = -3.3 )
{
    std::normal_distribution<> dist1( 0.0 , 1.0 ) , dist2( 0.0 , 1.0 );
    x1.resize( N ); 
    x2.resize( N ); 
    y.resize( N ); 
    for( size_t i=0 ; i<N ; ++i )
    {
        double x1_ = dist1( rng ) , x2_ = dist2( rng );
        x1[i] = x1_;
        x2[i] = x2_;
        y[i] = a1 * ( x1_ * x1_ + x2_ * x2_ ) + a2 * cos( a3 * x1_ + a4 ) + a5;
    }
}




void main_function(int argc, char **argv)
{
    // PARAMETRES

    eoParser parser(argc, argv);

    std::vector< double > x1 , x2 , y;
    std::mt19937 my_rng;
    create_data( x1 , x2 , y , 10000 , my_rng );


    unsigned seed = parser.createParam(unsigned(time(0)), "seed", "Random number seed", 'S').value(); // will be in default section General
    unsigned vecSize = 5;
    unsigned popSize = parser.createParam(unsigned(10), "popSize", "Population size",'P', "Evolution engine" ).value();
    unsigned tSize = parser.createParam(unsigned(2), "tSize", "Tournament size",'T', "Evolution Engine" ).value();
    string loadName = parser.createParam(string(""), "Load","A save file to restart from",'L', "Persistence" ).value();
    unsigned maxGen = parser.createParam(unsigned(100), "maxGen", "Maximum number of generations",'G', "Stopping criterion" ).value();
    unsigned minGen = parser.createParam(unsigned(100), "minGen", "Minimum number of generations",'g', "Stopping criterion" ).value();
    unsigned steadyGen = parser.createParam(unsigned(100), "steadyGen", "Number of generations with no improvement",'s', "Stopping criterion" ).value();

    double pCross = parser.createParam(double(0.6), "pCross", "Probability of Crossover", 'C', "Genetic Operators" ).value();
    double pMut = parser.createParam(double(0.1), "pMut", "Probability of Mutation", 'M', "Genetic Operators" ).value();
    double hypercubeRate = parser.createParam(double(1), "hypercubeRate", "Relative rate for hypercube crossover", '\0', "Genetic Operators" ).value();
    double segmentRate = parser.createParam(double(1), "segmentRate", "Relative rate for segment crossover", '\0', "Genetic Operators" ).value();
    double EPSILON = parser.createParam(double(0.01), "EPSILON", "Width for uniform mutation", '\0', "Genetic Operators" ).value();
    double SIGMA = parser.createParam(double(0.3), "SIGMA", "Sigma for normal mutation", '\0', "Genetic Operators" ).value();
    double uniformMutRate = parser.createParam(double(1), "uniformMutRate", "Relative rate for uniform mutation", '\0', "Genetic Operators" ).value();
    double detMutRate = parser.createParam(double(1), "detMutRate", "Relative rate for det-uniform mutation", '\0', "Genetic Operators" ).value();
    double normalMutRate = parser.createParam(double(1), "normalMutRate", "Relative rate for normal mutation", '\0', "Genetic Operators" ).value();
    string str_status = parser.ProgramName() + ".status"; // default value
    string statusName = parser.createParam(str_status, "status","Status file",'S', "Persistence" ).value();
    if (parser.userNeedsHelp())
    {
	parser.printHelp(cout);
	exit(1);
    }
    if (statusName != "")
    {
	ofstream os(statusName.c_str());
	os << parser;		// and you can use that file as parameter file
    }

    // Fitness function
    FitnessFunc<Indi> plainEval( x1 , x2 , y );
    eoEvalFuncCounter<Indi> eval(plainEval);

    // Initilisation of population
    eoPop<Indi> pop;
    eoState inState;		// a state for loading - WITHOUT the parser
    inState.registerObject(rng);
    inState.registerObject(pop);

    if (loadName != "")
    {
        inState.load(loadName);
    }
    else
    {
        rng.reseed(seed);
        eoUniformGenerator<double> uGen(-1.0, 1.0);
        eoInitFixedLength<Indi> random(vecSize, uGen);

        pop.append(popSize, random);
        apply<Indi>(eval, pop);
    }

    pop.sort();
    cout << "Initial Population" << endl;
    cout << pop;


    // The robust tournament selection
    eoDetTournamentSelect<Indi> selectOne(tSize);
    eoSelectPerc<Indi> select(selectOne);

    eoGenerationalReplacement<Indi> replace;

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

    eoSGATransform<Indi> transform(xover, pCross, mutation, pMut);

    // STOP
    eoGenContinue<Indi> genCont(maxGen);
    eoSteadyFitContinue<Indi> steadyCont(minGen, steadyGen);
    eoFitContinue<Indi> fitCont(0);
    eoCombinedContinue<Indi> continuator(genCont);
    continuator.add(steadyCont);
    continuator.add(fitCont);


    eoCheckPoint<Indi> checkpoint(continuator);

    // Create a counter parameter
    eoValueParam<unsigned> generationCounter(0, "Gen.");
    eoIncrementor<unsigned> increment(generationCounter.value());

    checkpoint.add(increment);

    eoBestFitnessStat<Indi> bestStat;
    eoSecondMomentStats<Indi> SecondStat;

    checkpoint.add(bestStat);
    checkpoint.add(SecondStat);

    eoStdoutMonitor monitor;

    checkpoint.add(monitor);

    monitor.add(generationCounter);
    monitor.add(eval);
    monitor.add(bestStat);
    monitor.add(SecondStat);

    eoFileMonitor fileMonitor("stats.xg", " ");

    checkpoint.add(fileMonitor);

    fileMonitor.add(generationCounter);
    fileMonitor.add(bestStat);
    fileMonitor.add(SecondStat);

    eoState outState;
    outState.registerObject(parser);
    outState.registerObject(pop);
    outState.registerObject(rng);

    eoCountedStateSaver stateSaver1(20, outState, "generation");
    eoTimedStateSaver   stateSaver2(1, outState, "time");

    checkpoint.add(stateSaver1);
    checkpoint.add(stateSaver2);



    // the algorithm
    // stopping criterion, eval, selection, transformation, replacement
    eoEasyEA<Indi> gga(checkpoint, eval, select, transform, replace);
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
