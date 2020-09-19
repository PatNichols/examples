#ifndef PETLIB_RK_GEN_HPP
#define PETLIB_RK_GEN_HPP

#include <cstdlib>
#include <limits>
#include <iostream>
#include <cmath>

namespace petlib {

struct rk_stepper {
    double m_t;
    double m_dt;
    double *m_x;
    double *m_x1;
    double **m_derivs;
    double **a;
    double *b;
    double *c;
    std::size_t n_var;
    std::size_t n_order;
    void (*derivs_fun)(double,const double*,double*,std::size_t);

    rk_stepper(std::size_t order,
               std::size_t nvariables,
               double delta_t,
               double *x0,
               void (*der)(double,const double*,double*,std::size_t),
               void (*ass_cofs)(double**,double*,double*)):
        m_t(0.),m_dt(delta_t),
        m_x(),m_x1(),m_derivs(),
        a(),b(),c(),
        n_var(nvariables),
        n_order(order),
        derivs_fun(der)
    {
        m_x = new double[(n_var*2)];
        m_x1 = m_x + n_var;
        m_derivs = new double*[n_order];
        for (std::size_t k=0; k<n_order; ++k) m_derivs[k] = new double[n_var];
        std::size_t asize = n_order *( n_order - 1)/2;
        a = new double*[order-1];
        for (std::size_t k=0; k<(n_order-1); ++k) a[k]=new double[k+1];
        b = new double[n_order];
        c = new double[n_order-1];
        ass_cofs(a,b,c);
        memcpy(m_x,x0,sizeof(double)*n_var);
    }

    ~rk_stepper()
    {
        delete [] c;
        delete [] b;
        for (std::size_t k = n_order-1; k>0; ) {
            --k;
            delete  [] a[k];
        }
        delete [] a;
        for (std::size_t j=n_order; j>0;) {
            --j;
            delete [] m_derivs[j];
        }
        delete [] m_derivs;
        delete [] m_x;
    }

    void step() {
        std::size_t ia=0;
        derivs_fun(m_t,m_x,m_derivs[0],n_var);
        for (std::size_t m=1; m<n_order; ++m) {
            for (std::size_t i=0; i<n_var; ++i) {
                double der = 0.;
                for (std::size_t j=0; j<m; ++j) der+=m_derivs[j][i]*a[m-1][j];
                m_x1[i]=m_x[i]+der*m_dt;
            }
            double ts = m_t + m_dt * c[m-1];
            derivs_fun(ts,m_x1,m_derivs[m],n_var);
        }
        for (std::size_t i=0; i<n_var; ++i) {
            double der=0.0;
            for (std::size_t j=0; j<n_order; ++j) der+=m_derivs[j][i]*b[j];
            m_x[i] += der*m_dt;
        }
        m_t += m_dt;
    }

    friend std::ostream& operator<<(std::ostream& os,const rk_stepper& s) {
        os << s.m_t;
        for (std::size_t k=0; k<s.n_var; ++k)
            os << ", " << s.m_x[k];
        os << "\n";
        return os;
    }
};

struct rk4_stepper {
    double m_t;
    double m_dt;
    double *m_x;
    double *m_x1;
    double **m_derivs;
    std::size_t n_var;
    void (*derivs_fun)(double,const double*,double*,std::size_t);


    rk4_stepper(
        std::size_t nvariables,
        double delta_t,
        double *x0,
        void (*der)(double,const double*,double*,std::size_t)):
        m_t(0.),m_dt(delta_t),
        m_x(),m_x1(),m_derivs(),
        n_var(nvariables),
        derivs_fun(der)
    {
        m_x = new double[(n_var*2)];
        m_x1 = m_x + n_var;
        m_derivs = new double*[4];
        for (std::size_t k=0; k<4; ++k) m_derivs[k] = new double[n_var];
        memcpy(m_x,x0,sizeof(double)*n_var);
    }

