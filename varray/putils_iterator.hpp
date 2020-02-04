


template < typename dst_iter, typename input_iter >
constexpr dst_iter copy( dst_iter diter, input_iter iter, input_iter ilast) noexcept
{
    while (iter!=ilast) {
        *diter = *iter;
        ++diter;
        ++iter;
    }
    return diter;
}

template < typename dst_iter, typename input_iter >
constexpr dst_iter copy( dst_iter diter, input_iter iter, size_type count) noexept
{
    for (size_type i=0;i<count;++i) {
        *diter = *iter;
        ++diter;
        ++iter;
    }
    return diter;
}

template < typename dst_iter, typename value_type >
constexpr dst_iter fill( dst_iter diter, dst_iter dlast, const value_type& v) noexcept
{
    while (diter!=dlast) {
        *diter = v;
        ++diter;
    }
    return diter;
}

template < typename dst_iter, typename value_type >
constexpr dst_iter fill( dst_iter diter, const value_type& v, size_type count) noexept
{
    for (size_type i=0;i<count;++i) {
        *diter = v;
        ++diter;
    }
    return diter;
}

template < typename iter_type >
constexpr forward_move_in_place(iter_type dst,iter_type iter,iter_type ilast)
{
    iter_type i0 = iter;
    while (iter!=ilast) {
        *dst = *iter;
        ++iter;
        ++dst;
    }
    return i0;
}

template < typename iter_type >
constexpr backward_move_in_place(iter_type dst,iter_type iter,iter_type ilast)
{
    iter_type i0 = iter;
    while (iter!=ilast) {
        *dst = *iter;
        --iter;
        --dst;
    }
    return dst;
}
