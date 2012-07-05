#! /usr/bin/python

import os
import imp

from string import Template

from yapsy.IPlugin import IPlugin


def create_subparser( plugin , subparsers ):
    parser = subparsers.add_parser( plugin.name , help = plugin.help )
    parser.set_defaults( which = plugin.name )
    return parser


def add_filename_replacements( rep , filename ):
    fn = os.path.basename( filename )
    rep[ "FILENAMECAP" ] = ( os.path.splitext( fn )[0] ).upper()
    rep[ "FILEENDINGCAP" ] =  (( os.path.splitext( fn )[1] ).upper())[ 1: ]
    rep[ "FILENAME" ] = fn
    rep[ "DIRECTORY" ] = os.path.dirname( filename )


class supertoll_header( IPlugin ):

    def __init__( self ):
        self.name = "supertollheader"
        self.help = "Creates a header file with header guards and namespace defintions for SuperToll."
        self.filename_help = "Output file name(s)"
        self.namespace_help = "Namespace definitions to be created."
        self.template = """/*
 * $FILENAME
 * Date: $DATE
 * Author: $AUTHOR ($AUTHOREMAIL)
 */
"""

    def set_base_path( self , path ):
        mod = imp.find_module( "code_template_helpers" , [ path ] )

    def register_in_arg_parser( self , subparsers ):
        parser = create_subparser( self , subparsers )
        parser.add_argument( "-f" , "--filename" ,  nargs = "+" , help = self.filename_help , required=True )
        parser.add_argument( "-n" , "--namespace" , nargs = "*" , help = self.namespace_help )

    def do_work( self , args , default_replacements ):
        print "Creating " + self.name + " template(s) ..."
        if hasattr( args , "namespace" ) and ( len( args.namespace ) != 0 ) :
            s = "Found namespaces "
            for namespace in args.namespace: s += namespace + " "
            print s
        if hasattr( args , "filename" ) :
            for filename in args.filename:
                self.process( filename , default_replacements )

    def process( self , filename , default_replacements ):
        print "* Creating " + filename + " ..."
        replacements = default_replacements
        add_filename_replacements( replacements , filename )
        source = Template( self.template )
        # f = open( filename , "w" );
        # f.write( source.substitute( replacements ) )

    


    
    