    ~rk4_stepper()
    {
        for (std::size_t j=4; j>0;) {
            --j;
            delete [] m_derivs[j];
        }
        delete [] m_derivs;
        delete [] m_x;
    }

    void step() {
        std::size_t ia=0;
        derivs_fun(m_t,m_x,m_derivs[0],n_var);
        double step = 0.5 * m_dt;
        for (std::size_t i=0; i<n_var; ++i) {
            m_x1[i] = m_x[i] + step * m_derivs[0][i];
        }
        double t1 = m_t + 0.5 * m_dt;
        derivs_fun(t1,m_x1,m_derivs[1],n_var);
        for (std::size_t i=0; i<n_var; ++i) {
            m_x1[i] = m_x[i] + step * m_derivs[1][i];
        }
        derivs_fun(t1,m_x1,m_derivs[2],n_var);
        for (std::size_t i=0; i<n_var; ++i) {
            m_x1[i] = m_x[i] + m_dt * m_derivs[2][i];
        }
        t1 = m_t + m_dt;
        derivs_fun(t1,m_x1,m_derivs[3],n_var);
        step = m_dt/6.0;
        for (std::size_t i=0; i<n_var; ++i) {
            m_x[i] += step * (m_derivs[0][i]+2.*m_derivs[1][i]+2.*m_derivs[2][i]+m_derivs[3][i]);
        }
        m_t += m_dt;
    }

    friend std::ostream& operator<<(std::ostream& os,const rk4_stepper& s) {
        os << s.m_t;
        for (std::size_t k=0; k<s.n_var; ++k)
            os << ", " << s.m_x[k];
        os << "\n";
        return os;
    }
};

void assign_rk4_mod(double **a,double *b,double *c)
{
    b[0] = 1./8.;
    b[1]=  3./8.;
    b[2]= 3./8.;
    b[3] = 1./8.;

    a[0][0] = 1./3.;
    a[1][0] = -1./3.;
    a[1][1] = 1.0;

    a[2][0] = 1.0;
    a[2][1] = -1.0;
    a[2][2] = 1.0;

    c[0] = 1./3.;
    c[1] = 2./3.;
    c[2] = 1.0;
}

void assign_rk4(double **a,double *b,double *c)
{
    b[0] = 1./6.;
    b[1]=  1./3.;
    b[2]= 1./3.;
    b[3] = 1./6.;

    a[0][0] = 0.5;

    a[1][0] = 0.0;
    a[1][1] = 0.5;

    a[2][0] = 0.0;
    a[2][1] = 0.0;
    a[2][2] = 1.0;

    c[0] = 0.5;
    c[1] = 0.5;
    c[2] = 1.0;
}

void assign_heun(double **a,double *b,double *c)
{
    b[0] = 0.5;
    b[1]=  0.5;
    a[0][0] = 1.0;
    c[0] = 1.0;
}
void assign_midpoint(double **a,double *b,double *c)
{
    b[0] = 0.0;
    b[1]=  1.0;
    a[0][0] = 0.5;
    c[0] = 0.5;
}
void assign_ralston(double **a,double *b,double *c)
{
    b[0] = 0.25;
    b[1]=  0.75;
    a[0][0] = 2.0/3.0;
    c[0] = 2.0/3.0;
}


struct adaptive_rk_stepper {
    double m_t;
    double m_dt;
    double *m_x0;
    double *m_x;
    double *m_x1;
    double **m_derivs;
    double **a;
    double *b;
    double *berr;
    double *c;
    std::size_t n_var;
    std::size_t n_order;
    void (*derivs_fun)(double,const double*,double*,std::size_t);
    double err_tol;
    double min_step;
    double abs_min_dt;
    double max_step;
    
