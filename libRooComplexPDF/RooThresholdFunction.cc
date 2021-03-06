/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "RooThresholdFunction.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h> 
#include "TMath.h" 

#include <complex>
#include "libComplexPDF/libComplexPDF.h"

ClassImp(RooThresholdFunction) 

RooThresholdFunction::RooThresholdFunction(const char *name, const char *title,
            RooAbsReal& _m12,
            RooAbsReal& _m23,
            RooAbsReal& _Ec1,
            RooAbsReal& _Ec2,
            RooAbsReal& _Ec3,
            RooAbsReal& _Eth1,
            RooAbsReal& _Eth2,
            RooAbsReal& _Eth3,
            RooAbsReal& _E_x,
            RooAbsReal& _E_y,
            RooAbsReal& _E_xx,
            RooAbsReal& _E_xy,
            RooAbsReal& _E_yy,
            RooAbsReal& _E_xxx,
            RooAbsReal& _E_xxy,
            RooAbsReal& _E_xyy,
            RooAbsReal& _E_yyy,
            double _m12_offset,
            double _m23_offset,
            double _mM,
            double _m1,
            double _m2,
            double _m3) :
            RooAbsPdf(name,title),
            m12("m12","m12",this,_m12),
            m23("m23","m23",this,_m23),
            Ec1("Ec1","Ec1",this,_Ec1),
            Ec2("Ec2","Ec2",this,_Ec2),
            Ec3("Ec3","Ec3",this,_Ec3),
            Eth1("Eth1","Eth1",this,_Eth1),
            Eth2("Eth2","Eth2",this,_Eth2),
            Eth3("Eth3","Eth3",this,_Eth3),
            E_x("E_x","E_x",this,_E_x),
            E_y("E_y","E_y",this,_E_y),
            E_xx("E_xx","E_xx",this,_E_xx),
            E_xy("E_xy","E_xy",this,_E_xy),
            E_yy("E_yy","E_yy",this,_E_yy),
            E_xxx("E_xxx","E_xxx",this,_E_xxx),
            E_xxy("E_xxy","E_xxy",this,_E_xxy),
            E_xyy("E_xyy","E_xyy",this,_E_xyy),
            E_yyy("E_yyy","E_yyy",this,_E_yyy),
            m12_offset(_m12_offset),
            m23_offset(_m23_offset),
            mM(_mM),
            m1(_m1),
            m2(_m2),
            m3(_m3) {
}

RooThresholdFunction::RooThresholdFunction(const RooThresholdFunction& other, const char* name) :
            RooAbsPdf(other,name),
            m12("m12",this,other.m12),
            m23("m23",this,other.m23),
            Ec1("Ec1",this,other.Ec1),
            Ec2("Ec2",this,other.Ec2),
            Ec3("Ec3",this,other.Ec3),
            Eth1("Eth1",this,other.Eth1),
            Eth2("Eth2",this,other.Eth2),
            Eth3("Eth3",this,other.Eth3),
            E_x("E_x",this,other.E_x),
            E_y("E_y",this,other.E_y),
            E_xx("E_xx",this,other.E_xx),
            E_xy("E_xy",this,other.E_xy),
            E_yy("E_yy",this,other.E_yy),
            E_xxx("E_xxx",this,other.E_xxx),
            E_xxy("E_xxy",this,other.E_xxy),
            E_xyy("E_xyy",this,other.E_xyy),
            E_yyy("E_yyy",this,other.E_yyy),
            m12_offset(other.m12_offset),
            m23_offset(other.m23_offset),
            mM(other.mM),
            m1(other.m1),
            m2(other.m2),
            m3(other.m3) {
}

Double_t RooThresholdFunction::evaluate() const {

    
    double m31 = mM*mM + m1*m1 + m2*m2 + m3*m3 - m12 - m23;
    
    const double v_max1 = (mM-m3)*(mM-m3);
    const double v_max2 = (mM-m1)*(mM-m1);
    const double v_max3 = (mM-m2)*(mM-m2);
    
    
	if(ComplexPDF::InsideDalitzLimits(std::sqrt(m12),std::sqrt(m23),mM,m1,m2,m3)) {
        
        double CLEO = ComplexPDF::CLEOThresholdFunction(m12,Ec1,Eth1,v_max1) * ComplexPDF::CLEOThresholdFunction(m23,Ec2,Eth2,v_max2) * ComplexPDF::CLEOThresholdFunction(m31,Ec3,Eth3,v_max3);
        
        double poly = ComplexPDF::DalitzCubicPolynomial(m12,m23,E_x,E_y,E_xx,E_xy,E_yy,E_xxx,E_xxy,E_xyy,E_yyy,m12_offset,m23_offset);
        
        if (std::isnan(CLEO) || std::isnan(poly)) {
            std::cerr << "ERROR: In RooThresholdFunction return value NAN!" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        return std::abs(CLEO*poly);
        
	} else {
		return 1.0e-16;
	}
}
