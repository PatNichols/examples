

all:
	g++ -c lam.c -std=c++17
	gfortran -c lam_mod.f90 tst_lam.f90
	g++ -o tst_lam lam.o tst_lam.o lam_mod.o -lgfortran

clean:   
	rm -fr *.o *mod
	rm -fr tst_lam