    adaptive_rk_stepper(std::size_t order,
                        std::size_t nvariables,
                        double delta_t,
                        double *x0,
                        void (*der)(double,const double*,double*,std::size_t),
                        void (*ass_cofs)(double**,double*,double*,double*)):
        m_t(0.),m_dt(delta_t),
        m_x0(),m_x(),m_x1(),
        m_derivs(),
        a(), b(), c(),
        n_var(nvariables),
        n_order(order),
        derivs_fun(der),
        err_tol(),min_step(),abs_min_dt(1e300),max_step(delta_t)
    {
        m_x0 = new double[n_var*2];
        m_x = m_x0;
        m_x1 = m_x0 + n_var;
        m_derivs = new double*[n_order];
        for (std::size_t k=0; k<n_order; ++k) m_derivs[k] = new double[n_var];
        std::size_t nom1 = n_order-1;
        a = new double*[nom1];
        for (std::size_t k=0; k<nom1; ++k)
            a[k] = new double[k+1];
        b = new double[n_order];
        berr = new double[n_order];
        c = new double[nom1];
        ass_cofs(a,b,berr,c);
        for (std::size_t k=0; k<n_var; ++k) m_x[k]=x0[k];
        err_tol = std::numeric_limits<double>::epsilon()*100.;
        min_step = std::numeric_limits<double>::epsilon()*2.;
    }

    ~adaptive_rk_stepper()
    {
        delete [] c;
        delete [] berr;
        delete [] b;
        for (std::size_t j=(n_order-1); j>0;) {
            --j;
            delete [] a[j];
        }
        delete [] a;
        for (std::size_t j=n_order; j>0;) {
            --j;
            delete [] m_derivs[j];
        }
        delete [] m_derivs;
        delete [] m_x0;
    }

    void set_max_step(double new_step) {
        max_step = new_step;
    }

    void set_error_tolerance(double new_tol) {
        err_tol = new_tol;
    }
    
    void set_min_step(double new_step) {
        min_step = new_step;
    }

    void step() {
        abs_min_dt = fmin(abs_min_dt,m_dt);
        std::size_t max_try = 200;
        double err_max;
        for (std::size_t ktry=0; ktry<max_try; ++ktry) {
            derivs_fun(m_t,m_x,m_derivs[0],n_var);
            for (std::size_t m=1; m<n_order; ++m) {
                for (std::size_t i=0; i<n_var; ++i) {
                    double der = 0.;
                    for (std::size_t j=0; j<m; ++j) {
                        der+=m_derivs[j][i]*a[m-1][j];
                    }
                    m_x1[i]=m_x[i]+der*m_dt;
                }
                double ts = m_t + m_dt * c[m-1];
                derivs_fun(ts,m_x1,m_derivs[m],n_var);
            }
            err_max = 0.0;
            for (std::size_t i=0; i<n_var; ++i) {
                double der=0.0;
                double err=0.0;
                for (std::size_t j=0; j<n_order; ++j) {
                    der+=m_derivs[j][i]*b[j];
                    err+=m_derivs[j][i]*berr[j];
                }
                m_x1[i] = m_x[i] + der*m_dt;
                err_max = fmax(err_max,err);
            }
            err_max *= m_dt;
            double f = err_tol/(err_max+1.e-15);
            if (err_max <= err_tol) {
                double *tmp = m_x;
                m_x = m_x1;
                m_x1 = tmp;
                m_t += m_dt;
                m_dt *= f;
                m_dt = fmin(m_dt,max_step);
                return;
            } else {
                m_dt *= f;
                m_dt = fmax(m_dt,min_step);
            }
        }
        std::cout << "t = " << m_t << " dt = " << m_dt << "\n";
        std::cout << "err =  " << err_max << " err_tol = " << err_tol << "\n";
        std::cout << "max tries exceeded\n";
        exit(EXIT_FAILURE);
    }

    friend std::ostream& operator<<(std::ostream& os,const adaptive_rk_stepper& s) {
        os << s.m_t;
        for (std::size_t k=0; k<s.n_var; ++k)
            os << ", " << s.m_x[k];
        os << "\n";
        return os;
    }
};

struct rkf_stepper {
    double m_t;
    double m_dt;
    double *m_x0;
    double *m_x;
    double *m_x1;
    double **m_derivs;
    double err_tol;
    double min_step;
    double b[6];
    double berr[6];
    double a[5][5];
    double c[5];
    double abs_min_dt;
    double max_step;
    std::size_t n_var;
    void (*derivs_fun)(double,const double*,double*,std::size_t);


