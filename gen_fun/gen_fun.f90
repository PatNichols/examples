program gen_fun
  use,intrinsic :: iso_c_binding
  use gen_fun_mod
  
  real(c_double) :: r
  integer(c_int32_t) :: i
  
  i = 32
  r = 2.0
  
  call my_fun(r)
  call my_fun(i)
  call my_fun(i,r)
end program  