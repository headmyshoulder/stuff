/*
 * main.cpp
 * Date: 2013-08-12
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#include <memory>
#include <iostream>



template< typename Result >
class IExpr
{
public:
    
    virtual ~IExpr( void ) {}
    virtual size_t arity( void ) const = 0;
    virtual Result eval( void ) = 0;
};

template< typename Result >
using UniqueExprPtr = std::unique_ptr< IExpr< Result > >;








template< typename Result >
class TerminalExpr : public IExpr< Result >
{
public:
    size_t arity( void ) const override { return 0; }
};



template< typename Result >
class UnaryExpr : public IExpr< Result >
{
public:
    
    UnaryExpr( UniqueExprPtr< Result > child )
    : m_child( std::move( child ) ) { }
    
    size_t arity( void ) const override { return 1; }
    
protected:
    
    UniqueExprPtr< Result > m_child;
};




template< typename Result >
class BinaryExpr : public IExpr< Result >
{
public:
    
    BinaryExpr( UniqueExprPtr< Result > left , UniqueExprPtr< Result > right )
    : m_left( std::move( left ) ) , m_right( std::move( right ) ) { }
    
    size_t arity( void ) const override { return 2; }
    
protected:
    
    UniqueExprPtr< Result > m_left , m_right;
};






using BooleanExpr = IExpr< bool >;
using BooleanExprPtr = UniqueExprPtr< bool >;
using TerminalBooleanExpr = TerminalExpr< bool >;
using UnaryBooleanExpr = UnaryExpr< bool >;
using BinaryBooleanExpr = BinaryExpr< bool >;



class True : public TerminalBooleanExpr
{
public:
    
    bool eval( void ) override { return true; }
};

class False : public TerminalBooleanExpr
{
public:
    
    bool eval( void ) override { return false; }
};

class And : public BinaryBooleanExpr
{
public:
    And( BooleanExprPtr left , BooleanExprPtr right )
    : BinaryBooleanExpr( std::move( left ) , std::move( right ) ) { }
    
    bool eval( void ) override { return m_left->eval() && m_right->eval(); }
};

class Or : public BinaryBooleanExpr
{
public:
    Or( BooleanExprPtr left , BooleanExprPtr right )
    : BinaryBooleanExpr( std::move( left ) , std::move( right ) ) { }
    
    bool eval( void ) override { return m_left->eval() || m_right->eval(); }
};

class Not : public UnaryBooleanExpr
{
public:
    Not( BooleanExprPtr child ) : UnaryBooleanExpr( std::move( child ) ) { }
    
    bool eval( void ) override { return ! m_child->eval(); }
};






using IntegerExpr = IExpr< int >;
using IntegerExprPtr = IExprPtr< int >;

class Plus : public IntegerExpr {};
class Minus : public IntegerExpr {};
class Value : public IntegerExpr {}

// ...

int main( int argc , char *argv[] )
{
    using namespace std;
    
    And expr( BooleanExprPtr( new True() ) , BooleanExprPtr( new False() ) );
    cout << "True && False = " << expr.eval() << endl;
    
    return 0;
}