    rkf_stepper(
        std::size_t nvariables,
        double delta_t,
        double *x0,
        void (*der)(double,const double*,double*,std::size_t)):
        m_t(0.),m_dt(delta_t),
        m_x0(),m_x(),m_x1(),m_derivs(),
        abs_min_dt(1e300),max_step(delta_t),
        n_var(nvariables),
        derivs_fun(der)
    {
        m_x0 = new double[(n_var*2)];
        m_x = m_x0;
        m_x1 = m_x0 + n_var;
        m_derivs = new double*[6];
        for (std::size_t k=0; k<6; ++k) m_derivs[k] = new double[n_var];
        err_tol = std::numeric_limits<double>::epsilon()*100.;
        min_step = std::numeric_limits<double>::epsilon()*2.;
        for (std::size_t k=0; k<n_var; ++k) m_x[k]=x0[k];
        b[0] = 16./135.;
        b[1] = 0.0;
        b[2] = 6656./12825.;
        b[3] = 28561./56430.;
        b[4] = -9./50.0;
        b[5] = 2./55.0;
        berr[0] =b[0]-25.0/216.0;
        berr[1]= 0.0;
        berr[2] =b[2]-1408./2565.;
        berr[3] =b[3]-2197.0/4104.;
        berr[4] =b[4]+0.2;
        berr[5] =b[5];

        a[0][0] = 0.25;

        a[1][0] = 3./32.;
        a[1][1] = 9./32.;

        a[2][0] = 1932./2197.0;
        a[2][1] = -7200./2197.0;
        a[2][2] = 7296./2197.0;

        a[3][0] =  439./216.;
        a[3][1] = -8.0;
        a[3][2] = 3680./513.;
        a[3][3] = -845.0/4104.;

        a[4][0] = -8./27.0;
        a[4][1] = 2.0;
        a[4][2] = -3544./2565.;
        a[4][3] = 1859./4104.;
        a[4][4] =  -11./40.;

        c[0] = 0.25;
        c[1] = 3./8.;
        c[2] = 12./13.;
        c[3] = 1.0;
        c[4] = 0.5;
    }

    ~rkf_stepper()
    {
        for (std::size_t j=6; j>0;) {
            --j;
            delete [] m_derivs[j];
        }
        delete [] m_derivs;
        delete [] m_x0;
    }

    void set_max_step(double new_step) { max_step = new_step;}
    
    void set_error_tolerance(double new_tol) {
        err_tol = new_tol;
    }
    void set_min_step(double new_step) {
        min_step = new_step;
    }

    void step() {
        abs_min_dt = fmin(abs_min_dt,m_dt);
        std::size_t max_try = 200;
        double err_max;
        for (std::size_t ktry=0; ktry<max_try; ++ktry) {
            derivs_fun(m_t,m_x,m_derivs[0],n_var);
            for (std::size_t m=1; m<6; ++m) {
                for (std::size_t i=0; i<n_var; ++i) {
                    double der = 0.;
                    for (std::size_t j=0; j<m; ++j) {
                        der+=m_derivs[j][i]*a[m-1][j];
                    }
                    m_x1[i]=m_x[i]+der*m_dt;
                }
                double ts = m_t + m_dt * c[m-1];
                derivs_fun(ts,m_x1,m_derivs[m],n_var);
            }
            err_max = 0.0;
            for (std::size_t i=0; i<n_var; ++i) {
                double der=0.0;
                double err=0.0;
                for (std::size_t j=0; j<6; ++j) {
                    der+=m_derivs[j][i]*b[j];
                    err+=m_derivs[j][i]*berr[j];
                }
                m_x1[i] = m_x[i] + der*m_dt;
                err_max = fmax(err_max,err);
            }
            err_max *= m_dt;
            double f = err_tol/(err_max+1.0e-15);
            if (err_max <= err_tol) {
                double *tmp = m_x;
                m_x = m_x1;
                m_x1 = tmp;
                m_t += m_dt;
                m_dt *= f;
                m_dt = fmin(m_dt,max_step);
                return;
            } else {
                m_dt *= f;
                m_dt = fmax(m_dt,min_step);
            }
        }
        std::cout << "t = " << m_t << " dt = " << m_dt << "\n";
        std::cout << "err =  " << err_max << " err_tol = " << err_tol << "\n";
        std::cout << "max tries exceeded\n";
        exit(EXIT_FAILURE);
    }

