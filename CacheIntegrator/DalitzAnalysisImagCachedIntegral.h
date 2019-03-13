#pragma once
/**
 * @file	DalitzAnalysisImagCachedIntegral.h
 * @date	Nov 11, 2013
 * @author	petric
 * @brief	brief description
 *
 * long description
 */

#ifndef DALITZANALYSISIMAGCACHEDINTEGRAL_H_
#define DALITZANALYSISIMAGCACHEDINTEGRAL_H_

#include "CacheIntegrator/CachedIntegralMC.h"
#include "CacheIntegrator/DalitzAnalysisCore.h"
#include "libComplexPDF/libComplexPDF.h"

/**
 * Imaginary part integral of the MassHelictyAngles distribtion integral.
 */
class DalitzAnalysisImagCachedIntegral : public DalitzAnalysisCoreCachedIntegral {
public:
	DalitzAnalysisImagCachedIntegral() : DalitzAnalysisCoreCachedIntegral() {};
	DalitzAnalysisImagCachedIntegral(const std::vector<std::pair<double, double> >& limits,
			const std::vector<double>& parameters,
			const double eps_abs = -1, const double eps_rel = -1, const double integral = -999, const int neval = -1)
	: DalitzAnalysisCoreCachedIntegral(limits,parameters,eps_abs,eps_rel,integral,neval) {};

	virtual double DoEval(const double* x) const {
		// variables x[]
		double mass_AB = x[0];
		double mass_BC = x[1];
		// variable parameters
		unsigned int index = (unsigned int)m_parameters[0];
		unsigned int index_cc = (unsigned int)m_parameters[1];
		// constant parameters for resonances
        double mass_1 = m_parameters[2];
        double width_1 = m_parameters[3];
        double r_1 = m_parameters[4];
        double mass_2 = m_parameters[5];
        double width_2 = m_parameters[6];
        double a_2 = m_parameters[7];
        double r_2 = m_parameters[8];
        double cutoff_2 = m_parameters[9];
        double mass_3 = m_parameters[10];
        double width_3 = m_parameters[11];
        double r_3 = m_parameters[12];
        double mass_4 = m_parameters[13];
        double width_4 = m_parameters[14];
        double r_4 = m_parameters[15];
        double mass_5 = m_parameters[16];
        double width_5 = m_parameters[17];
        double r_5 = m_parameters[18];
        double mass_6 = m_parameters[19];
        double width_6 = m_parameters[20];
        double r_6 = m_parameters[21];
        double mass_7 = m_parameters[22];
        double width_7 = m_parameters[23];
        double a_7 = m_parameters[24];
        double r_7 = m_parameters[25];
        double cutoff_7 = m_parameters[26];
		// constant parameters
		double m_B0 = m_parameters[27];
		double m_Pi0 = m_parameters[28];
		double m_Pi = m_parameters[29];
		double m_K = m_parameters[30];
		// efficiency function
		EFFICIENCY_FUNCTION eff_func = (EFFICIENCY_FUNCTION)((unsigned int)m_parameters[31]);

		std::complex<double> S = GetS(index, mass_AB, mass_BC, mass_1, width_1, r_1, mass_2, width_2, a_2, r_2, cutoff_2, mass_3, width_3, r_3, mass_4, width_4, r_4, mass_5, width_5, r_5, mass_6, width_6, r_6, mass_7, width_7, a_7, r_7, cutoff_7, m_B0, m_Pi0, m_Pi, m_K);
        
		std::complex<double> S_cc = std::conj(GetS(index_cc, mass_AB, mass_BC, mass_1, width_1, r_1, mass_2, width_2, a_2, r_2, cutoff_2, mass_3, width_3, r_3, mass_4, width_4, r_4, mass_5, width_5, r_5, mass_6, width_6, r_6, mass_7, width_7, a_7, r_7, cutoff_7, m_B0, m_Pi0, m_Pi, m_K));

		return GetEff(mass_AB,mass_BC,eff_func) * (S * S_cc).imag();
	}

	virtual ROOT::Math::IMultiGenFunction* Clone() const {
		DalitzAnalysisImagCachedIntegral* f = new DalitzAnalysisImagCachedIntegral(*this);
		return f;
	}

	virtual std::string GetIntegralClassIdentifier() const {
		return "DalitzAnalysisImagCachedIntegral";
	}
};

#endif /* DALITZANALYSISIMAGCACHEDINTEGRAL_H_ */
