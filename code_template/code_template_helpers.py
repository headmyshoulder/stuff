#! /usr/bin/python

import os
from string import Template
from yapsy.IPlugin import IPlugin

default_class_template = """class $CLASSNAME
{
public:

    $CLASSNAME( void );
    ~$CLASSNAME( void );
};

"""


def create_subparser( plugin , subparsers ):
    parser = subparsers.add_parser( plugin.name , help = plugin.description )
    parser.set_defaults( which = plugin.name )
    return parser

def check_filename_ending( filename , default_ending ) :
    endings = ( "cpp" , "py" , "h" , "hpp" , "c" , "cxx" , "cc" , "hxx" , "ipp" )
    if( default_ending not in endings ):
        raise Exception( "Unknown default ending " + default_ending)
    ending = os.path.splitext( filename )[1]
    if( len( ending ) == 0 ):
        return os.path.splitext( filename )[0] + "." + default_ending
    if( len( ending ) ==  1 ):
        return os.path.splitext( filename )[0] + "." + default_ending
    if( ending != ( "." + default_ending ) ):
        print "Maybe the filename has the wrong line ending : " + filename + " , expected is " + default_ending
        return filename
    return filename



class APlugin( IPlugin ) :
    def __init__( self ):
        self.name = "APlugin"
        self.description = "Description"

    def set_name( self , name ):
        self.name = name
    
    def set_description( self , description ):
        self.description = description


    
    
    



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
