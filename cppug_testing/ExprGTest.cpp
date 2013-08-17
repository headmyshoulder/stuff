/*
 * ExprGTest.cpp
 * Date: 2013-08-14
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include "Expr.h"

#include <gtest/gtest.h>

#include <stdexcept>



/*
 * SIMPLE TESTS
 */

TEST( TrueTest , eval )
{
    True expr;
    EXPECT_TRUE( expr.eval() );
    ASSERT_TRUE( expr.eval() );
}

TEST( TrueTest , arity )
{
    True expr;
    EXPECT_EQ( expr.arity() , 0 );
}


TEST( FalseTest , eval )
{
    False expr;
    EXPECT_FALSE( expr.eval() );
}

TEST( FalseTest , arity )
{
    False expr;
    EXPECT_EQ( expr.arity() , 0 );
}

/*

EXPECT_TRUE             ASSERT_TRUE
EXPECT_FALSE            ASSERT_FALSE
EXPECT_EQ               ASSERT_EQ               a == b
EXPECT_NE               ASSERT_NE               a != b
EXPECT_LT               ASSERT_LT               a < b
EXPECT_LE               ASSERT_LT               a <= b
EXPECT_GT               ASSERT_GT               a > b
EXPECT_GE               ASSERT_GE               a >= b

EXPECT_STREQ            ASSERT_STREQ            strcmp( a , b )
EXPECT_STRNE            ASSERT_STRNE            ! strcmp( a , b )
EXPECT_STRCASEEQ        ASSERT_STRCASEEQ        strcmp( ignorecase( a ) , ignorecase( b ) )
EXPECT_STRCASENE        ASSERT_STRCASENE        ! strcmp( ignorecase( a ) , ignorecase( b ) )

*/






















/*
 * TESTING WITH FIXTURES
 */
class TrueTest2 : public ::testing::Test {
 protected:

    void SetUp( void )
    {
        m_expr.reset( new True() );
    }
    
    void TearDown( void )
    {
        m_expr.release();
    }

    BooleanExprPtr m_expr;
};

TEST_F( TrueTest2 , eval )
{
    EXPECT_TRUE( m_expr->eval() );
}

TEST_F( TrueTest2 , arity )
{
    EXPECT_EQ( m_expr->arity() , 0 );
}











/*
 * TESTING FLOATING POINT VALUES
 */

TEST( SinTest , value )
{
    EXPECT_DOUBLE_EQ( sin( 0.0 ) , 0.0 );
    // with pricision
}

TEST( CosTest , value )
{
    EXPECT_DOUBLE_EQ( cos( 0.0 ) , 1.0 );
}

/*

EXPECT_DOUBLE_EQ        ASSERT_DOUBLE_EQ
EXPECT_FLOAT_EQ         ASSERT_FLOAT_EQ
EXPECT_NEAR             ASSERT_NEAR             val1 , val2 , tolerenace

*/














/*
 * TESTING EXCEPTIONS
 */
double arcSin( double x )
{
    if( std::abs( x ) > 1.0 )
        throw std::runtime_error( "arcSin : x must be in the range -1:1" );
    return asin( x );
}


TEST( ArcSinTest , goodValueEval )
{
    EXPECT_DOUBLE_EQ( arcSin( 0.0 ) , 0.0 );
//     EXPECT_DOUBLE_EQ( arcSin( 1.0 ) , 1.5 , 0.2 );
//     EXPECT_DOUBLE_EQ( arcSin( -1.0 ) , -1.5 , 0.2 );
}

TEST( ArcSinTest , badValueEval )
{
    EXPECT_THROW( { arcSin( 2.0 ); } , std::runtime_error );
}


/*

EXPECT_THROW           ASSERT_THROW
EXPECT_ANY_THROW       ASSERT_ANY_THROW
EXPECT_NO_THROW        ASSERT_NO_THROW

*/






















/*
 * PARAMETRIZED TESTS
 */

class ArcSinValidRange : public ::testing::TestWithParam< double >
{
};

class ArcSinInvalidRange : public ::testing::TestWithParam< double >
{
};

TEST_P( ArcSinValidRange , testValidRange )
{
    EXPECT_NO_THROW( { arcSin( GetParam() ) ; } );
}

TEST_P( ArcSinInvalidRange , testInvalidRange )
{
    EXPECT_ANY_THROW( { arcSin( GetParam() ); } );
}

INSTANTIATE_TEST_CASE_P( test , ArcSinValidRange , ::testing::Range( -0.99 , 0.99 , 0.01 ) );
INSTANTIATE_TEST_CASE_P( test1 , ArcSinInvalidRange , ::testing::Range( -10.0 , -1.01 , 0.01 ) );
INSTANTIATE_TEST_CASE_P( test2 , ArcSinInvalidRange , ::testing::Range( 1.01 , 10.0 , 0.01 ) );

