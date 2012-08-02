TYPE_ERASURE_DIR = $(HOME)/boost/testing/type_erasure

GCC_DIR = $(HOME)/boost/testing/gcc-4.7.0/build
CC = $(GCC_DIR)/bin/g++
CXX = $(GCC_DIR)/bin/g++


# CC = g++-4.7
# CXX = g++-4.7


CXXFLAGS += -I$(BOOST_ROOT) -I$(TYPE_ERASURE_DIR) -std=c++11


all : rule_engine
rule_engine : rule_engine.o
rule_engine.o : rule_engine.cpp rule_engine.h

clean :
	rm *.o rule_engine *~