    friend std::ostream& operator<<(std::ostream& os,const rkf_stepper& s) {
        os << s.m_t;
        for (std::size_t k=0; k<s.n_var; ++k)
            os << ", " << s.m_x[k];
        os << "\n";
        return os;
    }
};





void assign_rkf(double **a,double *b,double *berr,double *c)
{
    b[0] = 16./135.;
    b[1] = 0.0;
    b[2] = 6656./12825.;
    b[3] = 28561./56430.;
    b[4] = -9./50.0;
    b[5] = 2./55.0;
    berr[0] =b[0]-25.0/216.0;
    berr[1]= 0.0;
    berr[2] =b[2]-1408./2565.;
    berr[3] =b[3]-2197.0/4104.;
    berr[4] =b[4]+0.2;
    berr[5] =b[5];

    a[0][0] = 0.25;

    a[1][0] = 3./32.;
    a[1][1] = 9./32.;

    a[2][0] = 1932./2197.0;
    a[2][1] = -7200./2197.0;
    a[2][2] = 7296./2197.0;

    a[3][0] =  439./216.;
    a[3][1] = -8.0;
    a[3][2] = 3680./513.;
    a[3][3] = -845.0/4104.;

    a[4][0] = -8./27.0;
    a[4][1] = 2.0;
    a[4][2] = -3544./2565.;
    a[4][3] = 1859./4104.;
    a[4][4] =  -11./40.;

    c[0] = 0.25;
    c[1] = 3./8.;
    c[2] = 12./13.;
    c[3] = 1.0;
    c[4] = 0.5;
}

void assign_heun_euler(double **a,double *b,double *berr,double *c)
{
    b[0]= -0.5;
    b[1]= 0.5;
    berr[0]=b[0]-1.0;
    berr[1]=b[1];
    a[0][0] = 1.0;
    c[0] = 1.0;
}

void assign_bogacki_shampine(double **a,double *b,double *berr,double *c)
{
    b[0] = 2.0/9.;
    b[1] = 1./3.;
    b[2] = 4./9.;
    b[3] = 0.0;
    berr[0] =b[0]-7./24.;
    berr[1] =b[1]- 0.25 ;
    berr[2] =b[2]- 1./3.;
    berr[3] =b[3]- 0.125;
    a[0][0] = 0.5;
    a[1][0] = 0.0;
    a[1][1] = 0.75;
    a[2][0] = 2./9.;
    a[2][1] = 1./3.;
    a[2][2] = 4./9.;
    c[0] = 0.5;
    c[1] = 0.75;
    c[2] = 1.0;
}

}
#endif

struct stopwatch {
    double acc;
    struct timespec ts;
    struct timespec tf;

    stopwatch():acc(0.),ts(),tf() {}
    void start() {
        clock_gettime(CLOCK_MONOTONIC,&ts);
    };
    void stop()
    {
        clock_gettime(CLOCK_MONOTONIC,&tf);
        acc += tf.tv_sec - ts.tv_sec + 1.e-9*(tf.tv_nsec - ts.tv_nsec);
    }
    double elapsed_time() {
        return acc;
    }
    void clear() {
        acc=0.0;
    }
};

