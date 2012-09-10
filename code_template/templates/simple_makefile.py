#! /usr/bin/python

import os

from string import Template

import code_template_helpers 




filename_help = "Output file name(s)"
target_help = "Cpp targets to be created."
template = """# Date: $DATE
# Author: $AUTHOR ($AUTHOREMAIL)

CC = g++
CXX = g++

INCLUDES = -I$$(BOOST_ROOT)
CXXFLAGS += $$(INCLUDES)

all : $TARGET

$TARGETS

clean :
    rm *.o *~ $TARGET
"""



class simplemakefile( code_template_helpers.APlugin ):

    def register_in_arg_parser( self , subparsers ):
        parser = code_template_helpers.create_subparser( self , subparsers )
        parser.add_argument( "-f" , "--filename" ,  nargs = "+" , help = filename_help , default=["Makefile"] )
        parser.add_argument( "-t" , "--target" , nargs = "*" , help = target_help )



    def do_work( self , args , replacements ):
        print "Creating " + self.name + " template(s) ..."

        replacements[ "TARGET" ] = ""
        replacements[ "TARGETS" ] = ""
        if ( hasattr( args , "target" ) ) and ( args.target is not None ) and ( len( args.target ) != 0 ) :
            for target in args.target:
                replacements[ "TARGET" ] += ( target + " " )
                replacements[ "TARGETS" ] += ( target + " : " + target + ".o" + "\n" )
            
        if hasattr( args , "filename" ) :
            for filename in args.filename:
                code_template_helpers.default_processing( filename , replacements , template )
