#! /usr/bin/python

import os
from string import Template

default_class_template = """class $CLASSNAME
{
public:

    $CLASSNAME( void );
    ~$CLASSNAME( void );
};

"""


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



def add_namespace_replacements( replacements , args , top_namespace ):
    replacements[ "NAMESPACE_OPENING" ] = "namespace " + top_namespace + " {\n"
    replacements[ "NAMESPACE_CLOSING" ] = "} // namespace " + top_namespace + "\n"

    if hasattr( args , "namespace" ) and ( args.namespace is not None ) and ( len( args.namespace ) != 0 ) :
        s = "Found namespace(s) : "
        for namespace in args.namespace : s += namespace + " "
        print s

        for namespace in args.namespace:
            replacements[ "NAMESPACE_OPENING" ] += "namespace " + namespace + " {\n"
            replacements[ "NAMESPACE_CLOSING" ] = "} // namespace " + namespace + "\n" + replacements[ "NAMESPACE_CLOSING" ]



def add_class_replacements( replacements , args , class_template ):
    replacements[ "CLASS_TEMPLATE" ] = ""

    if hasattr( args , "classes" ) and ( args.classes is not None ) and ( len( args.classes ) != 0 ) :
        s = "Found class(es) : "
        for c in args.classes : s += c + " "
        print s

        for c in args.classes:
            class_source = Template( class_template )
            replacements[ "CLASS_TEMPLATE" ] += class_source.substitute( { "CLASSNAME" : c } )



def default_processing( filename , replacements , template ):
    print "* Creating " + filename + " ..."
    add_filename_replacements( replacements , filename )
    source = Template( template )
    f = open( filename , "w" );
    f.write( source.substitute( replacements ) )
