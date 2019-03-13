/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "RooKKInterferenceNorm.h"
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h> 
#include "TMath.h" 
#include "libComplexPDF/libComplexPDF.h"
#include "boost/math/constants/constants.hpp"
#include "CacheIntegrator/CacheIntegrator.h"
#include "CacheIntegrator/KKInterferenceNormCachedIntegral.h"
#include "CacheIntegrator/KKInterferenceRealCachedIntegral.h"
#include "CacheIntegrator/KKInterferenceImagCachedIntegral.h"

ClassImp(RooKKInterferenceNorm)

RooKKInterferenceNorm::RooKKInterferenceNorm(const char *name, const char *title,
		RooAbsReal& _m,
		RooAbsReal& _phi,
		RooAbsReal& _cos_th1,
		RooAbsReal& _cos_th2,
		RooAbsReal& _amp_flatte,
		RooAbsReal& _phase_flatte,
		RooAbsReal& _mass_flatte,
		RooAbsReal& _g_1,
		RooAbsReal& _g_2,
		RooAbsReal& _g_3,
		RooAbsReal& _amp_phi_long,
		RooAbsReal& _phase_phi_long,
		RooAbsReal& _amp_phi_para,
		RooAbsReal& _phase_phi_para,
		RooAbsReal& _amp_phi_perp,
		RooAbsReal& _phase_phi_perp,
		RooAbsReal& _mass_phi,
		RooAbsReal& _width_phi,
		RooAbsReal& _r_phi,
		RooAbsReal& _J_phi,
		ACCEPTANCE_FUNCTION _acceptance_function,
		double _m_1a,
		double _m_1b,
		double _m_2a,
		double _m_2b,
		double _m_3a,
		double _m_3b,
		double _m1,
		double _m2,
		double _m3,
		double _mM,
		KKInterferenceCacheIntegrators_4D _s_4D) :
		RooAbsPdf(name,title),
		m("m","m",this,_m),
		phi("phi","phi",this,_phi),
		cos_th1("cos_th1","cos_th1",this,_cos_th1),
		cos_th2("cos_th2","cos_th2",this,_cos_th2),
		amp_flatte("amp_flatte","amp_flatte",this,_amp_flatte),
		phase_flatte("phase_flatte","phase_flatte",this,_phase_flatte),
		mass_flatte("mass_flatte","mass_flatte",this,_mass_flatte),
		g_1("g_1","g_1",this,_g_1),
		g_2("g_2","g_2",this,_g_2),
		g_3("g_3","g_3",this,_g_3),
		amp_phi_long("amp_phi_long","amp_phi_long",this,_amp_phi_long),
		phase_phi_long("phase_phi_long","phase_phi_long",this,_phase_phi_long),
		amp_phi_para("amp_phi_para","amp_phi_para",this,_amp_phi_para),
		phase_phi_para("phase_phi_para","phase_phi_para",this,_phase_phi_para),
		amp_phi_perp("amp_phi_perp","amp_phi_perp",this,_amp_phi_perp),
		phase_phi_perp("phase_phi_perp","phase_phi_perp",this,_phase_phi_perp),
		mass_phi("mass_phi","mass_phi",this,_mass_phi),
		width_phi("width_phi","width_phi",this,_width_phi),
		r_phi("r_phi","r_phi",this,_r_phi),
		J_phi("J_phi","J_phi",this,_J_phi),
		m_acceptance_function(_acceptance_function),
		m_1a(_m_1a),
		m_1b(_m_1b),
		m_2a(_m_2a),
		m_2b(_m_2b),
		m_3a(_m_3a),
		m_3b(_m_3b),
		m1(_m1),
		m2(_m2),
		m3(_m3),
		mM(_mM),
		m_norm_cache_integrator(_s_4D.norm_cache_integrator),
		m_real_cache_integrator(_s_4D.real_cache_integrator),
		m_imag_cache_integrator(_s_4D.imag_cache_integrator){
}

