
FC?=gfortran

all:
	$(FC) -c gen_fun_mod.f90
	$(FC) -o gen_fun gen_fun.f90 gen_fun_mod.o
 
clean:
	rm -fr *.o *.mod
	rm -fr gen_fun
                
