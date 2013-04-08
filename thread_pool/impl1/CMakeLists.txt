# Date: 2013-04-04
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

set ( TINYTHREAD_CPP "/home/karsten/boost/testing/TinyThread++-1.1/source/tinythread.cpp" )

include_directories ( "/home/karsten/boost/testing/TinyThread++-1.1/source/" )

add_executable ( main_impl1 main.cpp ThreadPool.cpp ${TINYTHREAD_CPP} )
target_link_libraries ( main_impl1 pthread )