double drand() {
    double x = rand();
    x /= RAND_MAX;
    x = x + x - 1.0;
    return x;
}


void derivs1(double t,const double *x,double *der,std::size_t n)
{
    der[0] = t*t*t*exp(-2.*t)-2*x[0];
}


void test1()
{
    std::size_t i;
    double y0 = 1.0;
    double dx = 0.1;
    petlib::rk4_stepper stepper(1,dx,&y0,&derivs1);
    std::size_t nsteps = 1.0/dx;
    
    std::cout << "rk4 stepper\n";
    std::cout << "dx = " << dx << "\n";
    std::cout << stepper;
    for (i=0; i<nsteps; ++i) {
        stepper.step();
        std::cout << stepper;
    }
    std::cout <<"\n";


    std::cout << "generic stepper rk4\n";
    std::cout << "dx = " << dx << "\n";
    petlib::rk_stepper stepper2(4,1,dx,&y0,&derivs1,&petlib::assign_rk4);
    std::cout << stepper2;
    for (i=0; i<nsteps; ++i) {
        stepper2.step();
        std::cout << stepper2;
    }
    std::cout <<"\n";
    
    
    petlib::rk_stepper stepper2m(4,1,dx,&y0,&derivs1,&petlib::assign_rk4_mod);
    std::cout << "generic stepper mod_rk4\n";
    std::cout << "dx = " << dx << "\n";    
    std::cout << stepper2m;
    for (i=0; i<nsteps; ++i) {
        stepper2m.step();
        std::cout << stepper2m;
    }
    std::cout <<"\n";
    dx= 0.05;

    std::cout <<"\n";
    nsteps = 1.0/dx;
    std::cout << "generic stepper rk4\n";
    std::cout << "dx = " << dx << "\n";    
    petlib::rk_stepper stepper3(4,1,dx,&y0,&derivs1,&petlib::assign_rk4);
    std::cout << stepper3;
    for (i=0; i<nsteps; ++i) {
        stepper3.step();
        if ((i%2)) std::cout << stepper3;
    }
    std::cout <<"\n";
    petlib::rk_stepper stepper4(4,1,dx,&y0,&derivs1,&petlib::assign_rk4_mod);
    std::cout << "generic stepper rk4_mod\n";
    std::cout << "dx = " << dx << "\n";
    std::cout << stepper4;
    for (i=0; i<nsteps; ++i) {
        stepper4.step();
        if ((i%2))std::cout << stepper4;
    }
    std::cout <<"\n";
    petlib::rk_stepper stepper5(2,1,dx,&y0,&derivs1,&petlib::assign_midpoint);
    std::cout << "generic stepper midpoint\n";
    std::cout << "dx = " << dx << "\n";
    std::cout << stepper5;
    for (i=0; i<nsteps; ++i) {
        stepper5.step();
        if ((i%2))std::cout << stepper5;
    }
    dx = 0.0005;
    nsteps = 1.0/dx;
    std::cout << "\n";
    std::cout << "dx =  " << dx << "\n";
    petlib::rk_stepper stepper6(4,1,dx,&y0,&derivs1,&petlib::assign_rk4_mod);
    std::cout << "generic stepper rk4_mod\n";
    std::cout << "dx = " << dx << "\n";
    std::cout << stepper6;
    double x_print = 0.1;
    for (i=0; i<nsteps; ++i) {
        stepper6.step();
        if (stepper6.m_t >= x_print) {
            std::cout << stepper6;
            x_print += 0.1;
        }
    }
    std::cout << stepper6 << "\n";
    std::cout <<"\n";
    dx = 0.0005;
    nsteps = 1.0/dx;
    std::cout << "\n";
    std::cout << "dx =  " << dx << "\n";
    petlib::rk_stepper stepper_h(4,1,dx,&y0,&derivs1,&petlib::assign_heun);
    std::cout << "generic stepper heun\n";
    std::cout << "dx = " << dx << "\n";
    std::cout << stepper_h;
    x_print = 0.1;
    for (i=0; i<nsteps; ++i) {
        stepper_h.step();
        if (stepper_h.m_t >= x_print) {
            std::cout << stepper_h;
            x_print += 0.1;
        }
    }
    std::cout << stepper_h << "\n";
    std::cout <<"\n";
}