RooKKInterferenceNorm::RooKKInterferenceNorm(const RooKKInterferenceNorm& other, const char* name) :
	   RooAbsPdf(other,name),
	   m("m",this,other.m),
	   phi("phi",this,other.phi),
	   cos_th1("cos_th1",this,other.cos_th1),
	   cos_th2("cos_th2",this,other.cos_th2),
	   amp_flatte("amp_flatte",this,other.amp_flatte),
	   phase_flatte("phase_flatte",this,other.phase_flatte),
	   mass_flatte("mass_flatte",this,other.mass_flatte),
	   g_1("g_1",this,other.g_1),
	   g_2("g_2",this,other.g_2),
	   g_3("g_3",this,other.g_3),
	   amp_phi_long("amp_phi_long",this,other.amp_phi_long),
	   phase_phi_long("phase_phi_long",this,other.phase_phi_long),
	   amp_phi_para("amp_phi_para",this,other.amp_phi_para),
	   phase_phi_para("phase_phi_para",this,other.phase_phi_para),
	   amp_phi_perp("amp_phi_perp",this,other.amp_phi_perp),
	   phase_phi_perp("phase_phi_perp",this,other.phase_phi_perp),
	   mass_phi("mass_phi",this,other.mass_phi),
	   width_phi("width_phi",this,other.width_phi),
	   r_phi("r_phi",this,other.r_phi),
	   J_phi("J_phi",this,other.J_phi),
	   m_acceptance_function(other.m_acceptance_function),
	   m_1a(other.m_1a),
	   m_1b(other.m_1b),
	   m_2a(other.m_2a),
	   m_2b(other.m_2b),
	   m_3a(other.m_3a),
	   m_3b(other.m_3b),
	   m1(other.m1),
	   m2(other.m2),
	   m3(other.m3),
	   mM(other.mM),
	   m_norm_cache_integrator(other.m_norm_cache_integrator),
	   m_real_cache_integrator(other.m_real_cache_integrator),
	   m_imag_cache_integrator(other.m_imag_cache_integrator){
}

Double_t RooKKInterferenceNorm::evaluate() const {
	static const double pi = boost::math::constants::pi<double>();
	// calculate momentum in rest frame of resonance
	double q = ComplexPDF::CalculateMomentum(m,m1,m2);
	// if the resonance is evaluated at m<(m1+m2), q values could be nan and we should return 0 probability
	if(std::isnan(q) || m < (m1+m2) || m > (mM-m3))
		return 0.0;

	double acos_cos_th1 = std::acos(cos_th1);
	double acos_cos_th2 = std::acos(cos_th2);

	// acceptance function (use K892 central position as eff for m_KPi and thus assume flat acceptance in m_KK distribution
	double acceptance = GetEff(m3,phi,acos_cos_th1,acos_cos_th2,m_acceptance_function);

	// *** complex amplitude of flatte
	// * angular distribution
	//std::vector<std::complex<double> > A_0;
	//A_0.push_back(std::polar((double)amp_flatte,(double)phase_flatte));
	//std::complex<double> angular_flatte = ComplexPDF::HelicityAngles(phi,acos_cos_th1,acos_cos_th2,1,0,A_0);
	std::complex<double> angular_flatte = std::polar((double)amp_flatte,(double)phase_flatte) *
			ComplexPDF::SphericalHarmonic(1,0,acos_cos_th1,(pi-phi)) * ComplexPDF::SphericalHarmonic(0,0,-acos_cos_th2,0);

	// * mass distribution
	// coupling constants of both channels
	std::vector<double> g; g.reserve(3);
	g.push_back(g_1);
	g.push_back(g_2);
	g.push_back(g_3);
	// children masses for both channels
	std::vector<std::pair<double,double> > c_mass; c_mass.reserve(3);
	c_mass.push_back(std::make_pair(m_1a,m_1b));
	c_mass.push_back(std::make_pair(m_2a,m_2b));
	c_mass.push_back(std::make_pair(m_3a,m_3b));
	// compute amplitude using the flatte formalism
	std::complex<double> KK_flatte = ComplexPDF::FlatteFormalism(m,mass_flatte,g,c_mass,q);

	// *** complex amplitude of phi
	// * angular distribution
	// Spin J=1 (lambda -1,0,1)
//	std::complex<double> A_1para = std::polar((double)amp_phi_para,(double)phase_phi_para);
//	std::complex<double> A_1perp = std::polar((double)amp_phi_perp,(double)phase_phi_perp);
//	std::vector<std::complex<double> > A_1; A_1.reserve(3);
//	A_1.push_back((A_1para - A_1perp)/std::sqrt(2));
//	A_1.push_back(std::polar((double)amp_phi_long,(double)phase_phi_long));
//	A_1.push_back((A_1para + A_1perp)/std::sqrt(2));
//	std::complex<double> angular_phi = ComplexPDF::HelicityAngles(phi,acos_cos_th1,acos_cos_th2,1,1,A_1);
	//std::complex<double> angular_phi = std::polar((double)amp_phi_long,(double)phase_phi_long) *
			ComplexPDF::SphericalHarmonic(1,0,acos_cos_th1,(pi-phi)) * ComplexPDF::SphericalHarmonic(1,0,-acos_cos_th2,0);
	// * mass distribution
	//unsigned int J = (unsigned int)J_phi;
	//double q_phi = ComplexPDF::CalculateMomentum(mass_phi,m1,m2);
	//std::complex<double> KK_phi = ComplexPDF::RelativisticSpinBreitWigner(m,mass_phi,width_phi,r_phi,J,q,q_phi);

	// *** phase space factor
	double F = ComplexPDF::PhaseSpaceFactor(m,mM,m1,m2,m3);

	/*
	// combined amplitude
	std::complex<double> amplitude = KK_phi * angular_phi + KK_flatte * angular_flatte;

	// return |A|^2
	return acceptance * std::norm(amplitude) * F;
	*/

	//double amplitude = std::norm(KK_flatte * angular_flatte) + 2 * ((KK_flatte * angular_flatte) * std::conj(KK_phi * angular_phi)).real();
	double amplitude = std::norm(KK_flatte * angular_flatte);

//	if(amplitude < 0) {
//		std::cout << "ERR: AmplitudeNorm smaller than 0 for A_10 = (" << amp_phi_long << "," << phase_phi_long
//				<< ") and A_f = (" << amp_flatte << "," << phase_flatte << ") \n";
//	}

	return amplitude < 0 ? 0.0 : acceptance * amplitude * F;
}

