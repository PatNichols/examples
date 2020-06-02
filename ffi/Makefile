CXX?=g++
FC?=gfortran

all:
	${CXX} -g -c test_ffi.cpp
	${FC} -g -c test_mod.f90 test.f90 
	${CXX} -g -o test test_ffi.o test.o test_mod.o -lgfortran
   
clean:
	rm -f test
	rm -f test_mod.mod
	rm -f *.o
