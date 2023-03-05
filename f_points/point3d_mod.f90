module point3d_mod
      use,intrinsic :: iso_fortran_env
      use :: point2d_mod 
      type :: point3d
          real(real64) :: x,y,z
      contains
          procedure,private,pass(this) :: point3d_copy
          procedure,private,pass(this) :: point3d_copy_point2d
          generic,public :: assignment(=) => point3d_copy,point3d_copy_point2d
          procedure,private,pass(this) :: point3d_add
          procedure,private,pass(this) :: point3d_add_32
          procedure,private,pass(this) :: point3d_add_23
          generic,public :: operator(+) => point3d_add,point3d_add_32,point3d_add_23 
      end type    
contains
      subroutine point3d_copy(this,p_from) 
          implicit none
          class(point3d),intent(in) :: p_from
          class(point3d),intent(out) :: this
          this%x = p_from%x
          this%y = p_from%y
          this%z = p_From%z 
      end subroutine       
      subroutine point3d_copy_point2d(this,p_from) 
          implicit none
          class(point2d),intent(in) :: p_from
          class(point3d),intent(out) :: this
          this%x = p_from%x
          this%y = p_from%y
          this%z = 0.0d0
      end subroutine       
      pure function point3d_add(this,rhs) result (total)
          implicit none
          class(point3d),intent(in) :: this
          class(point3d),intent(in) :: rhs
          type(point3d) :: total 
          total%x = this%x + rhs%x
          total%y = this%y + rhs%y
          total%z = this%z + rhs%z 
      end function
      pure function point3d_add_32(this,rhs) result (total)
          implicit none
          class(point3d),intent(in) :: this
          class(point2d),intent(in) :: rhs
          type(point3d) :: total 
          total%x = this%x + rhs%x
          total%y = this%y + rhs%y
          total%z = this%z
      end function
      pure function point3d_add_23(rhs,this) result (total)
          implicit none
          class(point3d),intent(in) :: this
          class(point2d),intent(in) :: rhs
          type(point3d) :: total 
          total%x = this%x + rhs%x
          total%y = this%y + rhs%y
          total%z = this%z
      end function

end module
