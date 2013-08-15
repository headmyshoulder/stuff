/*

TESTING

Karsten Ahnert
Ambrosys GmbH


Frameworks und Best Practises

Content

Introduction

Frameworks

Library Code




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

