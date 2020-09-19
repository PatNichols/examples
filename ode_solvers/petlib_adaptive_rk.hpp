#ifndef PETLIB_adaptive_rk_HPP
#define PETLIB_adaptive_rk_HPP

#include <cstdlib>
#include <limits>
#include <iostream>
#include <cmath>

namespace petlib {

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

	adaptive_rk_stepper(std::size_t order,
		std::size_t nvariables,
		double delta_t,
		double *x0,
		void (*der)(double,const double*,double*,std::size_t),
		void (*ass_cofs)(double*,double*,double*):
			m_t(0.),m_dt(delta_t),
			m_x0(),m_x(),m_x1(),m_derivs(),
			a(),b(),c(),
			n_var(nvariables),
			n_order(order),
			derivs_fun(der),
			err_tol(),min_step()
	{
		m_x0 = new double[n_var*2];
		m_x = m_x0;
		m_x1 = m_x0 + n_var;
		m_derivs = new double*[n_order];
		for (std::size_t k=0;k<n_order;++k) m_derivs[k] = new double[n_var];
		std::size_t asize = n_order *( n_order - 1)/2;
		a = new double*[n_order-1];
		for (std::size_t k=0;k<(n_order-1);++k)
			a[k] = new double[k+1];
		b = new double[n_order];
		berr = new double[n_order];
		c = new double[n_order-1];
		ass_cofs(a,b,berr,c);
		for (std::size_t k=0;k<n_var;++k) m_x[k]=x0[k];
		err_tol = std::numeric_limits<double>::epsilon()*100.;
		min_step = std::numeric_limits<double>::epsilon()*2.;
	}

	~adaptive_rk_stepper()
	{
		delete [] c;
		delete [] berr;
		delete [] b;
		for (std::size_t j=(n_order-1);j>0;) {
			--j; delete [] a[j];
		}
		delete [] a;
		for (std::size_t j=n_order;j>0;) {
			--j; delete [] m_derivs[j];
		}
		delete [] m_derivs;
		delete [] m_x0;
	}

	void set_error_tolerance(double new_tol) { err_tol = new_tol;}
	void set_min_step(double new_step) { min_step = new_step;} 

	void step() {
		 double err_max;
		 while (1) { 
			std::size_t ia=0;
			derivs_fun(m_t,m_x,m_derivs[0],n_var);
			for (std::size_t m=1;m<n_order;++m) {
				for (std::size_t i=0;i<n_var;++i) {
					double der = 0.;
					for (std::size_t j=0;j<m;++j) der+=m_derivs[j][i]*a[m-1][j];
					m_x1[i]=m_x[i]+der*m_dt;
				}
				double ts = m_t + m_dt * c[m-1];
				derivs_fun(ts,m_x1,m_derivs[m],n_var)
			}
			for (std::size_t i=0;i<n_var;++i) {
				double der=0.0;
				double err=0.0;
				for (std::size_t j=0;j<n_order;++j) {
					der+=m_derivs[j][i]*b[j];
					err+=m_derivs[j][i]*berr[j];
				}
				m_x1[i] = m_x[i] + (der+err)*m_dt;
				err_max = fmax(err_max,err);
			}
			double f = err_tol/err_max;
			if (err_max <= err_tol) {
			    double *t1 = m_x;
			    m_x = m_x1;
			    m_x1 = t1;	
			    m_t += m_dt;
			    m_dt *= f;
			    break;
			}else{
				double fm_dt = m_dt * f;
				if (fm_dt<min_step) {
					std::cerr << "warning!!!!! err is above tolerance but time step cannot be reduced further\n";
					std::cerr << "max error = " << err_max << " error tolerance " << err_tolerance << "\n";
					std::cerr << "new time step = " << (fm_dt) << "\n";
					double *tmp = m_x;
					m_x = m_x1;
					m_x1 = tmp;
					m_t *= m_dt;
					m_dt = min_step;
					break;
				}else{
					m_dt = fm_dt;
				}			
			}
		}
    }

    friend std::ostream& operator<<(std::ostream& os,const adaptive_rk_stepper& s) {
		os << s.m_t;
		for (std::size_t k=0;k<s.n_var;++k)
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
	std::size_t n_var;
	void (*derivs_fun)(double,const double*,double*,std::size_t);
	

	rkf_stepper(
		std::size_t nvariables,
		double delta_t,
		double *x0,
		void (*der)(double,const double*,double*,std::size_t)):
			m_t(0.),m_dt(delta_t),
			m_x0(),m_x(),m_x1(),m_derivs(),
			n_var(nvariables),
			derivs_fun(der)
	{
		m_x0 = new double[(n_var*2)];
		m_x = m_x0;
		m_x1 = m_x0 + n_var;
		m_derivs = new double*[5];
		for (std::size_t k=0;k<5;++k) m_derivs[k] = new double[n_var];
		err_tol = std::numeric_limits<double>::epsilon()*100.;
		min_step = std::numeric_limits<double>::epsilon()*2.;
	}

	~rkf_stepper()
	{
		for (std::size_t j=5;j>0;) {
			--j; delete [] m_derivs[j];
		}
		delete [] m_derivs;
		delete [] m_x0;
	}

	void step() {
	
		std::size_t max_trys = 100ULL;
		
		double b1 = 25.0/216.0;
		double b2 = 0.0;
		double b3 = 1408./2565.;
		double b4 = 2197.0/4104.;
		double b5 = -0.2;
		double b6 = 0.0;

		double berr1 = 16./135.-b1;
		double berr2 = 0.0;
		double berr3 = 6656./12825. - b3;
		double berr4 = 28561./56430. - b4;
		double berr5 = -9./50.0 + 0.2;
		double berr6 = 2./55.0;

		double bx1 = 16./135.;
		double bx2= 0.0;
		double bx3 = 6656./12825.;
		double bx4 = 28561./56430.;
		double bx5 = -9./50.0;
		double bx6 = 2./55.0;

		double a20 = 1932./2197.0;
		double a21 = -7200./2197.0;
		double a22 = 7296./2197.0;

		double a30 =  439./216.;
		double a31 = -8.0;
		double a32 = 3680./513.
		double a33 = -845.0/4104.;

		double a40 = -8./27.0;
		double a41 = 2.0;
		double a42 = -3544./2565.;
		double a43 = 1859./4104.;
		double a44 =  -11./40.; 

		double err_max = 0.0;
		for (std::size_t kx=0;kx<max_trys;++kx) {
			derivs_fun(m_t,m_x,m_derivs[0],n_var);
			double step = m_dt*0.25;
			for (std::size_t i=0;i<n_var;++i) {
				m_x1[i] = m_x[i] + step * m_derivs[0][i];
			}
			double t1 = m_t + step;
			derivs_fun(t1,m_x1,derivs[1],nvar);
			step = m_dt / 32.0;
			for (std::size_t i=0;i<n_var;++i) {
				m_x1[i] = m_x[i] + 
					step * (9.*m_derivs[1][i] + 
						3.0* m_derivs[0][i]);
			}
			t1 = m_t + 3.0*m_dt/8.0;
			derivs_fun(t1,m_x1,derivs[2],nvar);
			for (std::size_t i=0;i<n_var;++i) {
				m_x1[i] = m_x[i] + 
				m_dt * (a22*m_derivs[2][i]+a21*m_derivs[1][i]+
					a20*m_derivs[0][i]);
			}		
			t1 = m_t + m_dt*12.0/13.0;
			derivs_fun(t1,m_x1,m_derivs[3],nvar);
			t1 = m_t + 3.0*m_dt/8.0;
			derivs_fun(t1,m_x1,derivs[2],nvar);
			for (std::size_t i=0;i<n_var;++i) {
				m_x1[i] = m_x[i] + 
				m_dt * (a33*m_derivs[3][i]+a32*m_derivs[2][i]+
					a31*m_derivs[1][i]+a30*m_derivs[0][i]);
			}		
			t1 = m_t + m_dt;
			derivs_fun(t1,m_x1,m_derivs[4],nvar);
			t1 = m_t + 3.0*m_dt/8.0;
			derivs_fun(t1,m_x1,derivs[2],nvar);
			for (std::size_t i=0;i<n_var;++i) {
				m_x1[i] = m_x[i] + 
				m_dt * (a44*m_derivs[4][i]+a43*m_derivs[3][i]+
					a42*m_derivs[2][i]+a41*m_derivs[1][i]+
					a40*m_derivs[0][i]);
			}		
			t1 = m_t + m_dt*0.5;
			derivs_fun(t1,m_x1,m_derivs[5],nvar);			
			double err_max = 0.0;
			for (std::size_t i=0;i<n_var;++i) {
				m_x1[i] = m_x[i] + m_dt * (
					bx1*m_derivs[0][i]+
					bx2*m_derivs[1][i]+
					bx3*m_derivs[2][i]+
					bx4*m_derivs[3][i]+
					bx5*m_derivs[4][i]+
					bx6*m_derivs[5][i]);
			}
			for (std::size_t i=0;i<n_var;++i) {
				double err =
					berr1*m_derivs[0][i]+
					berr2*m_derivs[1][i]+
					berr3*m_derivs[2][i]+
					berr4*m_derivs[3][i]+
					berr5*m_derivs[4][i]+
					berr6*m_derivs[5][i]);
				err_max = fmax(err_max,err);
			}
			err_max *= m_dt;
			double f = err_tol/err_max;
			if (f <= 1.0) {
				double *tmp = m_x;
				m_x = m_x1;
				m_x1 = tmp;
				m_t += m_dt;
				m_dt *=f;
				return;	
			}else{
				m_dt *= f;
				m_dt = fmax(m_dt,min_step);
			}
		}
		std::cerr << " dt  = " << m_dt < " min step = " << min_step << "\n";
		std::cerr << " err = " << err_max << " err tolerance = " << err_tol << "\n";
		std::cerr << " exiting now ";
		std::cerr << "max tries exceeded!\n"
		exit(EXIT_FAILURE);
	}

	friend std::ostream& operator<<(std::ostream& os,const rkf_stepper& s) {
		os << s.m_t;
		for (std::size_t k=0;k<s.n_var;++k)
			os << ", " << s.m_x[k];
		os << "\n";
		return os;
	}
};


void assign_rkf(double *a,double *b,double *berr,double *c)
{
		b[0] = 25.0/216.0;
		b[1]= 0.0;
		b[2] = 1408./2565.;
		b[3] = 2197.0/4104.;
		b[4] = -0.2;
		b[5] = 0.0;

		berr[0] = 16./135.-b[0];
		berr[1] = 0.0;
		berr[2] = 6656./12825. - b[2];
		berr[3] = 28561./56430. - b[3];
		berr[4] = -9./50.0 + 0.2;
		berr[5] = 2./55.0;

		a[0] = 0.25;
		a[1] = 3./32.;
		a[2] = 9./32.;
 
		a[3] = 1932./2197.0;
		a[4] = -7200./2197.0;
		a[5] = 7296./2197.0;

		a[6] =  439./216.;
		a[7] = -8.0;
		a[8] = 3680./513.
		a[9] = -845.0/4104.;

		a[10] = -8./27.0;
		a[11] = 2.0;
		a[12] = -3544./2565.;
		a[13] = 1859./4104.;
		a[14] =  -11./40.; 		

		c[0] = 0.25;
		c[1] = 3./8.;
		c[2] = 12./13.;
		c[3] = 1.0;
		c[4] = 0.5;
}

void assign_heun_euler(double *a,double *b,double *berr,double *c)
{
		b[0] = 1.0;
		b[1] = 0.0;
		berr[0] = -0.5;  
		berr[1]	= 0.5;
		a[0] = 1.0;
		c[0] = 1.0;
}

void assign_bogacki_shampire(double *a,double *b,double *berr,double *c)
{
		b[0] = 2.0/9.;
		b[1] = 1./3.;
		b[2] = 4./9.;
		b[3] = 0.0; 
		berr[0] = 7./24.-b[0];  
		berr[1]	= 0.25 - b[1];
		berr[2] = 1./3.- b[2];
		berr[3] = 0.125;
		a[0] = 0.5;
		a[1] = 0.0;
		a[2] = 0.75;
		a[3] = 2./9.;
		a[4] = 1./3.;
		a[5] = 4./9.;
		c[0] = 0.5;
		c[1] = 0.75;
		c[2] = 1.0;
}


}
#endif






