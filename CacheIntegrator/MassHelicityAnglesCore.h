#pragma once
/**
 * @file	MassHelicityAnglesCoreCachedIntegral.h
 * @date	May 23, 2011
 * @author	mprim
 * @brief	brief description
 *
 * long description
 */

#ifndef MASSHELICITYANGLESCORE_H_
#define MASSHELICITYANGLESCORE_H_

#include "CacheIntegrator/CachedIntegral.h"
#include "libComplexPDF/libComplexPDF.h"
#include "boost/math/constants/constants.hpp"
#include "libRooComplexPDF/RooMassHelicityAngles.h"

class MassHelicityAnglesCoreCachedIntegral : public CachedIntegral {
public:
	virtual std::string GetIntegralClassIdentifier() const = 0;

	virtual ROOT::Math::IBaseFunctionMultiDim* Clone() const = 0;

	virtual double DoEval(const double* x) const = 0;

protected:
	MassHelicityAnglesCoreCachedIntegral() : CachedIntegral() {};
	MassHelicityAnglesCoreCachedIntegral(const std::vector<std::pair<double, double> >& limits,
			const std::vector<double>& parameters,
			const double eps_abs = -1, const double eps_rel = -1, const double integral = -999, const int neval = -1)
	: CachedIntegral(limits,parameters,eps_abs,eps_rel,integral,neval) {};

	/**
	 * @brief Core function to get SphericalHarmonic*SphericalHarmonic*Resonance needed for integration
	 *
	 * @param m
	 * @param phi
	 * @param th1
	 * @param th2
	 * @param J
	 * @param lambda
	 * @param mass_0
	 * @param width_0
	 * @param a
	 * @param b
	 * @param mass_1
	 * @param width_1
	 * @param r_1
	 * @param mass_2
	 * @param width_2
	 * @param r_2
	 * @param m_Pi
	 * @param m_K
	 * @return
	 */
	std::complex<double> GetS(const double m, const double phi, const double th1, const double th2,
			const unsigned int J, const int lambda,
			const double mass_0, const double width_0, const double a, const double b,
			const double mass_1, const double width_1, const double r_1,
			const double mass_2, const double width_2, const double r_2,
			const double m_Pi, const double m_K) const {
		static const double pi = boost::math::constants::pi<double>();

		double q = ComplexPDF::CalculateMomentum(m,m_Pi,m_K);
		std::complex<double> Resonance;
		if(J==0) {
			double q_0 = ComplexPDF::CalculateMomentum(mass_0,m_Pi,m_K);
			Resonance = ComplexPDF::LASS(m,mass_0,width_0,a,b,q,q_0);
		} else if(J==1) {
			double q_1 = ComplexPDF::CalculateMomentum(mass_1,m_Pi,m_K);
			Resonance = ComplexPDF::RelativisticSpinBreitWigner(m,mass_1,width_1,r_1,1,q,q_1);
		} else if(J==2) {
			double q_2 = ComplexPDF::CalculateMomentum(mass_2,m_Pi,m_K);
			Resonance = ComplexPDF::RelativisticSpinBreitWigner(m,mass_2,width_2,r_2,2,q,q_2);
		} else {
			std::cerr << "ERROR: Spin " << J << " not implemented for GetS(..) in MassHelicityAnglesCore" << std::endl;
			std::exit(EXIT_FAILURE);
		}

		return ComplexPDF::SphericalHarmonic(J,lambda,th1,(pi-phi)) * ComplexPDF::SphericalHarmonic(1,-lambda,-th2,0) * Resonance;
	}
};

#endif /* MASSHELICITYANGLESCORE_H_ */
