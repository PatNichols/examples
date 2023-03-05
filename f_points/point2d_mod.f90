module point2d_mod
      use,intrinsic :: iso_fortran_env 
      type :: point2d
          real(real64) :: x,y
      contains
          procedure,private,pass(this) :: point2d_copy
          generic,public :: assignment(=) => point2d_copy
          procedure,private,pass(this) :: point2d_add
          generic,public :: operator(+) => point2d_add
      end type    
contains
      subroutine point2d_copy(this,p_from) 
          implicit none
          class(point2d),intent(in) :: p_from
          class(point2d),intent(out) :: this
          this%x = p_from%x
          this%y = p_from%y
      end subroutine point2d_copy      

      pure function point2d_add(this,rhs) result(total)
          implicit none
          class(point2d),intent(in) :: this
          class(point2d),intent(in) :: rhs
          type(point2d) :: total
          total%x = this%x + rhs%x
          total%y = this%y + rhs%y  
      end function point2d_add

end module
