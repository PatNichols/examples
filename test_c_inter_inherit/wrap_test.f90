program f_wrap_t
   use,intrinsic :: iso_fortran_env
   use :: f_wrap_mod

   type(foo_wrap_t) :: wrap
   character(len=256) :: sv
   real(real64) :: dv
   
   wrap = foo_wrap_t_ctor()
   dv = wrap%dvalue() 
   call wrap%svalue(sv)
   write(output_unit,*)'double value = ',dv
   write(output_unit,*)'string value = ',sv
end  program    	