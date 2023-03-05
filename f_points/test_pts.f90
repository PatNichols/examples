program test_eq_point3d
      use point3d_mod
      use point2d_mod
      type(point3d) :: p0
      type(point3d) :: p1
      type(point3d) :: p2

      type(point2d) :: p3
      type(point2d) :: p4
      type(point2d) :: p5

            
      p1%x = 3.0
      p1%y = 2.0
      p1%z = 1.0
      p2%x = 0.0
      p2%y = 0.0
      p2%Z = 0.0
      p3%x = 12.0d0
      p3%y = 15.0d0
      write(*,*)'p2(3d)=p1(3d)' 
      write(*,'(a, f15.10, f15.10, f15.10)')'p1 before',p1%x,p1%y,p1%z
      write(*,'(a, f15.10, f15.10, f15.10)')'p2 before',p2%x,p2%y,p2%z
      p2 = p1
      write(*,'(a, f15.10, f15.10, f15.10)')'p1 after',p1%x,p1%y,p1%z
      write(*,'(a, f15.10, f15.10, f15.10)')'p2 after',p2%x,p2%y,p2%z

      if ( p2%x /= p1%x ) then 
         stop 'copy failed in x' 
      end if
      if ( p2%y /= p1%y ) then 
         stop 'copy failed in y' 
      end if
      if ( p2%z /= p1%z ) then 
         stop 'copy failed in z' 
      end if 

      write(*,*)'p2(3d)=p3(2d)' 
      write(*,'(a, f15.10, f15.10, f15.10)')'p2 before',p2%x,p2%y,p2%z
      write(*,'(a, f15.10, f15.10, f15.10)')'p3 before',p3%x,p3%y
      p2 = p3
      write(*,'(a, f15.10, f15.10, f15.10)')'p2 after',p2%x,p2%y,p2%z
      write(*,'(a, f15.10, f15.10, f15.10)')'p3 after',p3%x,p3%y

      if ( p2%x /= p3%x ) then 
         stop 'copy 2d to 3d failed in x' 
      end if
      if ( p2%y /= p3%y ) then 
         stop 'copy 2d to 3d failed in y' 
      end if
      if ( p2%z /= 0. ) then 
         stop 'copy 2d to 3d failed in z' 
      end if 
       
      p3%x = 0.0
      p3%y = 0.0
      p4%x = 1.0
      p4%y = 2.0
       write(*,*)'p2(2d)=p3(2d)'     
      write(*,'(a, f15.10, f15.10, f15.10)')'p4 before',p4%x,p4%y
      write(*,'(a, f15.10, f15.10, f15.10)')'p3 before',p3%x,p3%y
      p3 = p4
      write(*,'(a, f15.10, f15.10, f15.10)')'p4 after',p4%x,p4%y
      write(*,'(a, f15.10, f15.10, f15.10)')'p3 after',p3%x,p3%y
      
      if ( p3%x /= p4%x ) then
          stop 'point2d equal failed for x'
      end if
      if ( p3%y /= p4%y ) then
          stop 'point2d equal failed for y'
      end if
      
      !!! test add
      
      p0%x = 0.d0
      p0%y = 0.d0
      p0%z = 0.d0
      p1%x = 1.0
      p1%y = 2.0
      p1%z = 3.0
      p2%x = 3.0
      p2%y = 2.0
      p2%z = 3.0
      write(*,*)'p0(3d)=p1(3d)+p2(3d)' 
      write(*,'(a, f15.10, f15.10, f15.10)')'p0 before',p0%x,p0%y,p0%z
      write(*,'(a, f15.10, f15.10, f15.10)')'p1 before',p1%x,p1%y,p1%z
      write(*,'(a, f15.10, f15.10, f15.10)')'p2 before',p2%x,p2%y,p2%z
      p0 = p1 + p2
      write(*,'(a, f15.10, f15.10, f15.10)')'p0 before',p0%x,p0%y,p0%z
      write(*,'(a, f15.10, f15.10, f15.10)')'p1 before',p1%x,p1%y,p1%z
      write(*,'(a, f15.10, f15.10, f15.10)')'p2 before',p2%x,p2%y,p2%z

      p0%x = 1.d0
      p0%y = 2.d0
      p0%z = 3.d0
      p3%x = 3.0
      p3%y = 3.0
      p1%x = 0.0
      p1%y = 0.0
      p1%z = 0.0

      write(*,*)'p1(3d)=p0(3d)+p3(2d)'       
      write(*,'(a, f15.10, f15.10, f15.10)')'p1 before',p1%x,p1%y,p1%z
      write(*,'(a, f15.10, f15.10, f15.10)')'p0 before',p0%x,p0%y,p0%z
      write(*,'(a, f15.10, f15.10, f15.10)')'p3 before',p3%x,p3%y
      p1 = p0 + p3
      write(*,'(a, f15.10, f15.10, f15.10)')'p1 after',p1%x,p1%y,p1%z
      write(*,'(a, f15.10, f15.10, f15.10)')'p0 after',p0%x,p0%y,p0%z
      write(*,'(a, f15.10, f15.10, f15.10)')'p3 after',p3%x,p3%y      
      
      if ( p1%x /= (p0%x + p3%x)) then
           stop 'point3d+point2d failed in x'
      end if
      if ( p1%y /= (p0%y + p3%y)) then
           stop 'point3d+point2d failed in y'
      end if
      if ( p1%z /= (p0%z)) then
           stop 'point3d+point2d failed in z'
      end if
      
      p2%x = 0.0
      p2%y = 0.0
      p2%z = 0.0
      write(*,*)'p2(3d)=p3(3d)+p0(2d)' 
      write(*,'(a, f15.10, f15.10, f15.10)')'p2 before',p2%x,p2%y,p2%z
      write(*,'(a, f15.10, f15.10, f15.10)')'p0 before',p0%x,p0%y,p0%z
      write(*,'(a, f15.10, f15.10, f15.10)')'p3 before',p3%x,p3%y
      p2 = p3 + p0
      write(*,'(a, f15.10, f15.10, f15.10)')'p2 after',p2%x,p2%y,p2%z
      write(*,'(a, f15.10, f15.10, f15.10)')'p0 after',p0%x,p0%y,p0%z
      write(*,'(a, f15.10, f15.10, f15.10)')'p3 after',p3%x,p3%y      
            
      if ( p2%x /= (p3%x + p0%x)) then
           stop 'point2d+point3d failed in x'
      end if
      if ( p2%y /= (p3%y + p0%y)) then
           stop 'point2d+point3d failed in y'
      end if
      if ( p2%z /= (p0%z)) then
           stop 'point2d+point3d failed in z'
      end if

      p3%x = 3.
      p3%y = 2.
      p4%x = 1.
      p4%y = 2.
      write(*,*)'p5(2d)=p4(2d)+p5(2d)'       
      write(*,'(a, f15.10, f15.10, f15.10)')'p2 before',p3%x,p3%y
      write(*,'(a, f15.10, f15.10, f15.10)')'p0 before',p4%x,p4%y
      write(*,'(a, f15.10, f15.10, f15.10)')'p3 before',p5%x,p5%y
      p5 = p4 + p3
      write(*,'(a, f15.10, f15.10, f15.10)')'p2 after',p3%x,p3%y
      write(*,'(a, f15.10, f15.10, f15.10)')'p0 after',p4%x,p4%y
      write(*,'(a, f15.10, f15.10, f15.10)')'p3 after',p5%x,p5%y

      if ( p5%x /= (p3%x + p4%x)) then
           stop 'point2d+point2d failed in x'
      end if
      if ( p5%y /= (p3%y + p4%y)) then
           stop 'point2d+point2d failed in y'
      end if
end program            