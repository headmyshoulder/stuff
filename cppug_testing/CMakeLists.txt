# Date: 2013-08-12
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
    set ( CMAKE_CXX_FLAGS "-std=c++11" )        ## Optimize
endif ()

include_directories ( ${Boost_INCLUDE_DIRS} )
include_directories ( gtest gtest/include )
include_directories ( gmock gmock/include )

enable_testing ( true )

add_executable ( main main.cpp )

add_executable ( ExprGTest ExprGTest.cpp gtest/src/gtest-all.cc gtest/src/gtest_main.cc gmock/src/gmock-all.cc )
target_link_libraries ( ExprGTest pthread )

add_test( NAME ExprGTest COMMAND ExprGTest )  



add_executable ( ExprCppunit CppunitMain ExprCppunit.cpp )
target_link_libraries ( ExprCppunit cppunit )

add_test( NAME ExprCppunit COMMAND ExprCppunit )