/*

INSTANTIATE_TEST_CASE_P( instantiateName , testName , generator );

generators:
Range( begin , end , step )
Values( x1 , x2 , x3 , x4 )
Bool() -> generates true and false
Combine( g1 , g2 ) -> 















/*
 * TESTING - What else
 */

/*

* Predicate testing EXPECT_PRED1( func , val ) - EXPECT_TRUE( func( val ) ) with neat output
* Static assertions - maybe not that useful
* Testwise setUp, tearDown methods
* Global setUp, tearDown methods
* Typed tests -> testing generic code
* command line options for
  * different output types
  * running only some test
  * repeating and shuffling tests ...


*/























/*
 * TESTING WITH MOCKS
 */



#include <gmock/gmock.h>

using namespace ::testing;

class BooleanExprMock : public BooleanExpr
{
public:
    MOCK_CONST_METHOD0( arity , size_t() );
    MOCK_METHOD0( eval , bool() );
};


TEST( AndTest , eval )
{
    std::unique_ptr< BooleanExprMock > left( new BooleanExprMock() );
    std::unique_ptr< BooleanExprMock > right( new BooleanExprMock() );

    
    EXPECT_CALL( *left , eval() )
        .Times(4)
        .WillOnce(Return(true))
        .WillOnce(Return(true))
        .WillOnce(Return(false))
        .WillOnce(Return(false));
    EXPECT_CALL( *right , eval() )
        .Times(2)
        .WillOnce(Return(true))
        .WillOnce(Return(false));
        
    And expr( std::move( left ) , std::move( right ) );
    EXPECT_TRUE( expr.eval() );
    EXPECT_FALSE( expr.eval() );
    EXPECT_FALSE( expr.eval() );
    EXPECT_FALSE( expr.eval() );
}

TEST( AndTest , arity )
{
    std::unique_ptr< BooleanExprMock > left( new BooleanExprMock() );
    std::unique_ptr< BooleanExprMock > right( new BooleanExprMock() );
    
    EXPECT_CALL( *left , eval() ).Times( 0 );
    EXPECT_CALL( *left , arity() ).Times( 0 );
    EXPECT_CALL( *right , eval() ).Times( 0 );
    EXPECT_CALL( *right , arity() ).Times( 0 );

    And expr( std::move( left ) , std::move( right ) );
    EXPECT_EQ( expr.arity() , 2 );
}

TEST( OrTest , eval )
{
    std::unique_ptr< BooleanExprMock > left( new BooleanExprMock() );
    std::unique_ptr< BooleanExprMock > right( new BooleanExprMock() );

    
    EXPECT_CALL( *left , eval() )
        .Times(4)
        .WillOnce(Return(true))
        .WillOnce(Return(true))
        .WillOnce(Return(false))
        .WillOnce(Return(false));
    EXPECT_CALL( *right , eval() )
        .Times(2)
        .WillOnce(Return(true))
        .WillOnce(Return(false));
        
    Or expr( std::move( left ) , std::move( right ) );
    EXPECT_TRUE( expr.eval() );
    EXPECT_TRUE( expr.eval() );
    EXPECT_TRUE( expr.eval() );
    EXPECT_FALSE( expr.eval() );
}

TEST( OrTest , arity )
{
    std::unique_ptr< BooleanExprMock > left( new BooleanExprMock() );
    std::unique_ptr< BooleanExprMock > right( new BooleanExprMock() );
    
    EXPECT_CALL( *left , eval() ).Times( 0 );
    EXPECT_CALL( *left , arity() ).Times( 0 );
    EXPECT_CALL( *right , eval() ).Times( 0 );
    EXPECT_CALL( *right , arity() ).Times( 0 );

    Or expr( std::move( left ) , std::move( right ) );
    EXPECT_EQ( expr.arity() , 2 );
}



/*

General syntax:

EXPECT_CALL(mock_object, method(matchers))
    .With(multi_argument_matcher)
    .Times(cardinality)
    .InSequence(sequences)
    .After(expectations)
    .WillOnce(action)
    .WillRepeatedly(action)
    .RetiresOnSaturation();
    
Matchers:

*/



class Foo
{
public:
    
    MOCK_CONST_METHOD1( doSomething , int( int ) );
};

int doSomethingWithFoo( Foo& foo , int x )
{
    return foo.doSomething( x * 2 ) + foo.doSomething( x * 2 ) + 2;
}

TEST( FooTest , doSomething )
{
    Foo foo;
    EXPECT_CALL( foo , doSomething( 12 ) )
        .Times( 2 )
        .WillOnce( Return( 3 ) )
        .WillOnce( Return( 4 ) );
    
    EXPECT_EQ( doSomethingWithFoo( foo , 6 ) , 9 );
}
