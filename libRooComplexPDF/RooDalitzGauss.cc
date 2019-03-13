/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
 * This code was autogenerated by RooClassFactory                            *
 *****************************************************************************/

// Your description goes here...

#include "Riostream.h"

#include "RooDalitzGauss.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include <math.h>
#include "RooMath.h"

#include <complex>
#include "libComplexPDF/libComplexPDF.h"

ClassImp(RooDalitzGauss)

RooDalitzGauss::RooDalitzGauss(const char *name, const char *title,
                               RooAbsReal& _m12,
                               RooAbsReal& _m23,
                               RooAbsReal& _mean,
                               RooAbsReal& _sigma,
                               RooAbsReal& _c1,
                               RooAbsReal& _c2,
                               RooAbsReal& _c3,
                               RooAbsReal& _c4,
                               int _comb,
                               double _m1,
                               double _m2,
                               double _m3,
                               double _mM) :
RooAbsPdf(name,title),
m12("m12","m12",this,_m12),
m23("m23","m23",this,_m23),
mean("mean","mean",this,_mean),
sigma("sigma","sigma",this,_sigma),
c1("c1","c1",this,_c1),
c2("c2","c2",this,_c2),
c3("c3","c3",this,_c3),
c4("c4","c4",this,_c4),
comb(_comb),
m1(_m1),
m2(_m2),
m3(_m3),
mM(_mM) {
}

RooDalitzGauss::RooDalitzGauss(const RooDalitzGauss& other, const char* name) :
RooAbsPdf(other,name),
m12("m12",this,other.m12),
m23("m23",this,other.m23),
mean("mean",this,other.mean),
sigma("sigma",this,other.sigma),
c1("c1",this,other.c1),
c2("c2",this,other.c2),
c3("c3",this,other.c3),
c4("c4",this,other.c4),
comb(other.comb),
m1(other.m1),
m2(other.m2),
m3(other.m3),
mM(other.mM) {
}

Double_t RooDalitzGauss::evaluate() const {
	double sqrt_m12 = std::sqrt(m12);
	double sqrt_m23 = std::sqrt(m23);
    double m31 = m1*m1 + m2*m2 + m3*m3 + mM*mM - m12 -m23;
	if(ComplexPDF::InsideDalitzLimits(sqrt_m12,sqrt_m23,mM,m1,m2,m3)) {
		
        if (comb==12) {
            Double_t arg= m12 - mean;
            Double_t gauss = std::exp(-0.5*arg*arg/(sigma*sigma));
            double x_min = (m2+m3)*(m2+m3);
            double x_max = (mM-m1)*(mM-m1);
            double x0 = m23 - (x_max-x_min)/2.0 ;
            Double_t poly = std::abs(1 + c1*x0 + c2*(2*x0*x0 - 1) + c3*(4*x0*x0*x0 - 3*x0) + c4*(8*x0*x0*x0*x0-8*x0*x0+1));
            return gauss*poly;
        }
        
        if (comb==23) {
            Double_t arg= m23 - mean;
            Double_t gauss = std::exp(-0.5*arg*arg/(sigma*sigma));
            double x_min = (m1+m3)*(m1+m3);
            double x_max = (mM-m2)*(mM-m2);
            double x0 = m31 - (x_max-x_min)/2.0 ;
            Double_t poly = std::abs(1 + c1*x0 + c2*(2*x0*x0 - 1) + c3*(4*x0*x0*x0 - 3*x0) + c4*(8*x0*x0*x0*x0-8*x0*x0+1));
            return gauss*poly;
        }
        
        if (comb==31) {
            Double_t arg= m31 - mean;
            Double_t gauss = std::exp(-0.5*arg*arg/(sigma*sigma));
            double x_min = (m1+m2)*(m1+m2);
            double x_max = (mM-m3)*(mM-m3);
            double x0 = m12 - (x_max-x_min)/2.0 ;
            Double_t poly = std::abs(1 + c1*x0 + c2*(2*x0*x0 - 1) + c3*(4*x0*x0*x0 - 3*x0) + c4*(8*x0*x0*x0*x0-8*x0*x0+1));
            return gauss*poly;
        }
        
	} else {
		return 1.0e-5;
	}
}