void test2()
{
    std::cout << "test2 adaptive\n";
    std::size_t i;
    double etol = 1.e-9;
    double min_dt = 1.e-12;
    double max_dt = 1.e-4;
    double y0 = 1.0;
    double dx = 0.05;
    double x_print = 0.1;

    petlib::adaptive_rk_stepper a_stepper(6,1,dx,&y0,&derivs1,&petlib::assign_rkf);
    a_stepper.set_min_step(min_dt);
    a_stepper.set_error_tolerance(etol);
    a_stepper.set_max_step(max_dt);
    std::cout << "generic adaptive stepper rkf\n";
    std::cout << "dx0 = " << dx << "\n"; 
    std::cout << a_stepper;
    std::size_t istep = 0;
    x_print = 0.1;
    while (a_stepper.m_t <= 1.00) {
        a_stepper.step();
        if (a_stepper.m_t >= x_print) {
            std::cout << a_stepper;
            x_print += 0.1;
        }
        ++istep;
    }
    std::cout << a_stepper;
    std::cout << "dt = " << a_stepper.m_dt << "\n";
    std::cout << "nsteps = " << istep << "\n";
    std::cout << "abs min dt = " << a_stepper.abs_min_dt << "\n";
    std::cout << "\n";

    istep = 0;
    petlib::rkf_stepper stepper(1,dx,&y0,&derivs1);
    stepper.set_min_step(min_dt);
    stepper.set_error_tolerance(etol);
    stepper.set_max_step(max_dt);
    std::cout << "rkf stepper\n";
    std::cout << "dx0 = " << dx << "\n"; 
    std::cout << stepper;
    x_print = 0.1;
    while (stepper.m_t <= 1.00) {
        stepper.step();
        if (stepper.m_t > x_print) {
            std::cout << stepper;
            x_print += 0.1;
        }
        ++istep;
    }
    std::cout << stepper;
    std::cout << "dt = " << stepper.m_dt << "\n";
    std::cout << "nsteps = " << istep << "\n";
    std::cout << "abs min dt = " << stepper.abs_min_dt << "\n";
    std::cout << "\n";

    istep = 0;
    petlib::adaptive_rk_stepper bstepper(4,1,dx,&y0,&derivs1,&petlib::assign_bogacki_shampine);
    bstepper.set_min_step(min_dt);
    bstepper.set_error_tolerance(etol);
    bstepper.set_max_step(max_dt);
    std::cout << "bogacki-shampine stepper\n";
    std::cout << "dx0 = " << dx << "\n"; 
    std::cout << bstepper;
    x_print = 0.1;
    while (bstepper.m_t <= 1.00) {
        bstepper.step();
        if (bstepper.m_t > x_print) {
            std::cout << bstepper;
            x_print += 0.1;
        }
        ++istep;
    }
    std::cout << bstepper;
    std::cout << "dt = " << stepper.m_dt << "\n";
    std::cout << "nsteps = " << istep << "\n";
    std::cout << "abs min dt = " << stepper.abs_min_dt << "\n";
    std::cout << "\n";

}

