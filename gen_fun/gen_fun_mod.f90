module gen_fun_mod
    use,intrinsic :: iso_c_binding
    use,intrinsic :: iso_fortran_env
    
    public my_fun
    
    interface my_fun
      module procedure f1
      module procedure f2
      module procedure f3
!      module procedure f4
    end interface

contains

    subroutine f1(i)
      integer i
    
      print *,"called f1 with ",i
    
    end subroutine f1    

    subroutine f2(r)
      real(c_double) r
    
      print *,"called f2 with ",r
    
    end subroutine f2     

    subroutine f3(i,r)
      real(c_double) r
      integer(c_int32_t) i
      print *,"called f3 with ",i, " and " ,r
    
    end subroutine f3   


!    subroutine f4(r,i)
!      real(c_double) :: r
!      integer(c_int32_t) :: i
!      print *,"called f4 with ",i," and ",r
!    end subroutine f4
      
end module