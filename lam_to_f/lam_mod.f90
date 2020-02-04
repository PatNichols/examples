module lam_mod
  use,intrinsic :: iso_c_binding
  use,intrinsic :: iso_fortran_env
  
  procedure(ffun),pointer :: ptr

  abstract interface
    function ffun(i,r) result(x)
      use iso_c_binding
      implicit none
      real(c_double),value,intent(in) :: r
      integer(c_int32_t),value,intent(in) :: i
      real(c_double) :: x
    end function
  end interface
  interface
    function c_get_flam() result(c_fptr) bind(c,name="c_get_flam")
    use iso_c_binding
    type(c_funptr) :: c_fptr
    end function
    function c_get_glam() result(c_fptr) bind(c,name="c_get_glam")
    use iso_c_binding
    type(c_funptr) :: c_fptr
    end function
    function c_get_xlam() result(c_fptr) bind(c,name="c_get_xlam")
    use iso_c_binding
    type(c_funptr) :: c_fptr
    end function
  end interface
contains
  function get_flam(i,r) result(x)
      use iso_c_binding
      implicit none
      real(c_double),intent(in) :: r
      integer(c_int32_t),intent(in) :: i
      real(c_double) :: x
      type(c_funptr) :: c_fptr
      procedure(ffun),pointer :: f_ptr
      print *,"fort inter i = ",i," r= ",r
      c_fptr = c_get_flam()
      call c_f_procpointer(c_fptr,f_ptr)
      x = f_ptr(i,r)
  end function      
  function get_glam(i,r) result(x)
      use iso_c_binding
      implicit none
      real(c_double),intent(in) :: r
      integer(c_int32_t),intent(in) :: i
      real(c_double) :: x
      type(c_funptr) :: c_fptr
      procedure(ffun),pointer :: f_ptr
      print *,"fort inter i = ",i," r= ",r
      c_fptr = c_get_glam()
      call c_f_procpointer(c_fptr,f_ptr)
      x = f_ptr(i,r)
  end function      
  function get_xlam(i,r) result(x)
      use iso_c_binding
      implicit none
      real(c_double),intent(in) :: r
      integer(c_int32_t),intent(in) :: i
      real(c_double) :: x
      type(c_funptr) :: c_fptr
      procedure(ffun),pointer :: f_ptr
      print *,"fort inter i = ",i," r= ",r
      c_fptr = c_get_xlam()
      call c_f_procpointer(c_fptr,f_ptr)
      x = f_ptr(i,r)
  end function      
end module
