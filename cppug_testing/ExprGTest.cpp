/*
 * ExprGTest.cpp
 * Date: 2013-08-14
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include "Expr.h"

#include <gtest/gtest.h>

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






// fixtures




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
        
    And andExpr( std::move( exprMock1 ) , std::move( exprMock2 ) );
    EXPECT_TRUE( andExpr.eval() );
    EXPECT_FALSE( andExpr.eval() );
    EXPECT_FALSE( andExpr.eval() );
    EXPECT_FALSE( andExpr.eval() );
}



