/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "RooLASS.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h> 
#include "TMath.h" 
#include "libComplexPDF/libComplexPDF.h"

ClassImp(RooLASS) 

RooLASS::RooLASS(const char *name, const char *title,
		RooAbsReal& _m,
		RooAbsReal& _mass_0,
		RooAbsReal& _width_0,
		RooAbsReal& _a,
		RooAbsReal& _b,
		double _m1,
		double _m2) :
		RooAbsPdf(name,title),
		m("m","m",this,_m),
		mass_0("mass_0","mass_0",this,_mass_0),
		width_0("width_0","width_0",this,_width_0),
		a("a","a",this,_a),
		b("b","b",this,_b),
		m1(_m1),
		m2(_m2) {
}

RooLASS::RooLASS(const RooLASS& other, const char* name) :
	   RooAbsPdf(other,name),
	   m("m",this,other.m),
	   mass_0("mass_0",this,other.mass_0),
	   width_0("width_0",this,other.width_0),
	   a("a",this,other.a),
	   b("b",this,other.b),
	   m1(other.m1),
	   m2(other.m2) {
}

double RooLASS::CalculateMomentum(double m) const {
	const double add_12 = m1 + m2;
	const double sub_12 = m1 - m2;

	return std::sqrt((m*m - add_12*add_12)*(m*m - sub_12*sub_12))/(2.0*m);
}

Double_t RooLASS::evaluate() const {
	// calculate momentum in rest frame of resonance
	double q = CalculateMomentum(m);
	double q_0 = CalculateMomentum(mass_0);
	// call complexPDF C++ library to get complex amplitude
	std::complex<double> amplitude = ComplexPDF::LASS(m,mass_0,width_0,a,b,q,q_0);
	// return |A|^2
	return std::norm(amplitude);
}
