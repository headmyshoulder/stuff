# Date: 2013-02-18
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

project ( PROJECT )
cmake_minimum_required ( VERSION 2.6 )

# Add path for custom modules
set( CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
  )

find_package ( Boost REQUIRED COMPONENTS thread system )

if ( CMAKE_COMPILER_IS_GNUCXX )
    set ( CMAKE_CXX_FLAGS "-std=c++0x" )        ## Optimize
endif ()

include_directories ( ${Boost_INCLUDE_DIRS} )
include_directories ( "/home/karsten/boost/testing/ml/de/differentialevolution" )

add_executable( main main.cpp )

target_link_libraries ( main ${Boost_LIBRARIES} )