Int_t RooDalitzGauss::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /* rangeName */) const {
	
    //if (matchArgs(allVars, analVars, m12, m23))
	//	return 1;
	//std::cerr << "INFO: Unsupported analytical integral was requested in RooDalitzAnalysis::getAnalyticalIntegral(...) " <<
	//		"--> Numeric integration will be used." <<  std::endl;
	return 0;
}


Double_t RooDalitzGauss::analyticalIntegral(Int_t code, const char* rangeName ) const {
    
    int n_slices = 500;
    
    static const Double_t root2 = sqrt(2.) ;
    static const Double_t rootPiBy2 = sqrt(atan2(0.0,-1.0)/2.0);
    Double_t xscale = root2*sigma;
    
	if (code == 1) {
        
        if (comb==12) {
            double x_min = (m1+m2)*(m1+m2);
            double x_max = (mM-m3)*(mM-m3);
            double dx = (x_max-x_min)/(double)n_slices;
            double x=0;
            Double_t integral = 0;
            double m23_max_2;
            double m23_min_2;
            
            for (int i = 1; i<n_slices; i++) {
                x = x_min + i *dx;
                ComplexPDF::DalitzLimits(std::sqrt(x), mM, m1, m2, m3, m23_max_2, m23_min_2);
                double y_min = (m2+m3)*(m2+m3);
                double y_max = (mM-m1)*(mM-m1);
                double y0 = x - (y_max-y_min)/2.0 ;
                Double_t poly = std::abs(1 + c1*y0 + c2*(2*y0*y0 - 1) + c3*(4*y0*y0*y0 - 3*y0) + c4*(8*y0*y0*y0*y0-8*y0*y0+1));
                integral+=(rootPiBy2*sigma*(RooMath::erf((m23_max_2-mean)/xscale)-RooMath::erf((m23_min_2-mean)/xscale)))*poly*dx;
            }
            return integral;
        }
        
        if (comb==23) {
            double x_min = (m2+m3)*(m2+m3);
            double x_max = (mM-m1)*(mM-m1);
            double dx = (x_max-x_min)/(double)n_slices;
            double x=0;
            Double_t integral = 0;
            double m23_max_2;
            double m23_min_2;
            
            for (int i = 1; i<n_slices; i++) {
                x = x_min + i *dx;
                ComplexPDF::DalitzLimits(std::sqrt(x), mM, m2, m3, m1, m23_max_2, m23_min_2);
                double y_min = (m1+m3)*(m1+m3);
                double y_max = (mM-m2)*(mM-m2);
                double y0 = x - (y_max-y_min)/2.0 ;
                Double_t poly = std::abs(1 + c1*y0 + c2*(2*y0*y0 - 1) + c3*(4*y0*y0*y0 - 3*y0) + c4*(8*y0*y0*y0*y0-8*y0*y0+1));
                integral+=(rootPiBy2*sigma*(RooMath::erf((m23_max_2-mean)/xscale)-RooMath::erf((m23_min_2-mean)/xscale)))*dx*poly;
            }
            return integral;
        }
        
        if (comb==31) {
            double x_min = (m1+m3)*(m1+m3);
            double x_max = (mM-m2)*(mM-m2);
            double dx = (x_max-x_min)/(double)n_slices;
            double x=0;
            Double_t integral = 0;
            double m23_max_2;
            double m23_min_2;
            
            for (int i = 1; i<n_slices; i++) {
                x = x_min + i *dx;
                ComplexPDF::DalitzLimits(std::sqrt(x), mM, m3, m1, m2, m23_max_2, m23_min_2);
                double y_min = (m1+m2)*(m1+m2);
                double y_max = (mM-m3)*(mM-m3);
                double y0 = x - (y_max-y_min)/2.0 ;
                Double_t poly = std::abs(1 + c1*y0 + c2*(2*y0*y0 - 1) + c3*(4*y0*y0*y0 - 3*y0) + c4*(8*y0*y0*y0*y0-8*y0*y0+1));
                integral+=(rootPiBy2*sigma*(RooMath::erf((m23_max_2-mean)/xscale)-RooMath::erf((m23_min_2-mean)/xscale)))*dx*poly;
            }
            return integral;
        }
        
	}
	std::cerr << "ERROR: Unsupported analytical integral code=" << code << " in RooDalitzGauss::analyticalIntegral(...) " <<
    "--> Return 0" << std::endl;
	return 0;
}


