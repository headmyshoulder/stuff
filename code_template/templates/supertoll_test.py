#! /usr/bin/python

import os

from string import Template
from yapsy.IPlugin import IPlugin

from code_template_helpers import *


filename_help = "Output file name(s)"
testname_help = "Name of the test" 
template = """/*
 * $FILENAME
 * Date: $DATE
 * Author: $AUTHOR ($AUTHOREMAIL)
 */

#include <cppunit/config/SourcePrefix.h>
#include <cppunit/extensions/HelperMacros.h>

#define TESTNAME $TESTNAME


class TESTNAME : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( TESTNAME );

    CPPUNIT_TEST( test1 );

    CPPUNIT_TEST_SUITE_END();

public:

    void setUp( void )
    {
    }

    void tearDown( void )
    {
    }


protected:

    void test1( void )
    {
        CPPUNIT_ASSERT( true );
    }

};

CPPUNIT_TEST_SUITE_REGISTRATION( TESTNAME );
"""



class supertoll_header( IPlugin ):

    def __init__( self ):
        self.name = "supertolltest"
        self.help = "Creates a SuperToll test file."


    def register_in_arg_parser( self , subparsers ):
        parser = create_subparser( self , subparsers )
        parser.add_argument( "-f" , "--filename" ,  nargs = "+" , help = filename_help , required=True )
        parser.add_argument( "-t" , "--testname" , nargs = "?" , help = testname_help , dest = "testname" , default="SuperTollTest" )


    def do_work( self , args , replacements ):
        print "Creating " + self.name + " template(s) ..."

        replacements[ "TESTNAME" ] = ""

        if hasattr( args , "testname" ) and ( args.testname is not None ) :
            s = "Found test : " + args.testname
            print s
            replacements[ "TESTNAME" ] = args.testname


        if hasattr( args , "filename" ) :
            for filename in args.filename:
                default_processing( filename , replacements , template )
