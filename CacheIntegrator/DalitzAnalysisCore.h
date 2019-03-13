#pragma once
/**
 * @file	DalitzAnalysisCoreCachedIntegral.h
 * @date	Nov 10, 2013
 * @author	mpetric
 * @brief	brief description
 *
 * long description
 */

#ifndef DALITZANALYSISCORE_H_
#define DALITZANALYSISCORE_H_

#include "CacheIntegrator/CachedIntegralMC.h"
#include "libComplexPDF/libComplexPDF.h"
#include "boost/math/constants/constants.hpp"
#include "libRooComplexPDF/RooDalitzAnalysis.h"

class DalitzAnalysisCoreCachedIntegral : public CachedIntegralMC {
public:
	virtual std::string GetIntegralClassIdentifier() const = 0;
    
	virtual ROOT::Math::IMultiGenFunction* Clone() const = 0;
    
	virtual double DoEval(const double* x) const = 0;
    
protected:
	DalitzAnalysisCoreCachedIntegral() : CachedIntegralMC() {};
	DalitzAnalysisCoreCachedIntegral(const std::vector<std::pair<double, double> >& limits,
                                     const std::vector<double>& parameters,
                                     const double eps_abs = -1, const double eps_rel = -1, const double integral = -999, const int neval = -1)
	: CachedIntegralMC(limits,parameters,eps_abs,eps_rel,integral,neval) {};
    
	/**
	 * @brief Core function to get Matrix element for each resonance
	 *
	 * @param	index
	 * @param	mass_AB
	 * @param	mass_BC
	 * @param	mass_1
	 * @param	width_1
	 * @param	r_1
	 * @param	mass_2
	 * @param	width_2
	 * @param	a_2
	 * @param	r_2
	 * @param	cutoff_2
	 * @param	mass_3
	 * @param	width_3
	 * @param	r_3
	 * @param	mass_4
	 * @param	width_4
	 * @param	r_4
	 * @param	mass_5
	 * @param	width_5
	 * @param	r_5
	 * @param	mass_6
	 * @param	width_6
	 * @param	r_6
	 * @param	mass_7
	 * @param	width_7
	 * @param	a_7
	 * @param	r_7
	 * @return
	 */
	std::complex<double> GetS(const int index, const double mass_AB, const double mass_BC, const double mass_1, const double width_1, const double r_1, const double mass_2, const double width_2, const double a_2, const double r_2, const double cutoff_2, const double mass_3, const double width_3, const double r_3, const double mass_4, const double width_4, const double r_4, const double mass_5, const double width_5, const double r_5, const double mass_6, const double width_6, const double r_6, const double mass_7, const double width_7, const double a_7, const double r_7, const double cutoff_7, const double m_B0, const double m_Pi0, const double m_Pi, const double m_K) const {
        
        double MAB = std::sqrt(mass_AB);
        double MBC = std::sqrt(mass_BC);
        // double MCA = TMath::Sqrt(TMath::Abs(m_B0*m_B0 + m_K*m_K + m_Pi*m_Pi + m_Pi0*m_Pi0 - MAB*MAB - MBC*MBC)); //Actually not neede sinde it's calculated inside CompexPDF
        
        if(ComplexPDF::InsideDalitzLimits(MAB,MBC,m_B0,m_Pi0,m_Pi,m_K)){
            std::complex<double> Resonance;
            if(index==1) {
                Resonance = ComplexPDF::GetBreitWignerMatrixElement(MAB, MBC, ComplexPDF::CA, ComplexPDF::AB, mass_1, width_1, r_1, 1, m_B0, m_Pi0, m_Pi, m_K);
            } else if(index==2) {
                Resonance = ComplexPDF::GetLASSMatrixElement(MAB, MBC, ComplexPDF::CA, mass_2, width_2, a_2, r_2, cutoff_2, m_B0, m_Pi0, m_Pi, m_K);
            } else if(index==3) {
                Resonance = ComplexPDF::GetBreitWignerMatrixElement(MAB, MBC, ComplexPDF::AB, ComplexPDF::BC, mass_3, width_3, r_3, 1, m_B0, m_Pi0, m_Pi, m_K);
            } else if(index==4) {
                Resonance = ComplexPDF::GetBreitWignerMatrixElement(MAB, MBC, ComplexPDF::AB, ComplexPDF::BC, mass_4, width_4, r_4, 1, m_B0, m_Pi0, m_Pi, m_K);
            } else if(index==5) {
                Resonance = ComplexPDF::GetBreitWignerMatrixElement(MAB, MBC, ComplexPDF::AB, ComplexPDF::BC, mass_5, width_5, r_5, 1, m_B0, m_Pi0, m_Pi, m_K);
            } else if(index==6) {
                Resonance = ComplexPDF::GetBreitWignerMatrixElement(MAB, MBC, ComplexPDF::BC, ComplexPDF::CA, mass_6, width_6, r_6, 1, m_B0, m_Pi0, m_Pi, m_K);
            } else if(index==7) {
                Resonance = ComplexPDF::GetLASSMatrixElement(MAB, MBC, ComplexPDF::BC, mass_7, width_7, a_7, r_7, cutoff_7, m_B0, m_Pi0, m_Pi, m_K);
            } else if(index==8) {
                Resonance = ComplexPDF::DalitzPhaseSpaceAmplitude(MAB,MBC);
            } else {
                std::cerr << "ERROR: Index " << index << " not implemented for GetS(..) in DalitzAnalysisCore" << std::endl;
                std::exit(EXIT_FAILURE);
            }
            return Resonance;
        }else{
            return 0.0;
        }
        
        return 0.0;
        
	}
};

#endif /* DALITZANALYSISCORE_H_ */
