#pragma once
/**
 * @file	MassHelicityAnglesNormCachedIntegral_m_phi_th1.h
 * @date	May 23, 2011
 * @author	mprim
 * @brief	brief description
 *
 * long description
 */

#ifndef MASSHELICITYANGLESNORMCACHEDINTEGRAL_m_phi_th1_H_
#define MASSHELICITYANGLESNORMCACHEDINTEGRAL_m_phi_th1_H_

#include "CacheIntegrator/CachedIntegral.h"
#include "CacheIntegrator/MassHelicityAnglesCore.h"
#include "libComplexPDF/libComplexPDF.h"

/**
 * Norm part integral of the MassHelictyAngles distribtion integral.
 */
class MassHelicityAnglesNormCachedIntegral_m_phi_th1 : public MassHelicityAnglesCoreCachedIntegral {
public:
	MassHelicityAnglesNormCachedIntegral_m_phi_th1() : MassHelicityAnglesCoreCachedIntegral() {};
	MassHelicityAnglesNormCachedIntegral_m_phi_th1(const std::vector<std::pair<double, double> >& limits,
			const std::vector<double>& parameters,
			const double eps_abs = -1, const double eps_rel = -1, const double integral = -999, const int neval = -1)
	: MassHelicityAnglesCoreCachedIntegral(limits,parameters,eps_abs,eps_rel,integral,neval) {};

	virtual double DoEval(const double* x) const {
		// variables x[]
		double m = x[0];
		double phi = x[1];
		double th1 = std::acos(x[2]);
		// variable parameters
		unsigned int J = (unsigned int)m_parameters[0];
		int lambda = (int)m_parameters[1];
		//unsigned int J_cc = (unsigned int)m_parameters[2];
		//int lambda_cc = (int)m_parameters[3];
		// constant parameters for resonances
		double mass_0 = m_parameters[4];
		double width_0 = m_parameters[5];
		double a = m_parameters[6];
		double b = m_parameters[7];
		double mass_1 = m_parameters[8];
		double width_1 = m_parameters[9];
		double r_1 = m_parameters[10];
		double mass_2 = m_parameters[11];
		double width_2 = m_parameters[12];
		double r_2 = m_parameters[13];
		// constant parameters
		double m_B0 = m_parameters[14];
		double m_Pi = m_parameters[15];
		double m_K = m_parameters[16];
		double m_Phi = m_parameters[17];
		// efficiency function
		ACCEPTANCE_FUNCTION eff_func = (ACCEPTANCE_FUNCTION)((unsigned int)m_parameters[18]);
		// cos_th2
		double th2 = std::acos(m_parameters[19]);

		std::complex<double> S = GetS(m,phi,th1,th2,J,lambda,mass_0,width_0,a,b,mass_1,width_1,r_1,mass_2,width_2,r_2,m_Pi,m_K);
		double F = ComplexPDF::PhaseSpaceFactor(m,m_B0,m_Pi,m_K,m_Phi);

		return GetEff(m,phi,th1,th2,eff_func) * std::norm(S) * F;
	}

	virtual ROOT::Math::IBaseFunctionMultiDim* Clone() const {
		MassHelicityAnglesNormCachedIntegral_m_phi_th1* f = new MassHelicityAnglesNormCachedIntegral_m_phi_th1(*this);
		return f;
	}

	virtual std::string GetIntegralClassIdentifier() const {
		return "MassHelicityAnglesNormCachedIntegral_m_phi_th1";
	}
};

#endif /* MASSHELICITYANGLESNORMCACHEDINTEGRAL_m_phi_th1_H_ */
