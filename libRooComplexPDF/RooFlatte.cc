/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "RooFlatte.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h> 
#include "TMath.h" 
#include "libComplexPDF/libComplexPDF.h"

ClassImp(RooFlatte) 

RooFlatte::RooFlatte(const char *name, const char *title,
		RooAbsReal& _m,
		RooAbsReal& _mass_R,
		RooAbsReal& _g_1,
		RooAbsReal& _g_2,
		RooAbsReal& _g_3,
		double _m_1a,
		double _m_1b,
		double _m_2a,
		double _m_2b,
		double _m_3a,
		double _m_3b,
		double _m1,
		double _m2) :
		RooAbsPdf(name,title),
		m("m","m",this,_m),
		mass_R("mass_R","mass_R",this,_mass_R),
		g_1("g_1","g_1",this,_g_1),
		g_2("g2","g2",this,_g_2),
		g_3("g3","g3",this,_g_3),
		m_1a(_m_1a),
		m_1b(_m_1b),
		m_2a(_m_2a),
		m_2b(_m_2b),
		m_3a(_m_3a),
		m_3b(_m_3b),
		m1(_m1),
		m2(_m2){
}

RooFlatte::RooFlatte(const RooFlatte& other, const char* name) :
	   RooAbsPdf(other,name),
	   m("m",this,other.m),
	   mass_R("mass_R",this,other.mass_R),
	   g_1("g_1",this,other.g_1),
	   g_2("g2",this,other.g_2),
	   g_3("g3",this,other.g_3),
	   m_1a(other.m_1a),
	   m_1b(other.m_1b),
	   m_2a(other.m_2a),
	   m_2b(other.m_2b),
	   m_3a(other.m_3a),
	   m_3b(other.m_3b),
	   m1(other.m1),
	   m2(other.m2){
}

double RooFlatte::CalculateMomentum(double m) const {
	const double add_12 = m1 + m2;
	const double sub_12 = m1 - m2;

	return std::sqrt((m*m - add_12*add_12)*(m*m - sub_12*sub_12))/(2.0*m);
}

Double_t RooFlatte::evaluate() const {
	// The implementation in the ComplexPDF provides support for unlimited channels, this
	// RooFlatte PDF is just a wrapper to provide a simple example in case of three channels.
	// It should not be used directly, one should always incooperate the Flatte formalism
	// in the own PDF similar to the example below.

	// calculate momentum in rest frame of resonance
	double q = CalculateMomentum(m);
	// if the resonance is evaluated at m<(m1+m2), q values could be nan and we should return 0 probability
	if(std::isnan(q))
		return 0.0;

	// coupling constants of both channels
	std::vector<double> g;
	g.push_back(g_1);
	g.push_back(g_2);
	g.push_back(g_3);
	// children masses for both channels
	std::vector<std::pair<double,double> > c_mass;
	c_mass.push_back(std::make_pair(m_1a,m_1b));
	c_mass.push_back(std::make_pair(m_2a,m_2b));
	c_mass.push_back(std::make_pair(m_3a,m_3b));
	// compute amplitude using the flatte formalism
	std::complex<double> amplitude = ComplexPDF::FlatteFormalism(m,mass_R,g,c_mass,q);
	// return |A|^2
	return std::norm(amplitude);
}