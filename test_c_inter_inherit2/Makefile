
all:
	${CXX} -g -ggdb -c foo.cpp
	${F90} -g -ggdb -c foo_mod.f90
	${CXX} -g -ggdb -c foo_test_cxx.cpp
	${F90} -g -ggdb -c foo_test_f.f90     
	${F90} -g -ggdb -o foo_test_f foo_test_f.o foo_mod.o foo.o -lstdc++
	${CXX} -g -ggdb -o foo_test_cxx foo_test_cxx.o foo.o
	${CXX} -g -ggdb -c foo_test_c.cpp
	${CXX} -g -ggdb -o foo_test_c foo_test_c.o foo.o
        
clean:
	rm -f *.o *.mod foo_test_cxx foo_test_f foo_test_c   		
	