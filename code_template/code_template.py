#! /usr/bin/python

import argparse
import datetime
import os
import glob
import json

from pprint import pprint
from string import Template

import templates.simplecpp

directories = [ os.path.join( os.path.dirname( os.path.realpath(__file__) ) , "templates" ) ]

config = {
    "Author" : "Karsten Ahnert" ,
    "AuthorEmail" : "karsten.ahnert@gmx.de"
}

default_replacements = {
    "FILENAME" : "abc" ,
    "FILENAME_CAP" : "ABC" ,
    "FILEENDING_CAP" : "HPP" ,
    "DIRECTORY" : "" ,
    "AUTHOR" : config[ "Author" ] ,
    "AUTHOREMAIL" : config[ "AuthorEmail" ] ,
    "DATE" : datetime.date.today().isoformat()
}



def get_template_files( directories ) :
    files = []
    for dir in directories :
        fns = glob.glob( os.path.join( dir , "*.json" ) )
        for f in fns :
            files.append( f )
    return files

def create_arg_parser():
    parser = argparse.ArgumentParser(
        description = "Create code template for commonly used source code files."
        )
    subparsers = parser.add_subparsers( help="subcommand help" )
    return ( parser , subparsers )


def add_template_to_argparser( data , subparser ):
    parser = subparsers.add_parser(
        data[ "name" ] ,
        help = data[ "help" ] )
    for p in data[ "parameters" ]:
        parser.add_argument( p[ "filename" ] ,  nargs = 1 , help = p[ "help" ] )
    parser.set_defaults( which = data[ "name" ])

def get_replacements( template , args ):
    rep = default_replacements
    if hasattr( args , "filename" ) :
        fn = os.path.basename( args.filename[0] )
        rep[ "FILENAME_CAP" ] = ( os.path.splitext( fn )[0] ).upper()
        ext = ( os.path.splitext( fn )[1] ).upper()
        rep[ "FILEENDING_CAP" ] = ext[ 1: ]
        rep[ "FILENAME" ] = fn
    return rep



def create_template( template , args ):
    reps = get_replacements( template , args )
    cont = open( os.path.join( template[ "dirname" ] , template[ "code" ] ) , "r" ).read()
    abc = Template( cont )
    if hasattr( args , "filename" ):
        f = open( args.filename[0] , "w"  );
        f.write( abc.substitute( reps ) )






files = get_template_files( directories )

( parser , subparsers ) = create_arg_parser()

templates = {}

for f in files:
    data = json.load( open( f , "r" ) )
    data[ "dirname" ] = os.path.dirname( f )
    add_template_to_argparser( data , subparsers )
    templates[ data[ "name" ] ] = data



args = parser.parse_args()

template = templates[ args.which ]

create_template( template , args )