Int_t RooKKInterferenceNorm::getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* /* rangeName */) const {
	if (matchArgs(allVars, analVars, m, phi, cos_th1, cos_th2))
		return 5;
//	if (matchArgs(allVars, analVars, phi, cos_th1, cos_th2))
//		return 4;
//	if (matchArgs(allVars, analVars, m, cos_th1, cos_th2))
//		return 3;
//	if (matchArgs(allVars, analVars, m, phi, cos_th2))
//		return 2;
//	if (matchArgs(allVars, analVars, m, phi, cos_th1))
//		return 1;
	//std::cerr << "INFO: Unsupported analytical integral was requested in RooMassHelicityAnglesCPV::getAnalyticalIntegral(...) " <<
	//		"--> Numeric integration will be used." <<  std::endl;
	return 0;
}

Double_t RooKKInterferenceNorm::analyticalIntegral(Int_t code, const char* rangeName ) const {
	// parameters index 0-17 are same for all integrals
	// index 18 is the acceptance function used
	// index 19 might be one of the variables (only for plots)
	std::vector<double> parameters;
	parameters.reserve(20);
	parameters.assign(20,0);
	// constant parameters for resonances
	parameters[0] = mass_flatte;
	parameters[1] = g_1;
	parameters[2] = g_2;
	parameters[3] = g_3;
	parameters[4] = m_1a;
	parameters[5] = m_1b;
	parameters[6] = m_2a;
	parameters[7] = m_2b;
	parameters[8] = m_3a;
	parameters[9] = m_3b;
	parameters[10] = m1;
	parameters[11] = m2;
	parameters[12] = m3;
	parameters[13] = mM;
	parameters[14] = mass_phi;
	parameters[15] = width_phi;
	parameters[16] = J_phi;
	parameters[17] = r_phi;
	// acceptance function
	parameters[18] = m_acceptance_function;
	// compute norm for S or P wave
	parameters[19] = 0;

	if (code == 5) {
		std::vector<std::pair<double,double> > limits; limits.reserve(4);
		limits.push_back(std::make_pair(m.min(rangeName),m.max(rangeName)));
		limits.push_back(std::make_pair(phi.min(rangeName),phi.max(rangeName)));
		limits.push_back(std::make_pair(cos_th1.min(rangeName),cos_th1.max(rangeName)));
		limits.push_back(std::make_pair(cos_th2.min(rangeName),cos_th2.max(rangeName)));
		// swap limits if necessary
		for(unsigned int i = 0; i < limits.size(); ++i) {
			if(limits[i].first > limits[i].second) {
				std::swap(limits[i].first, limits[i].second);
			}
		}

		// matrix element
		double M = 0;
		// Int |A_f0980|^2
		M += amp_flatte*amp_flatte*m_norm_cache_integrator->GetIntegral(limits,parameters);
		// Int Re(A_f0980 x A_phi_long*)
		//M += 2*amp_flatte*amp_phi_long*std::cos(phase_flatte - phase_phi_long)*m_real_cache_integrator->GetIntegral(limits,parameters);
		// Int Im(A_f0980 x A_phi_long*)
		//M -= 2*amp_flatte*amp_phi_long*std::sin(phase_flatte - phase_phi_long)*m_imag_cache_integrator->GetIntegral(limits,parameters);

		return M;
	}

	std::cerr << "ERROR: Unsupported analytical integral code=" << code << " in RooMassHelicityAnglesCPV::analyticalIntegral(...) " <<
			"--> Return 0" << std::endl;
	return 0.0;
}