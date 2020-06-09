#ifndef PUTILS_QUARTERNION_HPP
#define PUTILS_QUARTERNION_HPP
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <limits>
#include <complex>
#include <sstream>
#include <complex>
namespace putils {

template < typename T >
class biquarternion
{
private:
    std::complex<T> m_x,m_y,m_z,m_t;
public:

    biquarternion():m_x(0),m_y(0),m_z(0),m_t(0) {}

    explicit biquarternion(const std::complex<T>& t, const std::complex<T>& x, const std::complex<T>& y, 
        const std::complex<T>& z):
        m_x(x),m_y(y),m_z(z),m_t(t) {}

    explicit biquarternion(const T& t, const T& x, const T& y, 
        const T& z):
        m_x(x),m_y(y),m_z(z),m_t(t) {}

    biquarternion& operator=(const T& x) noexcept {
        m_x=T(0);
        m_y=T(0);
        m_z=T(0);
        m_t = x;
        return *this;
    }

    biquarternion& operator=(const std::complex<T>& x) noexcept {
        m_x=T(0);
        m_y=T(0);
        m_z=T(0);
        m_t = x;
        return *this;
    }

    biquarternion& operator += (const T& x) noexcept {
        m_t += x;
        return *this;
    }

    biquarternion& operator -= (const T& x) noexcept {
        m_t -= x;
        return *this;
    }

    biquarternion& operator *= (const T& x) noexcept {
        m_x *= x;
        m_y *= x;
        m_z *= x;
        m_t *= x;
        return *this;
    }

    biquarternion& operator /= (const T& x) noexcept {
        T xi = T(1)/x;
        m_x *= xi;
        m_y *= xi;
        m_z *= xi;
        m_t *= xi;
        return *this;
    }

    biquarternion& operator += (const std::complex<T>& x) noexcept {
        m_t += x;
        return *this;
    }

    biquarternion& operator -= (const std::complex<T>& x) noexcept {
        m_t -= x;
        return *this;
    }

    biquarternion& operator *= (const std::complex<T>& x) noexcept {
        m_x *= x;
        m_y *= x;
        m_z *= x;
        m_t *= x;
        return *this;
    }

    biquarternion& operator /= (const std::complex<T>& x) noexcept {
        T xi = T(1)/x;
        m_x *= xi;
        m_y *= xi;
        m_z *= xi;
        m_t *= xi;
        return *this;
    }


    biquarternion& operator=(const biquarternion<T>& qx) noexcept {
        m_x= qx.m_x;
        m_y= qx.m_y;
        m_z= qx.m_z;
        m_t= qx.m_t;
        return *this;
    }
    biquarternion& operator += (const biquarternion<T>& qx) noexcept {
        m_x+= qx.m_x;
        m_y+= qx.m_y;
        m_z+= qx.m_z;
        m_t+= qx.m_t;
        return *this;
    }
    biquarternion& operator -= (const biquarternion<T>& qx) noexcept {
        m_x-= qx.m_x;
        m_y-= qx.m_y;
        m_z-= qx.m_z;
        m_t-= qx.m_t;
        return *this;
    }
    biquarternion& operator *= (const biquarternion<T>& qx) noexcept {
        std::complex<T> t_x = m_y * qx.m_z - m_z * qx.m_y + m_x * qx.m_t + m_t * qx.m_x;
        std::complex<T> t_y = m_z * qx.m_x - m_x * qx.m_z + m_y * qx.m_t + m_t * qx.m_y;
        std::complex<T> t_z = m_x * qx.m_y - m_y * qx.m_x + m_z * qx.m_t + m_t * qx.m_z;;
        std::complex<T> t_t = m_t * qx.m_t - m_x * qx.m_x - m_y * qx.m_y - m_z * qx.m_z;
        m_x = t_x;
        m_y = t_y;
        m_z = t_z;
        m_t = t_t;
        return *this;
    }

    biquarternion& operator /= (const biquarternion<T>& qx) noexcept {
        std::complex<T> t_x = -m_y * qx.m_z + m_z * qx.m_y + m_x * qx.m_t - m_t * qx.m_x;
        std::complex<T> t_y = -m_z * qx.m_x + m_x * qx.m_z + m_y * qx.m_t - m_t * qx.m_y;
        std::complex<T> t_z = -m_x * qx.m_y + m_y * qx.m_x + m_z * qx.m_t - m_t * qx.m_z;;
        std::complex<T> t_t = m_t * qx.m_t + m_x * qx.m_x + m_y * qx.m_y + m_z * qx.m_z;
        T nrm = qx.norm();
        m_x = t_x/nrm;
        m_y = t_y/nrm;
        m_z = t_z/nrm;
        m_t = t_t/nrm;
        return *this;
    }

