/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
 * This code was autogenerated by RooClassFactory                            *
 *****************************************************************************/

// Your description goes here...

#include "Riostream.h"

#include "RooDalitzNovosibirsk.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include <math.h>
#include "RooMath.h"
#include "TMath.h"

#include "libComplexPDF/libComplexPDF.h"

ClassImp(RooDalitzNovosibirsk)

RooDalitzNovosibirsk::RooDalitzNovosibirsk(const char *name, const char *title,
                               RooAbsReal& _m12,
                               RooAbsReal& _m23,
                               RooAbsReal& _mean,
                               RooAbsReal& _sigma,
                               RooAbsReal& _tail,
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
tail("tail","tail",this,_tail),
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

RooDalitzNovosibirsk::RooDalitzNovosibirsk(const RooDalitzNovosibirsk& other, const char* name) :
RooAbsPdf(other,name),
m12("m12",this,other.m12),
m23("m23",this,other.m23),
mean("mean",this,other.mean),
sigma("sigma",this,other.sigma),
tail("tail",this,other.tail),
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

Double_t RooDalitzNovosibirsk::evaluate() const {
	double sqrt_m12 = std::sqrt(m12);
	double sqrt_m23 = std::sqrt(m23);
    double m31 = m1*m1 + m2*m2 + m3*m3 + mM*mM - m12 -m23;
	if(ComplexPDF::InsideDalitzLimits(sqrt_m12,sqrt_m23,mM,m1,m2,m3)) {
		double poly = 0;
        double x = 0;
        if (comb==12) {
            x = m12;
            double x_min = (m2+m3)*(m2+m3);
            double x_max = (mM-m1)*(mM-m1);
            double x0 = m23 - (x_max-x_min)/2.0 ;
            poly = std::abs(1 + c1*x0 + c2*(2*x0*x0 - 1) + c3*(4*x0*x0*x0 - 3*x0) + c4*(8*x0*x0*x0*x0-8*x0*x0+1));
        }
        
        if (comb==23) {
            x = m23;
            double x_min = (m1+m3)*(m1+m3);
            double x_max = (mM-m2)*(mM-m2);
            double x0 = m31 - (x_max-x_min)/2.0 ;
            poly = std::abs(1 + c1*x0 + c2*(2*x0*x0 - 1) + c3*(4*x0*x0*x0 - 3*x0) + c4*(8*x0*x0*x0*x0-8*x0*x0+1));
        }
        
        if (comb==31) {
            x = m31;
            double x_min = (m1+m2)*(m1+m2);
            double x_max = (mM-m3)*(mM-m3);
            double x0 = m12 - (x_max-x_min)/2.0 ;
            poly = std::abs(1 + c1*x0 + c2*(2*x0*x0 - 1) + c3*(4*x0*x0*x0 - 3*x0) + c4*(8*x0*x0*x0*x0-8*x0*x0+1));
        }
        
        
        if (TMath::Abs(tail) < 1.e-7) {
            return TMath::Exp( -0.5 * TMath::Power( ( (x - mean) / sigma ), 2 ));
        }
        
        Double_t arg = 1.0 - ( x - mean ) * tail / sigma;
        
        if (arg < 1.e-7) {
            //Argument of logaritem negative. Real continuation -> function equals zero
            return 1.0e-05;
        }
        
        Double_t log = TMath::Log(arg);
        static const Double_t xi = 2.3548200450309494; // 2 Sqrt( Ln(4) )
        
        Double_t width_zero = ( 2.0 / xi ) * TMath::ASinH( tail * xi * 0.5 );
        Double_t width_zero2 = width_zero * width_zero;
        Double_t exponent = ( -0.5 / (width_zero2) * log * log ) - ( width_zero2 * 0.5 );
        
        return TMath::Exp(exponent) * poly;
        
	} else {
		return 1.0e-5;
	}
}



