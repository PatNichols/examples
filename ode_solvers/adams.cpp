

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>

struct adams_bashforth_stepper {
    double *p;
    double *m_derivs;
    double y;
    double m_dt;
    double m_t;
    std::size_t n_step;
    double (*derivs)(double,double);
    
    adams_bashforth_stepper(std::size_t num_steps,
        double delta_t,double y0,double (*f)(double,double)):
        p(),m_derivs(),y(y0),m_dt(delta_t),m_t(0),n_step(num_steps),derivs(f)
    {
        p = new double[n_step];
        m_derivs = new double[n_step];
            double pm[5][5];
            pm[0][0] = 1.0;
            pm[1][1] = 1.5;
            pm[1][0] = -.5;
            pm[2][2] = 23./12.0;
            pm[2][1] = -16./12.0;
            pm[2][0] = 5/12.0;
            pm[3][3] = 55.0/24.0;
            pm[3][2] = -59.0/24.0;
            pm[3][1] = 37.0/24.0;
            pm[3][0] = -9./24.0;
            pm[4][4] = 1901./720.;
            pm[4][3] = -2774./720.;
            pm[4][2] = 2616.0/720.;
            pm[4][1] = -1274/720.;
            pm[4][0] = 251./720.0;
        switch (n_step) {
        case 1:
            m_derivs[0] = derivs(m_t,y);
            p[0]=1.0;
            break;
        case 2:
            m_derivs[0] = derivs(m_t,y);
            y += m_derivs[0]*m_dt;
            m_t += m_dt;
            m_derivs[1] = derivs(m_t,y);
            p[0] = pm[1][0];
            p[1] = pm[1][1];
            break;
        case 3:
            m_derivs[0] = derivs(m_t,y);
            y += m_derivs[0]*m_dt;
            m_t += m_dt;
            m_derivs[1] = derivs(m_t,y);
            y += (m_derivs[0]*pm[1][0]  + m_derivs[1]*pm[1][1]) * m_dt;
            m_t += m_dt;
            m_derivs[2] = derivs(m_t,y);
            p[0] = pm[2][0];
            p[1] = pm[2][1];
            p[2] = pm[2][2];
            break;
        case 4:
            m_derivs[0] = derivs(m_t,y);
            y += m_derivs[0]*m_dt;
            m_t += m_dt;
            m_derivs[1] = derivs(m_t,y);
            y += (m_derivs[0]*pm[1][0]  + m_derivs[1]*pm[1][1]) * m_dt;
            m_t += m_dt;
            m_derivs[2] = derivs(m_t,y);
            y += (m_derivs[0]*pm[2][0]  + m_derivs[1]*pm[2][1] + m_derivs[2]*pm[2][2]) * m_dt;
            m_t += m_dt;
            m_derivs[3] = derivs(m_t,y);
            p[0] = pm[3][0];
            p[1] = pm[3][1];
            p[2] = pm[3][2];
            p[3] = pm[3][3];
            break;
        default:
            m_derivs[0] = derivs(m_t,y);
            y += m_derivs[0]*m_dt;
            m_t += m_dt;
            m_derivs[1] = derivs(m_t,y);
            y += (m_derivs[0]*pm[1][0]  + m_derivs[1]*pm[1][1]) * m_dt;
            m_t += m_dt;
            m_derivs[2] = derivs(m_t,y);
            y += (m_derivs[0]*pm[2][0]  + m_derivs[1]*pm[2][1] + m_derivs[2]*pm[2][2]) * m_dt;
            m_t += m_dt;
            m_derivs[3] = derivs(m_t,y);
            y += (m_derivs[0]*pm[3][0]  + m_derivs[1]*pm[3][1] + m_derivs[2]*pm[3][2]+
                m_derivs[3]*pm[3][3]) * m_dt;
            m_t += m_dt;
            m_derivs[4] = derivs(m_t,y);
            p[0] = pm[4][0];
            p[1] = pm[4][1];
            p[2] = pm[4][2];
            p[3] = pm[4][3];
            p[4] = pm[4][4];
            break;        
        }        
    }

    void step()
    {
        double der = p[0] * m_derivs[0];
        for (std::size_t i=1;i<n_step;++i) {
            der += p[i] * m_derivs[i];
            m_derivs[i-1] = m_derivs[i];
        }
        y += der*m_dt;
        m_t += m_dt;
        m_derivs[n_step-1] = derivs(m_t,y); 
    }

    
    friend std::ostream& operator<<(std::ostream& os,const adams_bashforth_stepper& s)
    {
        os << s.m_t << " " << s.y << "\n";
        return os;
    }
};

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

double derivs1(double t,double x)
{
    return t*t*t*exp(-2.*t)-2.*x;
}


int test(std::size_t p_steps)
{
    double dt = 0.001;
    std::size_t nt = 100;
    std::size_t m_steps = 1.0/dt;
    double y0 = 1.0;
    
    adams_bashforth_stepper stp(p_steps,dt,y0,&derivs1);
    std::cout << "adams stepper n = " << p_steps << "\n";
    std::cout << stp << "\n";
    std::size_t i = p_steps - 1;
    for ( ; i < m_steps; ++i) {
        stp.step();
        if ((i%nt)==0) std::cout << stp << "\n";
    }
    std::cout << stp << "\n\n";
    return 0;
}

int main()
{
    test(1);
    test(2);
    test(3);
    test(4);
    test(5);
}