
all:
	${CXX} -g -c foo.cpp
	${CXX} -g -c c_foo.cpp
	${FC} -g -c f_wrap.f90
	${FC} -g -c wrap_test.f90
#	${FC} -g -o wrap_test wrap_test.o f_wrap.o c_foo.o foo.o
	${FC} -g -o wrap_test foo.o c_foo.o f_wrap.o wrap_test.o

clean:
	rm -fr *.o *.mode wrap_test	
