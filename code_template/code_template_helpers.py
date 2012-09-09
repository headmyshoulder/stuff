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



def add_namespace_replacements( replacements , args , default_namespaces ):

    replacements[ "NAMESPACE_CLOSING" ] = ""
    replacements[ "NAMESPACE_OPENING" ] = ""
    for n in default_namespaces :
        replacements[ "NAMESPACE_OPENING" ] += "namespace " + n + " {\n"
        replacements[ "NAMESPACE_CLOSING" ] = "} // namespace " + n + "\n" + replacements[ "NAMESPACE_CLOSING" ]

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


def first_n_same( p1 , p2 ):
    n = min( len(p1) , len(p2 ) )
    for i in range(0,n):
        if( p1[i] != p2[i] ) : return False;
    return True

def full_split_impl( path , arr ):
    p = os.path.split( path )
    if( p[1] != "" ) :
        arr.append( p[1] )
        return full_split_impl( p[0] , arr )
    if( p[0] == "/" ):
        return arr
    if( p[0] != "" ):
        return full_split_impl( p[0] , arr )
    return arr

def full_split( path ):
    arr = full_split_impl( path , [] )
    arr.reverse()
    return arr

def full_join( path ):
    p = ""
    if len( path ) != 0 :
        p += path[0]
    for i in range(1,len(path)):
        p += "/" + path[i]
    return p



def find_boost_path( boost_path ):
    path = full_split( os.getcwd() )
    r = range( 0 , len( path ) )
    r.reverse()
    found = False
    for i in r:
        if path[i] == boost_path[0] :
            if first_n_same( path[i:] , boost_path ) :
                found = True
                break
    if found :
        return path[ i : ]
    else :
        return []

def find_odeint_lib_path():
    return find_boost_path( [ "libs" , "numeric" , "odeint" ] )
    
def find_odeint_header_path():
    return find_boost_path( [ "boost" , "numeric" , "odeint" ] )

def create_cap_boost_filename_str( f ):
    f = f.replace( "/" , "_" )
    f = f.replace( "." , "_" )
    f = f.upper()
    f = f + "_DEFINED"
    return f


if __name__ == "__main__" :

    p1 = [ "boost" , "numeric" , "odeint" ]
    p2 = [ "boost" , "numeric" , "odeint" , "stepper" ]
    p3 = [ "home" , "karsten" , "src" , "odeint-v2" , "boost" , "numeric" , "odeint" , "stepper" ]
    p4 = [ "boost" , "numeric" ]

    # print first_n_same( p1 , p2 )
    # print first_n_same( p2 , p1 )
    # print first_n_same( p1 , p3 )
    # print first_n_same( p3 , p1 )
    # print first_n_same( p1 , p4 )
    # print first_n_same( p4 , p1 )

    find_boost_path( p1 )

    print full_split( os.getcwd() )

    print create_cap_boost_filename_str( "boost/numeric/a.hpp" )
