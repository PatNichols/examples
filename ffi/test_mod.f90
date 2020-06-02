Module test_mod
      Use, intrinsic :: iso_c_binding
      Use, intrinsic :: iso_fortran_env
      Implicit None
      Public test_t
!
      Type, abstract :: test_t
         Real (c_double), Pointer, Dimension (:, :, :) :: a
         Real (c_double), Pointer, Dimension (:, :) :: b
         Real (c_double), Pointer, Dimension (:) :: c
         Real (c_double), Pointer :: d
      Contains
         procedure (test_set_a_value_int), deferred :: set_a_value
         procedure (test_set_b_value_int), deferred :: set_b_value
         procedure (test_set_c_value_int), deferred :: set_c_value
         procedure (test_set_d_value_int), deferred :: set_d_value
         procedure (test_get_a_value_int), deferred :: get_a_value
         procedure (test_get_b_value_int), deferred :: get_b_value
         procedure (test_get_c_value_int), deferred :: get_c_value
         procedure (test_get_d_value_int), deferred :: get_d_value
      End Type test_t
!
      Interface
         Subroutine test_set_a_value_int (obj, x, i, j, k)
            Use iso_c_binding
            import test_t
            class (test_t), intent (inout) :: obj
            Integer (c_int64_t), Intent (In) :: i, j, k
            Real (c_double), Intent (In) :: x
         End Subroutine test_set_a_value_int
!-----
         Subroutine test_set_b_value_int (obj, x, i, j)
            Use iso_c_binding
            import test_t
            class (test_t), intent (inout) :: obj
            Integer (c_int64_t), Intent (In) :: i, j
            Real (c_double), Intent (In) :: x
         End Subroutine test_set_b_value_int
!-----
         Subroutine test_set_c_value_int (obj, x, i)
            Use iso_c_binding
            import test_t
            class (test_t), intent (inout) :: obj
            Integer (c_int64_t), Intent (In) :: i
            Real (c_double), Intent (In) :: x
         End Subroutine test_set_c_value_int
!-----
         Subroutine test_set_d_value_int (obj, x)
            Use iso_c_binding
            import test_t
            class (test_t), intent (inout) :: obj
            Real (c_double), Intent (In) :: x
         End Subroutine test_set_d_value_int
!-----
         Function test_get_a_value_int (obj, i, j, k) Result (x)
            Use iso_c_binding
            import test_t
            class (test_t), intent (inout) :: obj
            Integer (c_int64_t), Intent (In) :: i, j, k
            Real (c_double) :: x
         End Function test_get_a_value_int
!-----
         Function test_get_b_value_int (obj, i, j) Result (x)
            Use iso_c_binding
            import test_t
            class (test_t), intent (inout) :: obj
            Integer (c_int64_t), Intent (In) :: i, j
            Real (c_double) :: x
         End Function
!-----
         Function test_get_c_value_int (obj, i) Result (x)
            Use iso_c_binding
            import test_t
            class (test_t), intent (inout) :: obj
            Integer (c_int64_t), Intent (In) :: i
            Real (c_double) :: x
         End Function
!-----
         Function test_get_d_value_int (obj) Result (x)
            Use iso_c_binding
            import test_t
            class (test_t), intent (inout) :: obj
            Real (c_double) :: x
         End Function
      End Interface
!
      Type cpp_test_impl_t
         Real (c_double), Pointer, Dimension (:, :, :) :: a
         Real (c_double), Pointer, Dimension (:, :) :: b
         Real (c_double), Pointer, Dimension (:) :: c
         Real (c_double), Pointer :: d
         Type (c_ptr), Private :: ptr = c_null_ptr
      Contains
         procedure cpp_test_set_a_value
         procedure cpp_test_set_b_value
         procedure cpp_test_set_c_value
         procedure cpp_test_set_d_value
         procedure cpp_test_get_a_value
         procedure cpp_test_get_b_value
         procedure cpp_test_get_c_value
         procedure cpp_test_get_d_value
      End Type cpp_test_impl_t
!
      Type, extends (test_t) :: c_test_t
         Type (cpp_test_impl_t), Allocatable :: ptr
      Contains
         procedure :: set_a_value => c_test_set_a_value
         procedure :: set_b_value => c_test_set_b_value
         procedure :: set_c_value => c_test_set_c_value
         procedure :: set_d_value => c_test_set_d_value
         procedure :: get_a_value => c_test_get_a_value
         procedure :: get_b_value => c_test_get_b_value
         procedure :: get_c_value => c_test_get_c_value
         procedure :: get_d_value => c_test_get_d_value
      End Type c_test_t
