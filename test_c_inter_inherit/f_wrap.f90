module f_wrap_mod
use,intrinsic :: iso_c_binding
use,intrinsic :: iso_fortran_env

    type :: foo_wrap_t
    	type(c_ptr) :: data_ptr    
    contains
    	procedure :: dvalue => foo_wrap_t_dvalue
    	procedure :: svalue => foo_wrap_t_svalue
    	final foo_wrap_t_dtor 
    end type

interface
    function foo_wrap_dval_f(p) result(res) bind(C,name="foo_wrap_dval")
      import c_ptr,c_double
      type(c_ptr),value :: p
      real(c_double) :: res
    end function 

    function foo_wrap_sval_f(p) result(res) bind(C,name="foo_wrap_sval")
      import c_ptr
      type(c_ptr),value :: p
      type(c_ptr) :: res
    end function
    
    subroutine foo_wrap_dtor_f(p) bind(C,name="foo_wrap_dtor")
      import c_ptr
      type(c_ptr),value :: p  
    end subroutine 
    
    function foo_wrap_ctor_f() result(res) bind(C,name="foo_wrap_ctor")
      import c_ptr
      type(c_ptr) :: res
    end function	

    function c_strlen(p) result(res) bind(C,name="strlen")
       import c_ptr,c_size_t
       integer(c_size_t) :: res
       type(c_ptr),value :: p 
    end function

    subroutine c_free(p) bind(C,name="free")
    	import c_ptr
    	type(c_ptr),value :: p 
    end subroutine

    function c_alloc(n) result(p) bind(C,name="malloc")
        import c_ptr,c_size_t
        integer(c_size_t) :: n
        type(c_ptr) :: p
    end function

end interface

contains

    function foo_wrap_t_ctor() result(this)
      implicit none
      type(foo_wrap_t) :: this
      this%data_ptr = foo_wrap_ctor_f()
    end function

    real(real64) function foo_wrap_t_dvalue(this) result(res)
      implicit none
      class(foo_wrap_t) :: this
      res = foo_wrap_dval_f(this%data_ptr)
    end function  

    subroutine foo_wrap_t_svalue(this,fstr)
      implicit none
      class(foo_wrap_t),intent(in) :: this
      character(len=*) :: fstr
      type(c_ptr) :: str_p
      character(c_char),dimension(:),pointer :: cstr
      integer(int64) :: wlen,clen,flen,i
       
      str_p = foo_wrap_sval_f(this%data_ptr)
      flen = len(fstr)
      clen = c_strlen(str_p)
      call c_f_pointer(str_p,cstr,[clen])
      wlen = min(flen,clen)
      do i=1,wlen
         fstr(i:i) = cstr(i)
      end do 
      if ( flen > clen ) then
         fstr(wlen+1:flen) = ' ' 
      end if  
    end subroutine

    subroutine foo_wrap_t_dtor(this)
      implicit none
      type(foo_wrap_t) :: this
      call foo_wrap_dtor_f(this%data_ptr) 
    end subroutine  
end module