void test4()
{
    stopwatch timer;
    timer.clear();
    std::cout << "test2 adaptive\n";
    std::size_t i;
    double etol = 1.e-9;
    double min_dt = 1.e-12;
    double max_dt = 1.e-5;
    double y0 = 1.0;
    double dx = 0.05;
    double x_print = 0.1;

    timer.start();
    petlib::adaptive_rk_stepper a_stepper(6,1,dx,&y0,&derivs1,&petlib::assign_rkf);
    a_stepper.set_min_step(min_dt);
    a_stepper.set_error_tolerance(etol);
    a_stepper.set_max_step(max_dt);
    std::cout << a_stepper;
    while (a_stepper.m_t <= 1.00) {
        a_stepper.step();
    }
    timer.stop();
    std::cout << "adaptive stepper generic rkf\n";    
    std::cout << a_stepper;
    std::cout << "elapsed time = " << timer.elapsed_time() << "\n";
    std::cout << "dt = " << a_stepper.m_dt << "\n";
    std::cout << "abs min dt = " << a_stepper.abs_min_dt << "\n";
    std::cout << "\n";

    timer.clear();
    timer.start();
    petlib::rkf_stepper stepper(1,dx,&y0,&derivs1);
    stepper.set_min_step(min_dt);
    stepper.set_error_tolerance(etol);
    stepper.set_max_step(max_dt);
    std::cout << stepper;
    while (stepper.m_t <= 1.00) {
        stepper.step();
    }
    timer.stop();
    std::cout << "adaptive stepper rkf\n";    
    std::cout << stepper;
    std::cout << "elapsed time = " << timer.elapsed_time() << "\n";
    std::cout << "dt = " << stepper.m_dt << "\n";
    std::cout << "abs min dt = " << stepper.abs_min_dt << "\n";
    std::cout << "\n";

    timer.clear();
    timer.start();
    petlib::adaptive_rk_stepper stepper_b(4,1,dx,&y0,&derivs1,&petlib::assign_bogacki_shampine);
    stepper_b.set_min_step(min_dt);
    stepper_b.set_error_tolerance(etol);
    stepper_b.set_max_step(max_dt);
    std::cout << stepper_b;
    while (stepper_b.m_t <= 1.00) {
        stepper_b.step();
    }
    timer.stop();
    std::cout << "adaptive stepper b-s\n";    
    std::cout << stepper_b;
    std::cout << "elapsed time = " << timer.elapsed_time() << "\n";
    std::cout << "dt = " << stepper_b.m_dt << "\n";
    std::cout << "abs min dt = " << stepper_b.abs_min_dt << "\n";
    std::cout << "\n";
}


void test3()
{
    double y0 = 1.000000;
    double dx = 0.000001;
    std::size_t nsteps = 1.0/dx;
    double etol = 1.e-7;
    double min_dt = 1.e-9;

    stopwatch timer;

    petlib::rk_stepper stepper(4,1,dx,&y0,&derivs1,&petlib::assign_rk4);
    petlib::rk_stepper stepper_m(4,1,dx,&y0,&derivs1,&petlib::assign_rk4_mod);
    petlib::rk4_stepper stepper4(1,dx,&y0,&derivs1);
    petlib::rk_stepper stepper_h(2,1,dx,&y0,&derivs1,&petlib::assign_heun);
    timer.start();
    for (std::size_t k=0; k<nsteps; ++k) {
        stepper.step();
    }
    timer.stop();
    std::cout << stepper << "\n";
    std::cout << "rk rk4      elapsed_time = " << timer.elapsed_time() << "\n";
    timer.clear();
    timer.start();
    for (std::size_t k=0; k<nsteps; ++k) {
        stepper_m.step();
    }
    timer.stop();
    std::cout << stepper_m << "\n";
    std::cout << "rk rk4_mod  elapsed_time = " << timer.elapsed_time() << "\n";
    timer.clear();
    timer.start();
    for (std::size_t k=0; k<nsteps; ++k) {
        stepper4.step();
    }
    timer.stop();
    std::cout << stepper4 << "\n";
    std::cout << "rk4         elapsed_time = " << timer.elapsed_time() << "\n";
    timer.clear();
    timer.start();
    for (std::size_t k=0; k<nsteps; ++k) {
        stepper_h.step();
    }
    timer.stop();
    std::cout << stepper_h << "\n";
    std::cout << "rk2         elapsed_time = " << timer.elapsed_time() << "\n";
}

int main()
{
    test1();
    test2();
    test3();
    test4();
}






