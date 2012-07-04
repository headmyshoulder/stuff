#! /usr/bin/python

from yapsy.IPlugin import IPlugin

def create_subparser( plugin , subparsers ):
    parser = subparsers.add_parser( plugin.name , help = plugin.help )
    parser.set_defaults( which = plugin.name )
    return parser

class simpleppp( IPlugin ):

    def __init__( self ):
        self.name = "simplecpp"
        self.help = "Creates a simple \"Hello world\" C++ file with one main function."
        self.filename_help = "Output file name(s)"

    def register_in_arg_parser( self , subparsers ):
        parser = create_subparser( self , subparsers )
        parser.add_argument( "filename" ,  nargs = "+" , help = self.filename_help )

    def do_work( self , args ):
        print "Creating " + self.name + " template(s) ..."
        if hasattr( args , "filename" ) :
            for filename in args.filename:
                self.process( filename )

    def process( self , filename ):
        print "* " + filename
        pass
    


    
    
