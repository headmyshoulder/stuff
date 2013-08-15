/*
 * ExprCppunit.cpp
 * Date: 2013-08-15
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include "Expr.h"

#include <cppunit/config/SourcePrefix.h>
#include <cppunit/extensions/HelperMacros.h>


class TrueTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TrueTest );
    CPPUNIT_TEST( eval );
    CPPUNIT_TEST( arity );
    CPPUNIT_TEST_SUITE_END();

public:
    
    void eval( void )
    {
        True expr;
        CPPUNIT_ASSERT( expr.eval() );
    }
    
    void arity( void )
    {
        True expr;
        CPPUNIT_ASSERT_EQUAL( size_t( 0 ) , expr.arity() );
    }
};

class TrueTest2 : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TrueTest2 );
    CPPUNIT_TEST( eval );
    CPPUNIT_TEST( arity );
    CPPUNIT_TEST_SUITE_END();
    
    BooleanExprPtr m_expr;

public:

    void setUp()
    {
        m_expr.reset( new True() );
    }
    void tearDown()
    {
        m_expr.release();
    }

protected:
    
    void eval( void )
    {
        CPPUNIT_ASSERT( m_expr->eval() );
    }
    
    void arity( void )
    {
        CPPUNIT_ASSERT_EQUAL( size_t( 0 ) , m_expr->arity() );
    }
};


CPPUNIT_TEST_SUITE_REGISTRATION( TrueTest );
CPPUNIT_TEST_SUITE_REGISTRATION( TrueTest2 );