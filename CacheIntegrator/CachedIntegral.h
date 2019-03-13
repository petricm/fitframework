#pragma once
/**
 * @file	CachedIntegral.h
 * @date	May 17, 2011
 * @author	mprim
 * @brief	brief description
 *
 * long description
 */

#ifndef CACHEDINTEGRAL_H_
#define CACHEDINTEGRAL_H_

#include <vector>
#include <string>
#include <sstream>
#include "Math/AdaptiveIntegratorMultiDim.h"
#include "Math/IFunction.h"
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

/**
 * A abstract base class that contains all necessary information an integral needs. E.g. limits, parameters etc...
 * The derived class needs to implement the constructors, DoEval, Clone and GetIntegralClassIdentifier methods.
 *
 * The Print method that returns a string representation may be overwritten.
 * Also the Integrate method may be overwritten if there is a better method than the default one (e.g. analytical)
 */
class CachedIntegral : public ROOT::Math::IBaseFunctionMultiDim {
public:
	virtual ~CachedIntegral() { }

	virtual std::string GetIntegralClassIdentifier() const = 0;

	virtual ROOT::Math::IBaseFunctionMultiDim* Clone() const = 0;

	double Evaluate(const std::vector<double> &vars) const {
		return this->DoEval(&vars[0]);
	}

	virtual std::string Print() const {
		std::stringstream ss;
		ss << "---------------------------------" << "\n";
		ss << "An integral (" << this->GetIntegralClassIdentifier() << ") with " << m_dimensions << " dimensions." << "\n";
		ss << "Limits are: ";
		for(unsigned int i = 0; i < m_limits.size(); ++i) {
			if(i != 0) {
				ss << ", ";
			}
			ss << "[" << m_limits[i].first << "," << m_limits[i].second << "]";
		}
		ss << "\n";
		ss << "Parameters are: [";
		for(unsigned int i = 0; i < m_parameters.size(); ++i) {
			if(i != 0) {
				ss << ", ";
			}
			ss << m_parameters[i];
		}
		ss << "]" << "\n";
		if(m_integral != -999) {
			ss << "Result: " << m_integral << " +- " << m_eps_abs << " RelEps: " << m_eps_rel << " NEval: " << m_neval << "\n";
		} else {
			ss << "Integral not yet calculated..." << "\n";
		}
		ss << "---------------------------------" << "\n";
		return ss.str();
	}

	virtual unsigned int NDim() const {
		return m_dimensions;
	}

	unsigned int GetNDim() const {
		return m_dimensions;
	}

	unsigned int GetNParameters() const {
		return m_parameters.size();
	}

	std::vector<double> GetParameters() const {
		return m_parameters;
	}

	std::vector<std::pair<double,double> > GetLimits() const {
		return m_limits;
	}

	double GetIntegral() {
		if(m_integral == -999) {
			Integrate();
		}
		return m_integral;
	}

	double GetEpsAbs() {
		if(m_eps_abs == -1) {
			Integrate();
		}
		return m_eps_abs;
	}

	double GetEbsRel() {
		if(m_eps_rel == -1) {
			Integrate();
		}
		return m_eps_rel;
	}

	int GetNEval() {
		if(m_neval == -1) {
			Integrate();
		}
		return m_neval;
	}

	int GetStatus() const {
		return m_status;
	}

	bool operator==(const CachedIntegral& rhs) const {
		return m_parameters == rhs.m_parameters && m_dimensions == rhs.m_dimensions && m_limits == rhs.m_limits;
	}

	bool operator<(const CachedIntegral& rhs) const {
		// compare parameter values (most likely to be different)
		if(m_parameters < rhs.m_parameters)
			return true;
		else if(m_parameters == rhs.m_parameters) {
			// compare dimension
			if(m_dimensions < rhs.m_dimensions)
				return true;
			else if(m_dimensions == rhs.m_dimensions) {
				// compare limits
				if(m_limits < rhs.m_limits)
					return true;
			}
		}
		return false;
	}

protected:
	virtual double DoEval(const double* x) const = 0;

	virtual void Integrate() {
		ROOT::Math::IBaseFunctionMultiDim *func = dynamic_cast<ROOT::Math::IBaseFunctionMultiDim*>(this);
		const double eps = 1e-6;
		unsigned int nmaxeval = 1000000000;
        
		ROOT::Math::AdaptiveIntegratorMultiDim *integrator = new ROOT::Math::AdaptiveIntegratorMultiDim(*func,eps,eps,nmaxeval);

		double *x_min = new double[m_dimensions];
		double *x_max = new double[m_dimensions];

		for(unsigned int i = 0; i < m_dimensions; ++i) {
			x_min[i] = m_limits[i].first;
			x_max[i] = m_limits[i].second;
		}

		m_integral = integrator->Integral(x_min, x_max);
		m_eps_rel = integrator->RelError();
		m_eps_abs = integrator->Error();
		m_neval = integrator->NEval();
		m_status = integrator->Status();

		if(integrator->Status()) {
			std::cerr << "---------------------------------------------" << std::endl;
			std::cerr << "ERROR: Integration status " << integrator->Status() << " indicates an error..." << std::endl;
			std::cerr << this->Print();
			std::cerr << "ERROR: " << "Integral: " << integrator->Result() << " +- " << integrator->Error() << std::endl;
			std::cerr << "ERROR: " << "RelErr:   " << integrator->RelError() << std::endl;
			std::cerr << "ERROR: " << "NEval:    " << integrator->NEval() << std::endl;
			std::cerr << "ERROR: Integration options:" << std::endl;
			integrator->Options().Print(std::cerr);
			if(integrator->Error() < eps && integrator->RelError() > eps && integrator->Result() < eps) {
				std::cerr << "ERROR: Integral is close to zero, absolute error is smaller than " << eps << "... Integral is probably fine" << std::endl;
			} else {
				std::cerr << "ERROR: Something went probably really wrong... Check this integral!" << std::endl;
			}
		}

		delete[] x_max;
		delete[] x_min;
		delete integrator;
	}

    CachedIntegral() : m_dimensions(0), m_eps_abs(-1), m_eps_rel(-1), m_integral(-999), m_neval(-1), m_status(-999) { }
	CachedIntegral(const std::vector<std::pair<double, double> >& limits,
				const std::vector<double>& parameters,
				const double eps_abs = -1, const double eps_rel = -1, const double integral = -999, const int neval = -1, const int status = -999) :
				m_dimensions(limits.size()), m_limits(limits), m_parameters(parameters),
				m_eps_abs(eps_abs), m_eps_rel(eps_rel), m_integral(integral), m_neval(neval), m_status(status) { }

	CachedIntegral(const CachedIntegral& rhs)
		: ROOT::Math::IBaseFunctionMultiDim(), m_dimensions(rhs.m_dimensions), m_limits(rhs.m_limits), m_parameters(rhs.m_parameters),
		  m_eps_abs(rhs.m_eps_abs), m_eps_rel(rhs.m_eps_rel), m_integral(rhs.m_integral), m_neval(rhs.m_neval), m_status(rhs.m_status) { }

	unsigned int m_dimensions;
	std::vector<std::pair<double, double> > m_limits;
	std::vector<double> m_parameters;
	double m_eps_abs;
	double m_eps_rel;
	double m_integral;
	int m_neval;
	int m_status;

	// code for serialization/deserialization
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int /* version */) {
		ar & m_dimensions;
		ar & m_limits;
		ar & m_parameters;
		ar & m_eps_abs;
		ar & m_eps_rel;
		ar & m_integral;
		ar & m_neval;
		ar & m_status;
	}
};

#endif /* CACHEDINTEGRAL_H_ */
