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
namespace putils {


template < typename T >
class quarternion
{
private:
    T m_x,m_y,m_z,m_t;
public:
    quarternion():m_x(0),m_y(0),m_z(0),m_t(0) {}

    explicit quarternion(const T& t, const T& x, const T& y, const T& z):
        m_x(x),m_y(y),m_z(z),m_t(t) {}

    quarternion& operator=(const T& x) noexcept {
        m_x=T(0);
        m_y=T(0);
        m_z=T(0);
        m_t = x;
        return *this;
    }

    quarternion& operator += (const T& x) noexcept {
        m_t += x;
        return *this;
    }

    quarternion& operator -= (const T& x) noexcept {
        m_t -= x;
        return *this;
    }

    quarternion& operator *= (const T& x) noexcept {
        m_x *= x;
        m_y *= x;
        m_z *= x;
        m_t *= x;
        return *this;
    }

    quarternion& operator /= (const T& x) noexcept {
        T xi = T(1)/x;
        m_x *= xi;
        m_y *= xi;
        m_z *= xi;
        m_t *= xi;
        return *this;
    }

    quarternion& operator=(const quarternion<T>& qx) noexcept {
        m_x= qx.m_x;
        m_y= qx.m_y;
        m_z= qx.m_z;
        m_t= qx.m_t;
        return *this;
    }
    quarternion& operator += (const quarternion<T>& qx) noexcept {
        m_x+= qx.m_x;
        m_y+= qx.m_y;
        m_z+= qx.m_z;
        m_t+= qx.m_t;
        return *this;
    }
    quarternion& operator -= (const quarternion<T>& qx) noexcept {
        m_x-= qx.m_x;
        m_y-= qx.m_y;
        m_z-= qx.m_z;
        m_t-= qx.m_t;
        return *this;
    }
    quarternion& operator *= (const quarternion<T>& qx) noexcept {
        T t_x = m_y * qx.m_z - m_z * qx.m_y + m_x * qx.m_t + m_t * qx.m_x;
        T t_y = m_z * qx.m_x - m_x * qx.m_z + m_y * qx.m_t + m_t * qx.m_y;
        T t_z = m_x * qx.m_y - m_y * qx.m_x + m_z * qx.m_t + m_t * qx.m_z;;
        T t_t = m_t * qx.m_t - m_x * qx.m_x - m_y * qx.m_y - m_z * qx.m_z;
        m_x = t_x;
        m_y = t_y;
        m_z = t_z;
        m_t = t_t;
        return *this;
    }

    quarternion& operator /= (const quarternion<T>& qx) noexcept {
        T t_x = -m_y * qx.m_z + m_z * qx.m_y + m_x * qx.m_t - m_t * qx.m_x;
        T t_y = -m_z * qx.m_x + m_x * qx.m_z + m_y * qx.m_t - m_t * qx.m_y;
        T t_z = -m_x * qx.m_y + m_y * qx.m_x + m_z * qx.m_t - m_t * qx.m_z;;
        T t_t = m_t * qx.m_t + m_x * qx.m_x + m_y * qx.m_y + m_z * qx.m_z;
        T nrm = qx.m_t * qx.m_t + qx.m_x*qx.m_x +qx.m_y * qx.m_y + qx.m_z * qx.m_z;
        m_x = t_x/nrm;
        m_y = t_y/nrm;
        m_z = t_z/nrm;
        m_t = t_t/nrm;
        return *this;
    }

    constexpr bool eq(const quarternion& z) const noexcept {
        T dx = m_x - z.m_x;
        T dy = m_y - z.m_y;
        T dz = m_z - z.m_z;
        T dd = dx*dx+dy*dy+dz*dz;
        return (sqrt(dd) < std::numeric_limits<T>::epsilon());
    }
    constexpr bool neq(const quarternion& z) const noexcept {
        T dx = m_x - z.m_x;
        T dy = m_y - z.m_y;
        T dz = m_z - z.m_z;
        T dd = dx*dx+dy*dy+dz*dz;
        return (sqrt(dd) > std::numeric_limits<T>::epsilon());
    }

    quarternion conj() const noexcept {
        return quarternion<T>(m_t,-m_x,-m_y,-m_z);
    }

    quarternion inverse() const noexcept {
        T p = m_t * m_t + m_x * m_x + m_y * m_y + m_z * m_z;
        p = T(1)/p;
        return quarternion<T>(m_t*p,-m_x*p,-m_y*p,-m_z*p);
    }

    constexpr T norm() const noexcept {
        return (m_t * m_t + m_x * m_x + m_y * m_y + m_z * m_z);
    }

