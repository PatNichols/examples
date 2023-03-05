

FC := gfortran

all:
	${FC} -c point2d_mod.f90
	${FC} -c point3d_mod.f90
	${FC} -c test_pts.f90
	${FC} -o test_pts test_pts.o point3d_mod.o point2d_mod.o