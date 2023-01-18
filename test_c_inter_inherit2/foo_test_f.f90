program foo_test_f
   use foo_mod
   implicit none
   type(foo) :: f1
   type(foo) :: fb
   type(foo) :: f2  
   integer :: k
   call foo_init(f1)
   call foobar_init(fb)
   write(*,*)'calling for foo'
   call f1%report()
   write(*,*)'calling for foobar'
   call fb%report()
   k = f1%get_int()
   write(*,*)'foo k = ',k
   k = fb%get_int()
   write(*,*)'foobar k = ',k
   f2 = f1
   call f2%report()    
   k = f1%get_int()
   write(*,*)'foo2 k = ',k   
end program