!
      Type, extends (test_t) :: f_test_t
      Contains
         procedure :: set_a_value => f_test_set_a_value
         procedure :: set_b_value => f_test_set_b_value
         procedure :: set_c_value => f_test_set_c_value
         procedure :: set_d_value => f_test_set_d_value
         procedure :: get_a_value => f_test_get_a_value
         procedure :: get_b_value => f_test_get_b_value
         procedure :: get_c_value => f_test_get_c_value
         procedure :: get_d_value => f_test_get_d_value
      End Type f_test_t
!
      Interface
!-----
!   c interface functions for c++ class
!-----
         Function cpp_test_impl_t_init () Result (obj) bind (c, name="c&
        &pp_test_impl_t_init")
            Use, intrinsic :: iso_c_binding
            Type (c_ptr) :: obj
         End Function
!-----
         Subroutine cpp_test_impl_t_free (obj) bind (c, name="cpp_test_&
        &impl_t_free")
            Use, intrinsic :: iso_c_binding
            Type (c_ptr), Intent (Inout) :: obj
         End Subroutine
!-----
         Subroutine cpp_test_impl_t_set_a_value (obj, x, i, j, k) bind &
        & (c, name="cpp_test_impl_t_set_a_value")
            Use, intrinsic :: iso_c_binding
            Type (c_ptr), Intent (In), value :: obj
            Integer (c_int64_t), Intent (In), value :: i, j, k
            Real (c_double), Intent (In), value :: x
         End Subroutine
!-----
         Subroutine cpp_test_impl_t_set_b_value (obj, x, i, j) bind (c, &
        & name="cpp_test_impl_t_set_b_value")
            Use, intrinsic :: iso_c_binding
            Type (c_ptr), Intent (In), value :: obj
            Integer (c_int64_t), Intent (In), value :: i, j
            Real (c_double), Intent (In), value :: x
         End Subroutine
!-----
         Subroutine cpp_test_impl_t_set_c_value (obj, x, i) bind (c, &
        & name="cpp_test_impl_t_set_c_value")
            Use, intrinsic :: iso_c_binding
            Type (c_ptr), Intent (In), value :: obj
            Integer (c_int64_t), Intent (In), value :: i
            Real (c_double), Intent (In), value :: x
         End Subroutine
!-----
         Subroutine cpp_test_impl_t_set_d_value (obj, x) bind (c, &
        & name="cpp_test_impl_t_set_d_value")
            Use, intrinsic :: iso_c_binding
            Type (c_ptr), Intent (In), value :: obj
            Real (c_double), Intent (In), value :: x
         End Subroutine
!-----
         Function cpp_test_impl_t_get_a_value (obj, i, j, k) Result (x) &
        & bind (c, name="cpp_test_impl_t_get_a_value")
            Use, intrinsic :: iso_c_binding
            Type (c_ptr), Intent (In), value :: obj
            Integer (c_int64_t), Intent (In), value :: i, j, k
            Real (c_double) :: x
         End Function
!-----
         Function cpp_test_impl_t_get_b_value (obj, i, j) Result (x) &
        & bind (c, name="cpp_test_impl_t_get_b_value")
            Use, intrinsic :: iso_c_binding
            Type (c_ptr), Intent (In), value :: obj
            Integer (c_int64_t), Intent (In), value :: i, j
            Real (c_double) :: x
         End Function
!-----
         Function cpp_test_impl_t_get_c_value (obj, i) Result (x) bind &
        & (c, name="cpp_test_impl_t_get_c_value")
            Use, intrinsic :: iso_c_binding
            Type (c_ptr), Intent (In), value :: obj
            Integer (c_int64_t), Intent (In), value :: i
            Real (c_double) :: x
         End Function
!-----
         Function cpp_test_impl_t_get_d_value (obj) Result (x) bind (c, &
        & name="cpp_test_impl_t_get_d_value")
            Use, intrinsic :: iso_c_binding
            Type (c_ptr), Intent (In), value :: obj
            Real (c_double) :: x
         End Function
!-----
         Function cpp_test_impl_t_get_a_pointer (obj) Result (x) bind &
        & (c, name="cpp_test_impl_t_get_a_pointer")
            Use, intrinsic :: iso_c_binding
            Type (c_ptr), Intent (In), value :: obj
            Type (c_ptr) :: x
         End Function
!-----
         Function cpp_test_impl_t_get_b_pointer (obj) Result (x) bind &
        & (c, name="cpp_test_impl_t_get_b_pointer")
            Use, intrinsic :: iso_c_binding
            Type (c_ptr), Intent (In), value :: obj
            Type (c_ptr) :: x
         End Function
!-----
         Function cpp_test_impl_t_get_c_pointer (obj) Result (x) bind &
        & (c, name="cpp_test_impl_t_get_c_pointer")
            Use, intrinsic :: iso_c_binding
            Type (c_ptr), Intent (In), value :: obj
            Type (c_ptr) :: x
         End Function
!-----
         Function cpp_test_impl_t_get_d_pointer (obj) Result (x) bind &
        & (c, name="cpp_test_impl_t_get_d_pointer")
            Use, intrinsic :: iso_c_binding
            Type (c_ptr), Intent (In), value :: obj
            Type (c_ptr) :: x
         End Function
!----
      End Interface
!
Contains
!
!====
! cpp functions
!===
!-----
      Function cpp_test_init () Result (obj)
         Use, intrinsic :: iso_c_binding
         Implicit None
         Type (cpp_test_impl_t) :: obj
         Type (c_ptr) :: c_a_ptr
         Type (c_ptr) :: c_b_ptr
         Type (c_ptr) :: c_c_ptr
         Type (c_ptr) :: c_d_ptr
         obj%ptr = cpp_test_impl_t_init ()
         c_a_ptr = cpp_test_impl_t_get_a_pointer (obj%ptr)
         c_b_ptr = cpp_test_impl_t_get_b_pointer (obj%ptr)
         c_c_ptr = cpp_test_impl_t_get_c_pointer (obj%ptr)
         c_d_ptr = cpp_test_impl_t_get_d_pointer (obj%ptr)
         Call c_f_pointer (c_a_ptr, obj%a, (/ 2, 3, 4 /))
         Call c_f_pointer (c_b_ptr, obj%b, (/ 3, 4 /))
         Call c_f_pointer (c_c_ptr, obj%c, (/ 4 /))
         Call c_f_pointer (c_d_ptr, obj%d)
      End Function
!-----
      Subroutine cpp_test_free (obj)
         Use, intrinsic :: iso_c_binding
         Implicit None
         class (cpp_test_impl_t), intent (inout) :: obj
         If (c_associated(obj%ptr)) Then
            Call cpp_test_impl_t_free (obj%ptr)
         End If
      End Subroutine
!-----
      Subroutine cpp_test_set_a_value (obj, x, i, j, k)
         Use, intrinsic :: iso_c_binding
         class (cpp_test_impl_t), intent (inout) :: obj
         Integer (c_int64_t), Intent (In) :: i, j, k
         Real (c_double), Intent (In) :: x
         Call cpp_test_impl_t_set_a_value (obj%ptr, x, i-1, j-1, k-1)
      End Subroutine
!-----
      Subroutine cpp_test_set_b_value (obj, x, i, j)
         Use, intrinsic :: iso_c_binding
         class (cpp_test_impl_t), intent (inout) :: obj
         Integer (c_int64_t), Intent (In) :: i, j
         Real (c_double), Intent (In) :: x
         Call cpp_test_impl_t_set_b_value (obj%ptr, x, i-1, j-1)
      End Subroutine
!-----
      Subroutine cpp_test_set_c_value (obj, x, i)
         Use, intrinsic :: iso_c_binding
         class (cpp_test_impl_t), intent (inout) :: obj
         Integer (c_int64_t), Intent (In) :: i
         Integer (c_int64_t) :: im1
         Real (c_double), Intent (In) :: x
         im1 = i - 1
         Call cpp_test_impl_t_set_c_value (obj%ptr, x, im1)
      End Subroutine
!-----
      Subroutine cpp_test_set_d_value (obj, x)
         Use, intrinsic :: iso_c_binding
         class (cpp_test_impl_t), intent (inout) :: obj
         Real (c_double), Intent (In) :: x
         Call cpp_test_impl_t_set_d_value (obj%ptr, x)
      End Subroutine
!-----
      Function cpp_test_get_a_value (obj, i, j, k) Result (x)
         Use, intrinsic :: iso_c_binding
         class (cpp_test_impl_t), intent (inout) :: obj
         Integer (c_int64_t), Intent (In) :: i, j, k
         Integer (c_int64_t) :: im1, jm1, km1
         Real (c_double) :: x
         im1 = i - 1
         jm1 = j - 1
         km1 = k - 1
         x = cpp_test_impl_t_get_a_value (obj%ptr, im1, jm1, km1)
      End Function
!-----
      Function cpp_test_get_b_value (obj, i, j) Result (x)
         Use, intrinsic :: iso_c_binding
         class (cpp_test_impl_t), intent (inout) :: obj
         Integer (c_int64_t), Intent (In) :: i, j
         Integer (c_int64_t) :: im1, jm1
         Real (c_double) :: x
         im1 = i - 1
         jm1 = j - 1
         x = cpp_test_impl_t_get_b_value (obj%ptr, im1, jm1)
      End Function
!-----
      Function cpp_test_get_c_value (obj, i) Result (x)
         Use, intrinsic :: iso_c_binding
         class (cpp_test_impl_t), intent (inout) :: obj
         Integer (c_int64_t), Intent (In) :: i
         Integer (c_int64_t) :: im1
         Real (c_double) :: x
         im1 = i - 1
         x = cpp_test_impl_t_get_c_value (obj%ptr, im1)
      End Function
!-----
      Function cpp_test_get_d_value (obj) Result (x)
         Use, intrinsic :: iso_c_binding
         class (cpp_test_impl_t), intent (inout) :: obj
         Real (c_double) :: x
         x = cpp_test_impl_t_get_d_value (obj%ptr)
      End Function
!====
! fortran functions
!====
      Subroutine f_test_init (obj)
         Use, intrinsic :: iso_c_binding
         Implicit None
         class (f_test_t), intent (inout), allocatable :: obj
         Integer :: i, j, k
         Real (c_double) :: acnt, bcnt, ccnt
         Allocate (obj)
         Allocate (obj%a(2, 3, 4))
         Allocate (obj%b(3, 4))
         Allocate (obj%c(4))
         Allocate (obj%d)
         acnt = 0.
         bcnt = 0.
         ccnt = 0.
         obj%d = 2.4d0
         Do i = 1, 4
            Do j = 1, 3
               Do k = 1, 2
                  obj%a (k, j, i) = acnt
                  acnt = acnt + 1.0
               End Do
               obj%b (j, i) = bcnt
               bcnt = bcnt + 1.0
            End Do
            obj%c (i) = ccnt
            ccnt = ccnt + 1.0
         End Do
      End Subroutine
!-----
      Subroutine f_test_free (obj)
         Use, intrinsic :: iso_c_binding
         Implicit None
         class (f_test_t), intent (inout) :: obj
         Deallocate (obj%d)
         Deallocate (obj%c)
         Deallocate (obj%b)
         Deallocate (obj%a)
      End Subroutine
!------
      Subroutine f_test_set_a_value (obj, x, i, j, k)
         Use, intrinsic :: iso_c_binding
         class (f_test_t), intent (inout) :: obj
         Real (c_double), Intent (In) :: x
         Integer (c_int64_t), Intent (In) :: i, j, k
         obj%a (i, j, k) = x
      End Subroutine
!------
      Subroutine f_test_set_b_value (obj, x, i, j)
         Use, intrinsic :: iso_c_binding
         class (f_test_t), intent (inout) :: obj
         Real (c_double), Intent (In) :: x
         Integer (c_int64_t), Intent (In) :: i, j
         obj%b (i, j) = x
      End Subroutine
!------
      Subroutine f_test_set_c_value (obj, x, i)
         Use, intrinsic :: iso_c_binding
         class (f_test_t), intent (inout) :: obj
         Real (c_double), Intent (In) :: x
         Integer (c_int64_t), Intent (In) :: i
         obj%c (i) = x
      End Subroutine
!------
      Subroutine f_test_set_d_value (obj, x)
         Use, intrinsic :: iso_c_binding
         class (f_test_t), intent (inout) :: obj
         Real (c_double), Intent (In) :: x
         obj%d = x
      End Subroutine
!------
      Function f_test_get_a_value (obj, i, j, k) Result (x)
         Use, intrinsic :: iso_c_binding
         class (f_test_t), intent (inout) :: obj
         Real (c_double) :: x
         Integer (c_int64_t), Intent (In) :: i, j, k
         x = obj%a (i, j, k)
      End Function
!------
      Function f_test_get_b_value (obj, i, j) Result (x)
         Use, intrinsic :: iso_c_binding
         class (f_test_t), intent (inout) :: obj
         Real (c_double) :: x
         Integer (c_int64_t), Intent (In) :: i, j
         x = obj%b (i, j)
      End Function
!------
      Function f_test_get_c_value (obj, i) Result (x)
         Use, intrinsic :: iso_c_binding
         class (f_test_t), intent (inout) :: obj
         Real (c_double) :: x
         Integer (c_int64_t), Intent (In) :: i
         x = obj%c (i)
      End Function
!------
      Function f_test_get_d_value (obj) Result (x)
         Use, intrinsic :: iso_c_binding
         class (f_test_t), intent (inout) :: obj
         Real (c_double) :: x
         x = obj%d
      End Function
!========
! c implentation functions
!========
      Subroutine c_test_init (obj)
         Use, intrinsic :: iso_c_binding
         Implicit None
         class (c_test_t), intent (inout), allocatable :: obj
         Allocate (c_test_t :: obj)
         obj%ptr = cpp_test_init ()
         obj%a => obj%ptr%a
         obj%b => obj%ptr%b
         obj%c => obj%ptr%c
         obj%d => obj%ptr%d
      End Subroutine
!-----
      Subroutine c_test_free (obj)
         Use, intrinsic :: iso_c_binding
         Implicit None
         class (c_test_t), intent (inout) :: obj
         Call cpp_test_free (obj%ptr)
      End Subroutine
!------
      Subroutine c_test_set_a_value (obj, x, i, j, k)
         Use, intrinsic :: iso_c_binding
         class (c_test_t), intent (inout) :: obj
         Real (c_double), Intent (In) :: x
         Integer (c_int64_t), Intent (In) :: i, j, k
         Call cpp_test_set_a_value (obj%ptr, x, i, j, k)
      End Subroutine
!------
      Subroutine c_test_set_b_value (obj, x, i, j)
         Use, intrinsic :: iso_c_binding
         class (c_test_t), intent (inout) :: obj
         Real (c_double), Intent (In) :: x
         Integer (c_int64_t), Intent (In) :: i, j
         Call cpp_test_set_b_value (obj%ptr, x, i, j)
      End Subroutine
!------
      Subroutine c_test_set_c_value (obj, x, i)
         Use, intrinsic :: iso_c_binding
         class (c_test_t), intent (inout) :: obj
         Real (c_double), Intent (In) :: x
         Integer (c_int64_t), Intent (In) :: i
         Call cpp_test_set_c_value (obj%ptr, x, i)
      End Subroutine
!------
      Subroutine c_test_set_d_value (obj, x)
         Use, intrinsic :: iso_c_binding
         class (c_test_t), intent (inout) :: obj
         Real (c_double), Intent (In) :: x
         Call cpp_test_set_d_value (obj%ptr, x)
      End Subroutine
!------
      Function c_test_get_a_value (obj, i, j, k) Result (x)
         Use, intrinsic :: iso_c_binding
         class (c_test_t), intent (inout) :: obj
         Real (c_double) :: x
         Integer (c_int64_t), Intent (In) :: i, j, k
         x = cpp_test_get_a_value (obj%ptr, i, j, k)
      End Function
!------
      Function c_test_get_b_value (obj, i, j) Result (x)
         Use, intrinsic :: iso_c_binding
         class (c_test_t), intent (inout) :: obj
         Real (c_double) :: x
         Integer (c_int64_t), Intent (In) :: i, j
         x = cpp_test_get_b_value (obj%ptr, i, j)
      End Function
!------
      Function c_test_get_c_value (obj, i) Result (x)
         Use, intrinsic :: iso_c_binding
         class (c_test_t), intent (inout) :: obj
         Real (c_double) :: x
         Integer (c_int64_t), Intent (In) :: i
         x = cpp_test_get_c_value (obj%ptr, i)
      End Function
!------
      Function c_test_get_d_value (obj) Result (x)
         Use, intrinsic :: iso_c_binding
         class (c_test_t), intent (inout) :: obj
         Real (c_double) :: x
         x = cpp_test_get_d_value (obj%ptr)
      End Function
End Module test_mod
!
