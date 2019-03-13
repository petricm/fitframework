#pragma once
/**
 * @file	KKInterferenceNormCachedIntegral.h
 * @date	May 23, 2011
 * @author	mprim
 * @brief	brief description
 *
 * long description
 */

#ifndef KKINTERFERENCEINORMCACHEDINTEGRAL_H_
#define KKINTERFERENCEINORMCACHEDINTEGRAL_H_

#include "CacheIntegrator/CachedIntegral.h"
#include "libComplexPDF/libComplexPDF.h"
#include "boost/math/constants/constants.hpp"

/**
 * Norm part integral of the MassHelictyAngles distribtion integral.
 */
class KKInterferenceNormCachedIntegral : public CachedIntegral {
public:
	KKInterferenceNormCachedIntegral() : CachedIntegral() {};
	KKInterferenceNormCachedIntegral(const std::vector<std::pair<double, double> >& limits,
			const std::vector<double>& parameters,
			const double eps_abs = -1, const double eps_rel = -1, const double integral = -999, const int neval = -1)
	: CachedIntegral(limits,parameters,eps_abs,eps_rel,integral,neval) {};

	virtual double DoEval(const double* x) const {
		static const double pi = boost::math::constants::pi<double>();
		// variables x[]
		double m = x[0];
		double phi = x[1];
		double th1 = std::acos(x[2]);
		double th2 = std::acos(x[3]);
		// parameters
		double mass_flatte = m_parameters[0];
		double g_1 = m_parameters[1];
		double g_2 = m_parameters[2];
		double g_3 = m_parameters[3];
		double m_1a = m_parameters[4];
		double m_1b = m_parameters[5];
		double m_2a = m_parameters[6];
		double m_2b = m_parameters[7];
		double m_3a = m_parameters[8];
		double m_3b = m_parameters[9];
		double m1 = m_parameters[10];
		double m2 = m_parameters[11];
		double m3 = m_parameters[12];
		double mM = m_parameters[13];
		double mass_phi = m_parameters[14];
		double width_phi = m_parameters[15];
		double J_phi = m_parameters[16];
		double r_phi = m_parameters[17];
		// efficiency function
		ACCEPTANCE_FUNCTION eff_func = (ACCEPTANCE_FUNCTION)((unsigned int)m_parameters[18]);
		// compute norm for S or P wave
		unsigned int wave = (unsigned int)m_parameters[19];

		// calculate momentum in rest frame of resonance
		double q = ComplexPDF::CalculateMomentum(m,m1,m2);
		// if the resonance is evaluated at m<(m1+m2), q values could be nan and we should return 0 probability
		if(std::isnan(q) || m < (m1+m2) || m > (mM-m3))
			return 0.0;

		if(wave == 0) {
			// *** complex amplitude of flatte
			// * angular distribution
			std::complex<double> angular_flatte = ComplexPDF::SphericalHarmonic(1,0,th1,(pi-phi)) * ComplexPDF::SphericalHarmonic(0,0,-th2,0);
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
			// phase space factor
			double F = ComplexPDF::PhaseSpaceFactor(m,mM,m1,m2,m3);
			// acceptance function (use K892 central position as eff for m_KPi and thus assume flat acceptance in m_KK distribution
			return GetEff(m3,phi,th1,th2,eff_func) * std::norm(KK_flatte*angular_flatte) * F;
		} else if(wave == 1) {
			// *** complex amplitude of phi
			// * angular distribution -helicity
			std::complex<double> angular_phi =	ComplexPDF::SphericalHarmonic(1,-1,th1,(pi-phi)) * ComplexPDF::SphericalHarmonic(1,1,-th2,0);
			// * mass distribution
			double q_phi = ComplexPDF::CalculateMomentum(mass_phi,m1,m2);
			std::complex<double> KK_phi = ComplexPDF::RelativisticSpinBreitWigner(m,mass_phi,width_phi,r_phi,J_phi,q,q_phi);
			// phase space factor
			double F = ComplexPDF::PhaseSpaceFactor(m,mM,m1,m2,m3);
			// acceptance function (use K892 central position as eff for m_KPi and thus assume flat acceptance in m_KK distribution
			return GetEff(m3,phi,th1,th2,eff_func) * std::norm(KK_phi*angular_phi) * F;
		} else if(wave == 2) {
			// *** complex amplitude of phi
			// * angular distribution 0helicity
			std::complex<double> angular_phi =	ComplexPDF::SphericalHarmonic(1,0,th1,(pi-phi)) * ComplexPDF::SphericalHarmonic(1,0,-th2,0);
			// * mass distribution
			double q_phi = ComplexPDF::CalculateMomentum(mass_phi,m1,m2);
			std::complex<double> KK_phi = ComplexPDF::RelativisticSpinBreitWigner(m,mass_phi,width_phi,r_phi,J_phi,q,q_phi);
			// phase space factor
			double F = ComplexPDF::PhaseSpaceFactor(m,mM,m1,m2,m3);
			// acceptance function (use K892 central position as eff for m_KPi and thus assume flat acceptance in m_KK distribution
			return GetEff(m3,phi,th1,th2,eff_func) * std::norm(KK_phi*angular_phi) * F;
		} else if(wave == 3) {
			// *** complex amplitude of phi
			// * angular distribution +helicity
			std::complex<double> angular_phi =	ComplexPDF::SphericalHarmonic(1,1,th1,(pi-phi)) * ComplexPDF::SphericalHarmonic(1,-1,-th2,0);
			// * mass distribution
			double q_phi = ComplexPDF::CalculateMomentum(mass_phi,m1,m2);
			std::complex<double> KK_phi = ComplexPDF::RelativisticSpinBreitWigner(m,mass_phi,width_phi,r_phi,J_phi,q,q_phi);
			// phase space factor
			double F = ComplexPDF::PhaseSpaceFactor(m,mM,m1,m2,m3);
			// acceptance function (use K892 central position as eff for m_KPi and thus assume flat acceptance in m_KK distribution
			return GetEff(m3,phi,th1,th2,eff_func) * std::norm(KK_phi*angular_phi) * F;
		} else {
			std::cerr << "ERROR: Unknown wave type " << wave << " was used in KKInterferenceNormCachedIntegral" << std::endl;
			return 0.0;
		}
	}

	virtual ROOT::Math::IBaseFunctionMultiDim* Clone() const {
		KKInterferenceNormCachedIntegral* f = new KKInterferenceNormCachedIntegral(*this);
		return f;
	}

	virtual std::string GetIntegralClassIdentifier() const {
		return "KKInterferenceNormCachedIntegral";
	}
};


#endif
