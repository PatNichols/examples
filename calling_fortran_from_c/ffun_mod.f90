module f_mod
    use,intrinsic :: iso_fortran_env  
    use,intrinsic :: iso_c_binding
contains
    subroutine f_fun1(a,b,c,n)
        real(real64) :: a,b,c
        integer(int32),intent(in) :: n
        real(real64) :: ta,tb,tc
        integer(int32) :: i

        write(error_unit,*)'f'
        write(error_unit,*)'a ',a,' b = ',b, 'c =',c    
        write(error_unit,*)'n ',n

        do i=1,n
                ta = a
                tb = b
                tc = c
                c = tb
                b = ta
                a = tc
        end do

        write(error_unit,*)'f'
        write(error_unit,*)'a ',a,' b = ',b, 'c =',c    
        write(error_unit,*)'n ',n
    end subroutine   

    subroutine c_fun1(a,b,c,n) bind(C,name="c_sub1")  
        real(c_double) :: a,b,c
        integer(c_int),value :: n
        call f_fun1(a,b,c,n)
    end subroutine
    
    real(real64) function f_fun2(a,b,c,n) result(val)
        real(real64),intent(in) :: a
        real(real64),intent(in) :: b
        real(real64),intent(in) :: c
        integer(int32),intent(in) :: n
        real(real64) :: sum
        integer(int32) :: i

        write(error_unit,*)'a ',a,' b = ',b, 'c =',c
        write(error_unit,*)'n ',n
        sum = 0.d0
        do i=1,n
           sum = sum + a*b + c
        end do
        write(error_unit,*)'sum = ',sum 
        val = sum
    end function

    function c_fun2(a,b,c,n) result(val) bind(C,name="c_fun2")
        real(c_double),value :: a
        real(c_double),value :: b
        real(c_double),value :: c
        integer(c_int),value :: n
        real(c_double) :: val
        val = f_fun2(a,b,c,n) 
    end function
    	 	
end module f_mod

