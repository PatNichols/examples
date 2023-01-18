module foo_mod
   use,intrinsic :: iso_c_binding
   use,intrinsic :: iso_fortran_env

   type :: foo
      type(c_ptr) :: data_ptr
   contains
      final :: foo_dealloc
      procedure,private,pass(this) :: copy => foo_copy
      procedure :: get_int => foo_get_int
      procedure :: report => foo_report   
      generic, public :: assignment(=) => copy
   end type 

interface
   function foo_c_alloc() result(p) bind(C,name="foo_c_alloc")
      import c_ptr
      type(c_ptr) :: p
   end function 

   function foobar_c_alloc() result(p) bind(C,name="foobar_c_alloc")
      import c_ptr
      type(c_ptr) :: p
   end function 

   subroutine foo_c_dealloc(p) bind(C,name="foo_c_dealloc")
      import c_ptr
      type(c_ptr) :: p
   end subroutine 

   subroutine foo_c_report(p) bind(C,name="foo_c_report")
      import c_ptr
      type(c_ptr),value :: p
   end subroutine 

   function foo_c_get_int(p) result(r) bind(C,name="foo_c_get_int") 
      import c_ptr,c_int
      type(c_ptr),value :: p
      integer(c_int) :: r
   end function
   
   subroutine foo_c_copy(pout,pin) bind(C,name="foo_c_copy")
      import c_ptr
      type(c_ptr),value :: pin
      type(c_ptr) :: pout
   end subroutine
      
end interface

contains
   subroutine foo_init(f)
      implicit none
      type(foo) :: f
      f%data_ptr = foo_c_alloc()  
   end subroutine

   subroutine foobar_init(f)
      implicit none
      type(foo) :: f
      f%data_ptr = foobar_c_alloc()  
   end subroutine
  
   subroutine foo_dealloc(f)
      implicit none
      type(foo) :: f 
      call foo_c_dealloc(f%data_ptr)
   end subroutine

   subroutine foo_copy(this,f)
      implicit none
      class(foo),intent(out) :: this
      class(foo),intent(in) :: f
      call foo_c_copy(this%data_ptr,f%data_ptr)
   end subroutine
   
   subroutine foo_report(f)
      implicit none
      class(foo),intent(in) :: f
      call foo_c_report(f%data_ptr) 
   end subroutine
   
   integer function foo_get_int(f) result(r)
      implicit none
      class(foo),intent(in) :: f
      r =  foo_c_get_int(f%data_ptr) 
   end function 
end module