    constexpr bool eq(const biquarternion& z) const noexcept {
        T d0 = std::norm(m_t-z.m_t);
        T dx = std::norm(m_x-z.m_x);
        T dy = std::norm(m_y-z.m_y);
        T dz = std::norm(m_z-z.m_z);
        T dd = d0+dx+dy+dz;
        return (dd < std::numeric_limits<T>::epsilon());
    }
    constexpr bool neq(const biquarternion& z) const noexcept {
        T d0 = std::norm(m_t-z.m_t);
        T dx = std::norm(m_x-z.m_x);
        T dy = std::norm(m_y-z.m_y);
        T dz = std::norm(m_z-z.m_z);
        T dd = d0+dx+dy+dz;
        return (dd > std::numeric_limits<T>::epsilon());
    }

    biquarternion conj() const noexcept {
        return biquarternion<T>(m_t,-m_x,-m_y,-m_z);
    }
    
    biquarternion complex_conj() const noexcept {
        return biquarternion<T>(std::conj(m_t),std::conj(m_x),std::conj(m_y),std::conj(m_z));    
    }

    biquarternion inverse() const noexcept {
        T p = this->norm();
        p = T(1)/p;
        return biquarternion<T>(m_t*p,-m_x*p,-m_y*p,-m_z*p);
    }

    constexpr T norm() const noexcept {
        return (std::norm(m_t) + std::norm(m_x) + std::norm(m_y) + std::norm(m_z));
    }

    biquarternion unit() const noexcept {
        T p = this->norm();
        if ( p < std::numeric_limits<T>::epsilon() ) {
            return biquarternion<T>();
        }
        p = T(1)/sqrt(p);
        return biquarternion<T>( p*m_t, p*m_x, p*m_y, p*m_z);
    }

    T vnorm() const noexcept {
        return std::norm(m_x)+std::norm(m_y)+std::norm(m_z);
    }

    std::ostream& write_to_stream(std::ostream& os) const {
        os << "[" << m_t << ", " << m_x << ", " << m_y << ", " << m_z << "]";
        return os;
    }

    std::istream& read_from_stream(std::istream& is) {
        T v;
        T vx,vy,vz,vt;
        char ch;
        while (1) {
            ch = is.peek();
            if (ch==' ') {
                // eat whitespace
                is >> ch;
                continue;
            }
            if (ch=='[') {
                // beginning of sequence
                is >> ch;
                break;
            }
            if (isdigit(ch) || ch=='-' || ch == '+' || ch=='(') {
                is >> m_t;
                m_x = m_y = m_z = T(0);
                return is;
            }
            // anything else is wrong
            throw std::ios::failure("istream >> biquarternion error");
        }
        is >> m_t;
        while (1) {
            ch = is.peek();
            if (ch==' ') {
                // eat whitespace
                is >> ch;
                continue;
            }
            if (ch==',') {
                // beginning of next value
                is >> ch;
                break;
            }
            // anything else is wrong
            throw std::ios::failure("istream >> biquarternion error");
        }
        is >> m_x;
        while (1) {
            ch = is.peek();
            if (ch==' ') {
                // eat whitespace
                is >> ch;
                continue;
            }
            if (ch==',') {
                // beginning of next value
                is >> ch;
                break;
            }
            // anything else is wrong
            throw std::ios::failure("istream >> biquarternion error");
        }
        is >> m_y;
        while (1) {
            ch = is.peek();
            if (ch==' ') {
                // eat whitespace
                is >> ch;
                continue;
            }
            if (ch==',') {
                // beginning of next value
                is >> ch;
                break;
            }
            // anything else is wrong
            throw std::ios::failure("istream >> biquarternion error");
        }
        is >> m_z;
        while (1) {
            ch = is.peek();
            if (ch==' ') {
                // eat whitespace
                is >> ch;
                continue;
            }
            if (ch==']') {
                // end of sequence
                is >> ch;
                return is;
            }
            // anything else is wrong
            throw std::ios::failure("istream >> biquarternion error");
        }
        return is;
    }

    inline std::complex<T> x() const noexcept {
        return m_x;
    }
    inline std::complex<T> y() const noexcept {
        return m_y;
    }
    inline std::complex<T> z() const noexcept {
        return m_z;
    }
    inline std::complex<T> t()  const noexcept {
        return m_t;
    }

    inline biquarternion<T> qvec() const noexcept {
        return biquarternion<T>(T(0),m_x,m_y,m_z);
    }

    inline T arg() const noexcept {
        return atan2( sqrt(std::norm(m_t)), sqrt(vnorm()));
    }

