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

        m_expr.reset( new True() );



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
    std::unique_ptr< BooleanExprMock > exprMock1( new BooleanExprMock() );
    std::unique_ptr< BooleanExprMock > exprMock2( new BooleanExprMock() );

    
    EXPECT_CALL( *exprMock1 , eval() )
        .Times(4)
        .WillOnce(Return(true))
        .WillOnce(Return(true))
        .WillOnce(Return(false))
        .WillOnce(Return(false));
    EXPECT_CALL( *exprMock2 , eval() )
        .Times(2)
        .WillOnce(Return(true))
        .WillOnce(Return(false));
        
    And expr( std::move( exprMock1 ) , std::move( exprMock2 ) );
    EXPECT_TRUE( expr.eval() );
    EXPECT_FALSE( expr.eval() );
    EXPECT_FALSE( expr.eval() );
    EXPECT_FALSE( expr.eval() );
}

TEST( AndTest , arity )
{
    And expr( BooleanExprPtr( new BooleanExprMock() ) , BooleanExprPtr( new BooleanExprMock() ) );
    EXPECT_EQ( expr.arity() , 2 );
}

TEST( OrTest , eval )
{
    std::unique_ptr< BooleanExprMock > exprMock1( new BooleanExprMock() );
    std::unique_ptr< BooleanExprMock > exprMock2( new BooleanExprMock() );

    
    EXPECT_CALL( *exprMock1 , eval() )
        .Times(4)
        .WillOnce(Return(true))
        .WillOnce(Return(true))
        .WillOnce(Return(false))
        .WillOnce(Return(false));
    EXPECT_CALL( *exprMock2 , eval() )
        .Times(2)
        .WillOnce(Return(true))
        .WillOnce(Return(false));
        
    Or expr( std::move( exprMock1 ) , std::move( exprMock2 ) );
    EXPECT_TRUE( expr.eval() );
    EXPECT_TRUE( expr.eval() );
    EXPECT_TRUE( expr.eval() );
    EXPECT_FALSE( expr.eval() );
}

TEST( OrTest , arity )
{
    Or expr( BooleanExprPtr( new BooleanExprMock() ) , BooleanExprPtr( new BooleanExprMock() ) );
    EXPECT_EQ( expr.arity() , 2 );
}







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