    quarternion unit() const noexcept {
        T p = m_t * m_t + m_x * m_x + m_y * m_y + m_z * m_z;
        if ( p < std::numeric_limits<T>::epsilon() ) {
            return quarternion<T>();
        }
        p = T(1)/sqrt(p);
        return quarternion<T>( p*m_t, p*m_x, p*m_y, p*m_z);
    }
    
    quarternion vunit() const noexcept {
        T vn = sqrt(vnorm());
        return quarternion<T>(T(0),m_x/vn,m_y/vn,m_z/vn);
    }

    T vnorm() const noexcept {
        return m_x*m_x+m_y*m_y+m_z*m_z;
    }

    quarternion exp() const noexcept {
        T vn = m_x * m_x + m_y * m_y + m_z * m_z;
        vn = sqrt(vn);
        T rx = exp(m_t);
        T qt = rx * cos(vn);
        T vfact = sin(vn)* rx / vn;
        T qx = m_x * vfact;
        T qy = m_y * vfact;
        T qz = m_z * vfact;
        return quarternion(qt,qx,qy,qz)
    }

    quarternion log() const noexcept {
        T vn = m_x * m_x + m_y * m_y + m_z * m_z;
        T qn = sqrt(vn + m_t * m_t);
        vn = sqrt(vn);
        T vninv = acos( m_t / qn) / vn;
        T qx = m_x * vninv;
        T qy = m_y * vninv;
        T qz = m_z * vninv;
        return quarternion<T>(log(qn),qx,qy,qz);
    }

    quarternion<T> pow( const T& alpha) const noexcept {
        T vn = m_x * m_x + m_y * m_y + m_z * m_z;
        T qn = sqrt(vn + m_t * m_t);
        vn = sqrt(vn);
        T phi = atan2( m_t, vn );
        T qna = pow(qn,alpha);
        T nt = qna * cos( alpha*phi);
        T vninv = qna * sin( alpha*phi) / vn;
        T nx = m_x * vninv;
        T ny = m_y * vninv;
        T nz = m_z * vninv;
        return quarternion<T>(nt,nx,ny,nz);
    }

    std::ostream& write_to_stream(std::ostream& os) const {
        os << "(" << m_t << ", " << m_x << ", " << m_y << ", " << m_z << ")";
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
            if (ch=='(') {
                // beginning of sequence
                is >> ch;
                break;
            }
            if (isdigit(ch) || ch=='-' || ch == '+') {
                is >> m_t;
                m_x = m_y = m_z = T(0);
                return is;
            }
            // anything else is wrong
            throw std::ios::failure("istream >> quarternion error");
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
            throw std::ios::failure("istream >> quarternion error");
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
            throw std::ios::failure("istream >> quarternion error");
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
            throw std::ios::failure("istream >> quarternion error");
        }
        is >> m_z;
        while (1) {
            ch = is.peek();
            if (ch==' ') {
                // eat whitespace
                is >> ch;
                continue;
            }
            if (ch==')') {
                // end of sequence
                is >> ch;
                return is;
            }
            // anything else is wrong
            throw std::ios::failure("istream >> quarternion error");
        }
        return is;
    }

    inline T x() const noexcept {
        return m_x;
    }
    inline T y() const noexcept {
        return m_y;
    }
    inline T z() const noexcept {
        return m_z;
    }
    inline T t()  const noexcept {
        return m_t;
    }

    inline quarternion<T> qvec() const noexcept {
        return quarternion<T>(T(0),m_x,m_y,m_z);
    }

    inline T arg() const noexcept {
        T vn = m_x*m_x+m_y*m_y+m_z*m_z;
        return atan2(m_t,sqrt(vn));
    }

    inline T abs() const noexcept {
        return sqrt(norm());    
    }

    inline quarternion<T> polar(
        const T& rho, const T& phi, const quarternion& uvec)
    {
        m_t = rho * cos(phi);
        T snp = rho * sin(phi);
        m_x = snp * uvec.m_x;
        m_y = snp * uvec.m_y;
        m_z = snp * uvec.m_z;
    } 

    inline quarternion<T> polar2(
        const T& rho, const T& phi, const quarternion& uvec)
    {
        m_t = rho * cos(phi);
        T snp = rho * sin(phi);
        T vnr = sqrt(uvec.m_x * uvec.m_x + uvec.m_y * uvec.m_y + uvec.m_z * uvec.m_z);
        m_x = snp * uvec.m_x/vnr;
        m_y = snp * uvec.m_y/vnr;
        m_z = snp * uvec.m_z/vnr;
    }         
};

template < typename T >
inline quarternion<T> operator+( const quarternion<T>& q1, const quarternion<T>& q2) {
    quarternion<T> res(q1);
    return (res+=q2);
}