    inline T abs() const noexcept {
        return sqrt(norm());    
    }

};

template < typename T >
inline biquarternion<T> operator+( const biquarternion<T>& q1, const biquarternion<T>& q2) {
    biquarternion<T> res(q1);
    return (res+=q2);
}

template < typename T >
inline biquarternion<T> operator-( const biquarternion<T>& q1, const biquarternion<T>& q2) {
    biquarternion<T> res(q1);
    return (res-=q2);
}
template < typename T >
inline biquarternion<T> operator*( const biquarternion<T>& q1, const biquarternion<T>& q2) {
    biquarternion<T> res(q1);
    return (res*=q2);
}

template < typename T >
inline biquarternion<T> operator+( const biquarternion<T>& q1, const T& r) {
    biquarternion<T> res(q1);
    return (res+=r);
}

template < typename T >
inline biquarternion<T> operator-( const biquarternion<T>& q1, const T& r) {
    biquarternion<T> res(q1);
    return (res-=r);
}
template < typename T >
inline biquarternion<T> operator*( const biquarternion<T>& q1, const T& r) {
    biquarternion<T> res(q1);
    return (res*=r);
}

template < typename T >
inline biquarternion<T> operator/( const biquarternion<T>& q1, const T& r) {
    biquarternion<T> res(q1);
    T ri = T(1)/r;
    return (res*=ri);
}

template < typename T >
inline biquarternion<T> operator+( const T& r, const biquarternion<T>& q1) {
    biquarternion<T> res(q1);
    return (res+=r);
}

template < typename T >
inline biquarternion<T> operator-( const T& r, const biquarternion<T>& q1) {
    biquarternion<T> res(q1);
    return (res-=r);
}
template < typename T >
inline biquarternion<T> operator*( const T& r, const biquarternion<T>& q1) {
    biquarternion<T> res(q1);
    return (res*=r);
}

template < typename T >
inline biquarternion<T> operator/( const T& r, const biquarternion<T>& q1) {
    biquarternion<T> res(q1.inverse());
    biquarternion<T> rx(r,T(0),T(0),T(0));
    rx *= res;
    return rx;
}


template < typename T >
inline biquarternion<T> operator+( const biquarternion<T>& q1, const std::complex<T>& r) {
    biquarternion<T> res(q1);
    return (res+=r);
}

template < typename T >
inline biquarternion<T> operator-( const biquarternion<T>& q1, const std::complex<T>& r) {
    biquarternion<T> res(q1);
    return (res-=r);
}
template < typename T >
inline biquarternion<T> operator*( const biquarternion<T>& q1, const std::complex<T>& r) {
    biquarternion<T> res(q1);
    return (res*=r);
}

template < typename T >
inline biquarternion<T> operator/( const biquarternion<T>& q1, const std::complex<T>& r) {
    biquarternion<T> res(q1);
    T ri = T(1)/r;
    return (res*=ri);
}

template < typename T >
inline biquarternion<T> operator+( const std::complex<T>& r, const biquarternion<T>& q1) {
    biquarternion<T> res(q1);
    return (res+=r);
}

template < typename T >
inline biquarternion<T> operator-( const std::complex<T>& r, const biquarternion<T>& q1) {
    biquarternion<T> res(q1);
    return (res-=r);
}
template < typename T >
inline biquarternion<T> operator*( const std::complex<T>& r, const biquarternion<T>& q1) {
    biquarternion<T> res(q1);
    return (res*=r);
}

template < typename T >
inline biquarternion<T> operator/( const std::complex<T>& r, const biquarternion<T>& q1) {
    biquarternion<T> res(q1.inverse());
    biquarternion<T> rx(r,std::complex<T>(0),std::complex<T>(0),std::complex<T>(0));
    rx *= res;
    return rx;
}




// return q1 * pow(q2,-1)
template < typename T >
inline biquarternion<T> rdiv(const biquarternion<T>& q1, const biquarternion<T>& q2)
{
    biquarternion<T> qx{q2.inverse()};
    return q1 * qx;
}

// return pow(q2,-1) * q1
template < typename T >
inline biquarternion<T> ldiv(const biquarternion<T>& q1, const biquarternion<T>& q2)
{
    biquarternion<T> qx{q2.inverse()};
    return qx * q1;
}

template < typename T > inline std::ostream& operator << ( std::ostream& os,
        const biquarternion<T>& q) {
    return q.write_to_stream(os);
}

template < typename T > inline std::istream& operator >> ( std::istream& is,
        biquarternion<T>& q) {
    return q.read_from_stream(is);
}



template < typename T> inline biquarternion<T> ihat()
{
    return biquarternion(std::complex<T>(0),std::complex<T>(1),std::complex<T>(0),std::complex<T>(0));
}

template < typename T> inline biquarternion<T> jhat()
{
    return biquarternion(std::complex<T>(0),std::complex<T>(0),std::complex<T>(1),std::complex<T>(0));
}

template < typename T> inline biquarternion<T> khat()
{
    return biquarternion(std::complex<T>(0),std::complex<T>(0),std::complex<T>(0),std::complex<T>(1));
}

template < typename T> inline biquarternion<T> no_hat()
{
    return biquarternion(std::complex<T>(1),std::complex<T>(0),std::complex<T>(0),std::complex<T>(0));
}

template < typename T > inline bool operator==(const biquarternion<T>& a,const biquarternion<T>& b) noexcept
{
    return a.eq(b);
}

template < typename T > inline bool operator!=(const biquarternion<T>& a,const biquarternion<T>& b) noexcept
{
    return a.neq(b);
}


template < typename T > inline biquarternion<T> normProduct(const biquarternion<T>& x,const biquarternion<T>& y) {
    biquarternion<T> yc = y.conj();
    return (x * yc + yc * x)*T(0.5f);
}

template < typename T > inline biquarternion<T> normDivision(const biquarternion<T>& x,const biquarternion<T>& y) {
    biquarternion<T> yc = y.conj()/y.norm();
    return (x * yc + yc * x)*T(0.5f);
}

}
#endif