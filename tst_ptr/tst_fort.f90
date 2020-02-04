program tst_ptr
  use iso_c_binding
  implicit none

  type a_t
    integer(c_int32_t), pointer :: px
  end type a_t

  class(a_t),allocatable :: bx
  class(a_t),allocatable :: ax
  integer(c_int32_t), pointer :: p1
  integer(c_int32_t), target, allocatable :: p0 

  allocate(bx)
  allocate(ax)
  ax%px => null()
  if (associated(p1).eqv..TRUE.) then
    print *,"p1 is_associated"
  else
    print *,"p1 is_not_assoc"
  end if

  if (associated(ax%px).eqv..TRUE.) then
    print *,"ax.px is_associated"
  else
    print *,"ax.px is_not_assoc"
  end if

  if (associated(bx%px).eqv..TRUE.) then
    print *,"bx.px is_associated"
  else
    print *,"bx.px is_not_assoc"
  end if

  if (allocated(p0).eqv..TRUE.) then
    print *,"p0 is_allocated"
  else
    print *,"p0 is_not_alloc"
  end if    

  allocate(p0)
  p0 = 1
  print *,"allocating p0"
  print *,"p0 = ",p0  
  p1=>p0
  ax%px => p0
  if (associated(p1).eqv..TRUE.) then
    print *,"p1 is_associated"
  else
    print *,"p1 is_not_assoc"
  end if

  if (associated(ax%px).eqv..TRUE.) then
    print *,"ax%px is_associated"
  else
    print *,"ax%px is_not_assoc"
  end if
  
  if (allocated(p0).eqv..TRUE.) then
    print *,"p0 is_allocated"
  else
    print *,"p0 is_not_alloc"
  end if    

end program tst_ptr  