template < typename T >
inline quarternion<T> operator-( const quarternion<T>& q1, const quarternion<T>& q2) {
    quarternion<T> res(q1);
    return (res-=q2);
}
template < typename T >
inline quarternion<T> operator*( const quarternion<T>& q1, const quarternion<T>& q2) {
    quarternion<T> res(q1);
    return (res*=q2);
}

template < typename T >
inline quarternion<T> operator+( const quarternion<T>& q1, const T& r) {
    quarternion<T> res(q1);
    return (res+=r);
}

template < typename T >
inline quarternion<T> operator-( const quarternion<T>& q1, const T& r) {
    quarternion<T> res(q1);
    return (res-=r);
}
template < typename T >
inline quarternion<T> operator*( const quarternion<T>& q1, const T& r) {
    quarternion<T> res(q1);
    return (res*=r);
}

template < typename T >
inline quarternion<T> operator/( const quarternion<T>& q1, const T& r) {
    quarternion<T> res(q1);
    T ri = T(1)/r;
    return (res*=ri);
}

template < typename T >
inline quarternion<T> operator+( const T& r, const quarternion<T>& q1) {
    quarternion<T> res(q1);
    return (res+=r);
}

template < typename T >
inline quarternion<T> operator-( const T& r, const quarternion<T>& q1) {
    quarternion<T> res(q1);
    return (res-=r);
}
template < typename T >
inline quarternion<T> operator*( const T& r, const quarternion<T>& q1) {
    quarternion<T> res(q1);
    return (res*=r);
}

template < typename T >
inline quarternion<T> operator/( const T& r, const quarternion<T>& q1) {
    quarternion<T> res(q1.inverse());
    quarternion<T> rx(r,T(0),T(0),T(0));
    rx *= res;
    return rx;
}

// return q1 * pow(q2,-1)
template < typename T >
inline quarternion<T> rdiv(const quarternion<T>& q1, const quarternion<T>& q2)
{
    quarternion<T> qx{q2.inverse()};
    return q1 * qx;
}

// return pow(q2,-1) * q1
template < typename T >
inline quarternion<T> ldiv(const quarternion<T>& q1, const quarternion<T>& q2)
{
    quarternion<T> qx{q2.inverse()};
    return qx * q1;
}

template < typename T > inline std::ostream& operator << ( std::ostream& os,
        const quarternion<T>& q) {
    return q.write_to_stream(os);
}

template < typename T > inline std::istream& operator >> ( std::istream& is,
        quarternion<T>& q) {
    return q.read_from_stream(is);
}


template < typename T>  inline quarternion<T> exp(const quarternion<T>& q)
{
    return q.exp();
}

template < typename T>  inline quarternion<T> log(const quarternion<T>& q)
{
    return q.log();
}

template < typename T>  inline quarternion<T> pow(const quarternion<T>& q,
        const T& r)
{
    return q.pow(r);
}


template < typename T> inline quarternion<T> ihat()
{
    return quarternion(T(0),T(1),T(0),T(0));
}

template < typename T> inline quarternion<T> jhat()
{
    return quarternion(T(0),T(0),T(1),T(0));
}

template < typename T> inline quarternion<T> khat()
{
    return quarternion(T(0),T(0),T(0),T(1));
}

template < typename T> inline quarternion<T> no_hat()
{
    return quarternion(T(1),T(0),T(0),T(0));
}

template < typename T > inline bool operator==(const quarternion<T>& a,const quarternion<T>& b) noexcept
{
    return a.eq(b);
}

template < typename T > inline bool operator!=(const quarternion<T>& a,const quarternion<T>& b) noexcept
{
    return a.neq(b);
}

template < typename T > inline quarternion<T> normProduct(const quarternion<T>& x,const quarternion<T>& y) {
    quarternion<T> yc = y.conj();
    return (x * yc + yc * x)*T(0.5f);
}

template < typename T > inline quarternion<T> normDivision(const quarternion<T>& x,const quarternion<T>& y) {
    quarternion<T> yc = y.conj()/y.norm();
    return (x * yc + yc * x)*T(0.5f);
}


template < typename T > inline quarternion<T> cosh( const quarternion<T>& q) noexcept {
    return (exp(q)+exp(-q))*T(0.5f);
}

template < typename T > inline quarternion<T> sinh( const quarternion<T>& q) noexcept {
    return (exp(q)-exp(-q))*T(0.5f);
}

template < typename T > inline quarternion<T> tanh( const quarternion<T>& q) noexcept {
    quarternion<T> exp_p = exp(q);
    quarternion<T> exp_m = exp(-1*q);
    return (exp_p-exp_m)/(exp_p+exp_m);  
}

template < typename T > inline quarternion<T> trans(const quarternion<T>& q, const quarternion<T>& g)
{
    return q.inverse()*q*g;
}

}


#endif