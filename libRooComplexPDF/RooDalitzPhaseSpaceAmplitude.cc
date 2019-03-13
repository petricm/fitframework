/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
 * This code was autogenerated by RooClassFactory                            *
 *****************************************************************************/

// Your description goes here...

#include "Riostream.h"

#include "RooDalitzPhaseSpaceAmplitude.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include <math.h>
#include "TMath.h"

#include <complex>
#include "libComplexPDF/libComplexPDF.h"

ClassImp(RooDalitzPhaseSpaceAmplitude)

RooDalitzPhaseSpaceAmplitude::RooDalitzPhaseSpaceAmplitude(const char *name, const char *title,
                                                           RooAbsReal& _m12,
                                                           RooAbsReal& _m23,
                                                           double _m1,
                                                           double _m2,
                                                           double _m3,
                                                           double _mM) :
RooAbsPdf(name,title),
m12("m12","m12",this,_m12),
m23("m23","m23",this,_m23),
m1(_m1),
m2(_m2),
m3(_m3),
mM(_mM) {
}

RooDalitzPhaseSpaceAmplitude::RooDalitzPhaseSpaceAmplitude(const RooDalitzPhaseSpaceAmplitude& other, const char* name) :
RooAbsPdf(other,name),
m12("m12",this,other.m12),
m23("m23",this,other.m23),
m1(other.m1),
m2(other.m2),
m3(other.m3),
mM(other.mM) {
}

Double_t RooDalitzPhaseSpaceAmplitude::evaluate() const {
	if(ComplexPDF::InsideDalitzLimits(std::sqrt(m12),std::sqrt(m23),mM,m1,m2,m3))
		return std::norm(ComplexPDF::DalitzPhaseSpaceAmplitude(m12,m23));
	else
		return 0.0;
}

Int_t RooDalitzPhaseSpaceAmplitude::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /* rangeName */) const {
	
    /*if (matchArgs(allVars, analVars, m12, m23))
		return 1;*/
	//std::cerr << "INFO: Unsupported analytical integral was requested in RooDalitzAnalysis::getAnalyticalIntegral(...) " <<
	//		"--> Numeric integration will be used." <<  std::endl;
	return 0;
}


Double_t RooDalitzPhaseSpaceAmplitude::analyticalIntegral(Int_t code, const char* rangeName ) const {
    if (code == 1) {
        return 595.03336458209;
    }
    std::cerr << "ERROR: Unsupported analytical integral code=" << code << " in RooDalitzPhaseSpaceAmplitude::analyticalIntegral(...) " <<
    "--> Return 0" << std::endl;
	return 0;
}
