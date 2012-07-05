#! /usr/bin/python

import os

from string import Template
from yapsy.IPlugin import IPlugin

import code_template_helpers


filename_help = "Output file name(s)"
namespace_help = "Namespace definitions to be created."
class_help = "Class templates to be created" 
template = """/*
 * $FILENAME
 * Date: $DATE
 * Author: $AUTHOR ($AUTHOREMAIL)
 */


$NAMESPACE_OPENING

$CLASS_TEMPLATE

$NAMESPACE_CLOSING
"""

class_template = """class $CLASSNAME
{
 public:

    $CLASSNAME( void );
    ~$CLASSNAME( void );
};

"""


class supertoll_header( IPlugin ):

    def __init__( self ):
        self.name = "supertollheader"
        self.help = "Creates a header file with header guards and namespace defintions for SuperToll."


    def register_in_arg_parser( self , subparsers ):
        parser = code_template_helpers.create_subparser( self , subparsers )
        parser.add_argument( "-f" , "--filename" ,  nargs = "+" , help = filename_help , required=True )
        parser.add_argument( "-n" , "--namespace" , nargs = "*" , help = namespace_help )
        parser.add_argument( "-c" , "--class" , nargs = "*" , help = class_help , dest = "classes" )


    def do_work( self , args , replacements ):

        print "Creating " + self.name + " template(s) ..."

        replacements[ "NAMESPACE_OPENING" ] = "namespace SuperToll {\n"
        replacements[ "NAMESPACE_CLOSING" ] = "} // namespace SuperToll\n"
        replacements[ "CLASS_TEMPLATE" ] = ""

        if hasattr( args , "namespace" ) and ( len( args.namespace ) != 0 ) :
            s = "Found namespace(s) : "
            for namespace in args.namespace : s += namespace + " "
            print s

            for namespace in args.namespace:
                replacements[ "NAMESPACE_OPENING" ] += "namespace " + namespace + " {\n"
                replacements[ "NAMESPACE_CLOSING" ] = "} // namespace " + namespace + "\n" + replacements[ "NAMESPACE_CLOSING" ]

        if hasattr( args , "classes" ) and ( len( args.classes ) != 0 ) :
            s = "Found class(es) : "
            for c in args.classes : s += c + " "
            print s

            for c in args.classes:
                class_source = Template( class_template )
                replacements[ "CLASS_TEMPLATE" ] += class_source.substitute( { "CLASSNAME" : c } )
            
        if hasattr( args , "filename" ) :
            for filename in args.filename:
                self.process( filename , replacements )


    def process( self , filename , replacements ):
        print "* Creating " + filename + " ..."
        code_template_helpers.add_filename_replacements( replacements , filename )
        source = Template( template )
        f = open( filename , "w" );
        f.write( source.substitute( replacements ) )

    


    
    
