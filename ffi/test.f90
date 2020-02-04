Program test
      Use, intrinsic :: iso_c_binding
      Use test_mod
      Implicit None
      class (c_test_t), allocatable :: c_tst
      class (f_test_t), allocatable :: f_tst
      Real (c_double) :: x_c, x_f
      Integer (c_int64_t) :: i, j, k
!
      Call c_test_init (c_tst)
      Call f_test_init (f_tst)
      Print *, "a values"
      Do i = 1, 4
         Do j = 1, 3
            Do k = 1, 2
               x_c = c_test_get_a_value (c_tst, k, j, i)
               x_f = f_test_get_a_value (f_tst, k, j, i)
               Print *, x_c, " ", x_f, " ", (x_c-x_f)
            End Do
         End Do
      End Do
      Print *, "b values"
      Do i = 1, 4
         Do j = 1, 3
            x_c = c_test_get_b_value (c_tst, j, i)
            x_f = f_test_get_b_value (f_tst, j, i)
            Print *, x_c, " ", x_f, " ", (x_c-x_f)
         End Do
      End Do
      Print *, "c values"
      Do i = 1, 4
         x_c = c_test_get_c_value (c_tst, i)
         x_f = f_test_get_c_value (f_tst, i)
         Print *, x_c, " ", x_f, " ", (x_c-x_f)
      End Do
      x_c = c_test_get_d_value (c_tst)
      x_f = f_test_get_d_value (f_tst)
      Print *, "d values"
      Print *, x_c, " ", x_f, " ", (x_c-x_f)
      Print *, "a values"
      Do i = 1, 4
         Do j = 1, 3
            Do k = 1, 2
               x_c = c_tst%a (k, j, i)
               x_f = f_tst%a (k, j, i)
               Print *, x_c, " ", x_f, " ", (x_c-x_f)
            End Do
         End Do
      End Do
      Print *, "b values"
      Do i = 1, 4
         Do j = 1, 3
            x_c = c_tst%b (j, i)
            x_f = f_tst%b (j, i)
            Print *, x_c, " ", x_f, " ", (x_c-x_f)
         End Do
      End Do
      Print *, "c values"
      Do i = 1, 4
         x_c = c_tst%c (i)
         x_f = f_tst%c (i)
         Print *, x_c, " ", x_f, " ", (x_c-x_f)
      End Do
      Print *, "d values"
      x_c = c_tst%d
      x_f = f_tst%d
      Print *, x_c, " ", x_f, " ", (x_c-x_f)
      call c_test_free(c_tst)
      call f_test_free(f_tst)
      Stop
End Program
