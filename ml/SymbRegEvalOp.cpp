/*
 *  Symbolic regression (symbreg):
 *  A simple GP example with Open BEAGLE
 *
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   SymbRegEvalOp.cpp
 *  \brief  Implementation of the class SymbRegEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14 $
 *  $Date: 2007/08/10 20:32:28 $
 */


#include "SymbRegEvalOp.hpp"
#include "beagle/GP.hpp"

#include <cmath>
#include <utility>

#include <boost/circular_buffer.hpp>
#include <boost/numeric/odeint/stepper/runge_kutta4.hpp>
#include <boost/numeric/odeint/integrate/integrate_const.hpp>
#include <boost/array.hpp>

using namespace Beagle;

/*!
 *  \brief Construct a new symbolic regression evaluation operator.
 */
SymbRegEvalOp::SymbRegEvalOp() :
    GP::EvaluationOp("SymbRegEvalOp")
{ }


/*!
 *  \brief Evaluate the individual fitness for the symbolic regression problem.
 *  \param inIndividual Individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness measure,
 */
Fitness::Handle SymbRegEvalOp::evaluate(GP::Individual& inIndividual, GP::Context& ioContext)
{
    double lSquareError = 0.;
    for( unsigned int i=0 ; i<mDataSet->size() ; i++ )
    {
        Beagle_AssertM( (*mDataSet)[i].second.size() == 1 );
        const Double lX( (*mDataSet)[i].second[0] );
        const Double lY( (*mDataSet)[i].second[1] );
        const Double lZ( (*mDataSet)[i].second[2] );
        setValue( "X" , lX , ioContext );
        setValue( "Y" , lY , ioContext );
        setValue( "Z" , lZ , ioContext );
        const Double lR( (*mDataSet)[i].first );
        Double lResult;
        inIndividual.run( lResult , ioContext );
        const double lError = lR - lResult;
        lSquareError += ( lError * lError );
    }
    const double lMSE  = lSquareError / mDataSet->size();
    const double lRMSE = sqrt( lMSE );
    const double lFitness = 1. / (1. + lRMSE);
    return new FitnessSimple( lFitness );
}


/*!
 * \brief Initialize the operator by sampling the function to regress.
 * \param ioSystem System to use to sample.
 */
void SymbRegEvalOp::init(System& ioSystem)
{
    using namespace boost::numeric::odeint;

    GP::EvaluationOp::init( ioSystem );

    Beagle::Logger& logger = ioSystem.getLogger();
    logger.log( "Called SymbRegEvalOp::init()" , Beagle::Logger::eBasic );

    // Get reference to data set component
    mDataSet = castHandleT< Beagle::DataSetRegression >( ioSystem.getComponent( "DataSetRegression" ) );
    if( mDataSet == NULL )
    {
        throw Beagle_RunTimeExceptionM( "'DataSetRegression' component not found in System, impossible to proceed further!" );
    }


    typedef boost::array< double , 3 > state_type;
    runge_kutta4< state_type > rk4;
    auto lorenz = []( const state_type &x , state_type &dxdt , double t ) {
        const double sigma = 10.0;
        const double R = 28.0;
        const double b = 8.0 / 3.0;
        dxdt[0] = sigma * ( x[1] - x[0] );
        dxdt[1] = R * x[0] - x[1] - x[0] * x[2];
        dxdt[2] = -b * x[2] + x[0] * x[1];
    };

    const double dt = 0.01;
    const size_t emb_dim = 3;
    const size_t emb_tau = 9;
    const size_t N = 10000;

    state_type x = { 10.0 , 10.0 , 10.0 };

    // transients
    integrate_const( rk4 , lorenz , x , 0.0 , 100.0 , dt );
    
    state_type dxdt;
    lorenz( x , dxdt , 0.0 );

    // initially full buffer
    boost::circular_buffer< std::pair< double , double > > buf( ( emb_dim - 1 ) * emb_tau +  1 );
    for( size_t i=0 ; i<buf.size() + 1 ; ++i )
    {
        rk4.do_step( lorenz , x , dxdt , 0.0 , dt );
        lorenz( x , dxdt , 0.0 );
        buf.push_back( std::make_pair( x[0] , dxdt[0] ) );
    }


    
    // If data set is empty, initialize it with random samples of quartic polynomial
    Beagle::DataSetRegression &ds = *mDataSet;
    if( ds.size() == 0 )
    {
        ds.resize( N );
        for( unsigned int i=0 ; i<ds.size() ; i++ )
        {
            ds[i].first = buf.back().first;
            ds[i].second.resize( 3 );
            for( size_t j=0 ; j<emb_dim ; ++j ) 
                ds[i].second[j] = buf[j*emb_dim].second;

            rk4.do_step( lorenz , x , dxdt , 0.0 , dt );
            lorenz( x , dxdt , 0.0 );
            buf.push_back( std::make_pair( x[0] , dxdt[0] ) );
        }
    }
}
