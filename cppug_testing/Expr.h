/*
 * Expr.h
 * Date: 2013-08-14
 * Author: Karsten Ahnert (karsten.ahnert@gmx.de)
 */

#ifndef EXPR_H_INCLUDED
#define EXPR_H_INCLUDED


#include <memory>
#include <cmath>



/*
 * Main Interface
 * Classic polymorphism
 */
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











/*
 * Abstract expressions
 */
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
    IExpr< Result >& child( void ) { return *m_child; }
    IExpr< Result > const& child( void ) const { return *m_child; }
    
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
    IExpr< Result >& left( void ) { return *m_left; }
    IExpr< Result > const& left( void ) const { return *m_left; }
    IExpr< Result >& right( void ) { return *m_right; }
    IExpr< Result > const& right( void ) const { return *m_right; }
    
protected:
    
    UniqueExprPtr< Result > m_left , m_right;
};










/*
 * Some shortcuts
 */


// Boolean expressions
using BooleanExpr = IExpr< bool >;
using BooleanExprPtr = UniqueExprPtr< bool >;
using TerminalBooleanExpr = TerminalExpr< bool >;
using UnaryBooleanExpr = UnaryExpr< bool >;
using BinaryBooleanExpr = BinaryExpr< bool >;

// Floating point expressions
using FloatingPointExpr = IExpr< double >;
using TerminalFloatingPointExpr = TerminalExpr< double >;
using UnaryFloatingPointExpr = UnaryExpr< double >;
using BinaryFloatingPointExpr = BinaryExpr< double >;
using FloatingPointExprPtr = UniqueExprPtr< double >;




























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

class Not : public UnaryBooleanExpr
{
public:
    Not( BooleanExprPtr child )
    : UnaryBooleanExpr( std::move( child ) ) { }
    
    bool eval( void ) override { return ! m_child->eval(); }
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













class Plus : public BinaryFloatingPointExpr
{
public:
    Plus( FloatingPointExprPtr left , FloatingPointExprPtr right )
    : BinaryFloatingPointExpr( std::move( left ) , std::move( right ) ) { }
    
    double eval( void ) override {return m_left->eval() + m_right->eval(); }
};

class Minus : public BinaryFloatingPointExpr
{
public:
    Minus( FloatingPointExprPtr left , FloatingPointExprPtr right )
    : BinaryFloatingPointExpr( std::move( left ) , std::move( right ) ) { }
    
    double eval( void ) override {return m_left->eval() - m_right->eval(); }
};

class Multiply : public BinaryFloatingPointExpr
{
public:
    Multiply( FloatingPointExprPtr left , FloatingPointExprPtr right )
    : BinaryFloatingPointExpr( std::move( left ) , std::move( right ) ) { }
    
    double eval( void ) override {return m_left->eval() * m_right->eval(); }
};

class Divides : public BinaryFloatingPointExpr
{
public:
    Divides( FloatingPointExprPtr left , FloatingPointExprPtr right )
    : BinaryFloatingPointExpr( std::move( left ) , std::move( right ) ) { }
    
    double eval( void ) override {return m_left->eval() / m_right->eval(); }
};

class Value : public TerminalFloatingPointExpr
{
public:
    Value( double val ) : m_val( val ) { }
    double eval( void ) override { return m_val; }
private:
    double m_val;
};

class Sin : public UnaryFloatingPointExpr
{
public:
    Sin( FloatingPointExprPtr child ) : UnaryFloatingPointExpr( std::move( child ) ) {}
    double eval( void ) override { return sin( m_child->eval() ); }
};

class Cos : public UnaryFloatingPointExpr
{
public:
    Cos( FloatingPointExprPtr child ) : UnaryFloatingPointExpr( std::move( child ) ) {}
    double eval( void ) override { return sin( m_child->eval() ); }
};


#endif // EXPR_H_INCLUDED
