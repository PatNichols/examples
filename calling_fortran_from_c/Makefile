
FC := gfortran
CXX := g++

all:
	${FC} -c -g -ggdb ffun_mod.f90
	${CXX} -c -g -ggdb ffun.cpp
	${CXX} -o ffun -ggdb -g ffun.o ffun_mod.o -I./ -L/usr/local/Cellar/gcc/12.2.0/lib/gcc/12 -lgfortran

clean:
	rm *.o *.mod ffun