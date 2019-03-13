/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
 * This code was autogenerated by RooClassFactory                            *
 *****************************************************************************/

// Your description goes here...

#include "Riostream.h"

#include "RooDalitzRelativisticSpinBreitWigner.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include <math.h>
#include "TMath.h"
#include "libComplexPDF/libComplexPDF.h"

ClassImp(RooDalitzRelativisticSpinBreitWigner)

RooDalitzRelativisticSpinBreitWigner::RooDalitzRelativisticSpinBreitWigner(const char *name, const char *title,
                                                                           RooAbsReal& _m12,
                                                                           RooAbsReal& _m23,
                                                                           RooAbsReal& _mass_J,
                                                                           RooAbsReal& _width_J,
                                                                           RooAbsReal& _c1,
                                                                           RooAbsReal& _c2,
                                                                           RooAbsReal& _c3,
                                                                           RooAbsReal& _c4,
                                                                           double _radius,
                                                                           unsigned int _spin,
                                                                           int _comb,
                                                                           double _m1,
                                                                           double _m2,
                                                                           double _m3,
                                                                           double _mM) :
RooAbsPdf(name,title),
m12("m12","m12",this,_m12),
m23("m23","m23",this,_m23),
mass_J("mass_J","mass_J",this,_mass_J),
width_J("width_J","width_J",this,_width_J),
c1("c1","c1",this,_c1),
c2("c2","c2",this,_c2),
c3("c3","c3",this,_c3),
c4("c4","c4",this,_c4),
radius(_radius),
spin(_spin),
comb(_comb),
m1(_m1),
m2(_m2),
m3(_m3),
mM(_mM) {
}

RooDalitzRelativisticSpinBreitWigner::RooDalitzRelativisticSpinBreitWigner(const RooDalitzRelativisticSpinBreitWigner& other, const char* name) :
RooAbsPdf(other,name),
m12("m12",this,other.m12),
m23("m23",this,other.m23),
mass_J("mass_J",this,other.mass_J),
width_J("width_J",this,other.width_J),
c1("c1",this,other.c1),
c2("c2",this,other.c2),
c3("c3",this,other.c3),
c4("c4",this,other.c4),
radius(other.radius),
spin(other.spin),
comb(other.comb),
m1(other.m1),
m2(other.m2),
m3(other.m3),
mM(other.mM) {
}

double RooDalitzRelativisticSpinBreitWigner::function(double x, double y) const{
    
    double M_Res = std::sqrt(x);
    double M_Ort = y;
    double M_A = 0;
    double M_B = 0;
    double x_min = 0;
    double x_max = 0;

    if (comb==12) {
        x_min = (m1+m2)*(m1+m2);
        x_max = (mM-m3)*(mM-m3);
        M_A = m1;
        M_B = m2;
    }else if (comb==23) {
        x_min = (m2+m3)*(m2+m3);
        x_max = (mM-m1)*(mM-m1);
        M_A = m2;
        M_B = m3;
    }else if (comb==31) {
        x_min = (m1+m3)*(m1+m3);
        x_max = (mM-m2)*(mM-m2);
        M_A = m3;
        M_B = m1;
    }else{
        std::cerr << "ERROR: combination " << comb << " not possible in RooDalitzRelativisticSpinBreitWigner(..)" << std::endl;
    }
    
    M_Ort = M_Ort - (x_max-x_min)/2.0;
    
    const double pAB = ComplexPDF::CalculateMomentum(M_Res, M_A, M_B);
    const double p_Res = ComplexPDF::CalculateMomentum(mass_J, M_A, M_B );
    
//    std::cout <<radius<<"\t"<<spin<<std::endl;
    std::complex<double> RBW = ComplexPDF::RelativisticSpinBreitWigner(M_Res, mass_J, width_J, radius, spin, pAB, p_Res);
    double poly = std::abs(1 + c1*M_Ort + c2*(2*M_Ort*M_Ort - 1) + c3*(4*M_Ort*M_Ort*M_Ort - 3*M_Ort) + c4*(8*M_Ort*M_Ort*M_Ort*M_Ort-8*M_Ort*M_Ort+1));
    return std::norm(RBW)*poly;
    
    
}


