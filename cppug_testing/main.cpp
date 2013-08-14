/*
 * main.cpp
 * Date: 2013-08-12
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include "Expr.h"

#include <iostream>





int main( int argc , char *argv[] )
{
    using namespace std;
    
    And expr( BooleanExprPtr( new True() ) , BooleanExprPtr( new False() ) );
    cout << "True && False = " << expr.eval() << endl;
    
    return 0;
}

