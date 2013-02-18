# Date: 2013-02-16
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

project ( PROJECT )
cmake_minimum_required ( VERSION 2.6 )

# Add path for custom modules
set( CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
  )

find_package ( Boost REQUIRED )

if ( CMAKE_COMPILER_IS_GNUCXX )
    set ( CMAKE_CXX_FLAGS "-std=c++0x" )        ## Optimize
endif ()

set ( OpenBeagleROOT "/home/karsten/boost/testing/ml/OpenBEAGLE-4.0.0-alpha2-Source/install" )
set ( PaccROOT "/home/karsten/boost/testing/ml/pacc-1.4.1-Source/install" )
set ( ODEINT_ROOT $ENV{ODEINT_ROOT} )

find_library ( OpenBeagleLIBRARY NAMES openbeagle PATHS ${OpenBeagleROOT}/lib )
find_library ( OpenBeagleGALIBRARY NAMES openbeagle-GA PATHS ${OpenBeagleROOT}/lib )
find_library ( OpenBeagleGPLIBRARY NAMES openbeagle-GP PATHS ${OpenBeagleROOT}/lib )
find_library ( PaccLIBRARY NAMES pacc PATHS ${PaccROOT}/lib )

message ( STATUS "OpenBeagleLIBRARY is ${OpenBeagleLIBRARY}" )
message ( STATUS "OpenBeagleGALIBRARY is ${OpenBeagleGALIBRARY}" )
message ( STATUS "OpenBeagleGPLIBRARY is ${OpenBeagleGPLIBRARY}" )
message ( STATUS "PaccLIBRARY is ${PaccLIBRARY}" )


include_directories ( ${Boost_INCLUDE_DIRS} )
include_directories ( ${OpenBeagleROOT}/include )
include_directories ( ${PaccROOT}/include )
include_directories ( ${ODEINT_ROOT} )


add_executable( main main.cpp )
add_executable( SymReg SymbRegMain.cpp SymbRegEvalOp.cpp )

target_link_libraries ( SymReg ${OpenBeagleLIBRARY} ${OpenBeagleGPLIBRARY} ${PaccLIBRARY} )