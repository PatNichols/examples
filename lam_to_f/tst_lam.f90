program lam_prog
   use,intrinsic :: iso_c_binding
   use,intrinsic :: iso_fortran_env
   use lam_mod
   implicit none
   real(c_double) :: x,y
   integer(c_int32_t) :: i   

   i = 32
   x = 64.0
   y = get_flam(i,x)
   print *,"y = ",y
   y = get_glam(i,x)
   print *,"y = ",y
   y = get_xlam(i,x)
   print *,"y = ",y
end program    