Double_t RooDalitzRelativisticSpinBreitWigner::evaluate() const {
    
	double sqrt_m12 = std::sqrt(m12);
	double sqrt_m23 = std::sqrt(m23);
    double m31 = m1*m1 + m2*m2 + m3*m3 + mM*mM - m12 -m23;
//        std::cout <<radius<<"\t"<<spin<<"\t"<<m1<<std::endl;
	if(ComplexPDF::InsideDalitzLimits(sqrt_m12,sqrt_m23,mM,m1,m2,m3)) {
        if (comb==12) {
            return function(m12,m23);
        }
        
        if (comb==23) {
            return function(m23,m31);
        }
        
        if (comb==31) {
            return function(m31,m12);
        }
    }else{
        return 1.0e-5;
    }
    
}


Double_t RooDalitzRelativisticSpinBreitWigner::adaptiveSimpsonsAux(double x, double a, double b, double epsilon, double S, double fa, double fb, double fc, int bottom) const {
    Double_t c = (a + b)/2, h = b - a;
    Double_t d = (a + c)/2, e = (c + b)/2;
    
    Double_t fd = function(x,d);
    Double_t fe = function(x,e);
    
    
    Double_t Sleft = (h/12)*(fa + 4*fd + fc);
    Double_t Sright = (h/12)*(fc + 4*fe + fb);
    Double_t S2 = Sleft + Sright;
    
    if (bottom <= 0 || fabs(S2 - S) <= 15*epsilon){
        return S2 + (S2 - S)/15;
    }
    return adaptiveSimpsonsAux(x, a, c, epsilon/2, Sleft,  fa, fc, fd, bottom-1) + adaptiveSimpsonsAux(x, c, b, epsilon/2, Sright, fc, fb, fe, bottom-1);
}

Double_t RooDalitzRelativisticSpinBreitWigner::adaptiveSimpsons(double x, double a, double b, double epsilon, int maxRecursionDepth) const {
    Double_t c = (a + b)/2;
    Double_t h = b - a;
    Double_t fa = function(x,a);
    Double_t fb = function(x,b);
    Double_t fc = function(x,c);
    Double_t S = (h/6)*(fa + 4*fc + fb);
    return adaptiveSimpsonsAux(x, a, b, epsilon, S, fa, fb, fc, maxRecursionDepth);
}

Int_t RooDalitzRelativisticSpinBreitWigner::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /* rangeName */) const {
	
    //if (matchArgs(allVars, analVars, m12, m23))
	//	return 1;
	//std::cerr << "INFO: Unsupported analytical integral was requested in RooDalitzAnalysis::getAnalyticalIntegral(...) " <<
	//		"--> Numeric integration will be used." <<  std::endl;
	return 0;
}


Double_t RooDalitzRelativisticSpinBreitWigner::analyticalIntegral(Int_t code, const char* rangeName ) const {
    
    int n_slices = 500;
    double eps = 0.1/(double)n_slices;
    
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
                integral+=adaptiveSimpsons(x, m23_min_2, m23_max_2, eps, 10)*dx;
            }
            return integral;
        }
        
        if (comb==23) {
            double x_min = (m2+m3)*(m2+m3);
            double x_max = (mM-m1)*(mM-m1);
            
            double dx = (x_max-x_min)/(double)n_slices;
            double x=0;
            Double_t integral = 0;
            double m31_max_2;
            double m31_min_2;
            
            for (int i = 1; i<n_slices; i++) {
                x = x_min + i *dx;
                ComplexPDF::DalitzLimits(std::sqrt(x), mM, m2, m3, m1, m31_max_2, m31_min_2);
                integral+=adaptiveSimpsons(x, m31_min_2, m31_max_2, eps, 10)*dx;
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
                integral+=adaptiveSimpsons(x, m23_min_2, m23_max_2, eps, 10)*dx;
            }
            return integral;
        }
        
	}
	std::cerr << "ERROR: Unsupported analytical integral code=" << code << " in RooDalitzGauss::analyticalIntegral(...) " <<
    "--> Return 0" << std::endl;
	return 0;
}
