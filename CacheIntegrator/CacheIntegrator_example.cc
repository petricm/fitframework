/**
 * @file	CacheIntegrator_example.cc
 * @date	May 27, 2011
 * @author	Michael Prim - mprim
 * @brief	A small example of how to use the CacheIntegrator
 *
 * This example shows how to derive a class from CachedIntegral and what methods need to be implemented.
 * It furthermore shows how the initialize a CacheIntegrator for a certain type of integral and how to use it.
 */

#include <iostream>
#include <vector>
#include "CachedIntegral.h"
#include "CacheIntegrator.h"

// Compile with: g++ CacheIntegrator_example.cc -g -o CacheIntegrator_example `root-config --cflags` `root-config --libs` -lboost_serialization -lboost_thread

/**
 * Integral of ax+by+cz over dx dy dz
 */
class LinearCachedIntegral : public CachedIntegral {
public:
	LinearCachedIntegral() : CachedIntegral() {};
	LinearCachedIntegral(const std::vector<std::pair<double, double> >& limits,
						const std::vector<double>& parameters,
						const double eps_abs = -1, const double eps_rel = -1, const double integral = -999, const int neval = -1)
	: CachedIntegral(limits,parameters,eps_abs,eps_rel,integral,neval) {};

	virtual double DoEval(const double* x) const {
		double a = m_parameters[0];
		double b = m_parameters[1];
		double c = m_parameters[2];

		return a*x[0]+b*x[1]+c*x[2];
	}

	virtual ROOT::Math::IBaseFunctionMultiDim* Clone() const {
		LinearCachedIntegral* f = new LinearCachedIntegral(*this);
		return f;
	}

	virtual std::string GetIntegralClassIdentifier() const {
		return "LinearCachedIntegral";
	}
};

/**
 * Analytic Integral of ax+by+cz over dx dy dz
 *
 * @param limits limits in x,y,z
 * @param parameters a,b,c
 * @return
 */
double analytic_integral(std::vector<std::pair<double, double> > limits, std::vector<double> parameters) {
	// 1/2 (xmax-xmin) (ymax-ymin) (zmax-zmin) (a (xmax+xmin)+b (ymax+ymin)+c (zmax+zmin))
	double result = 0.5;
	result *= (limits[0].second - limits[0].first) * (limits[1].second - limits[1].first) * (limits[2].second - limits[2].first);
	result *= (parameters[0]*(limits[0].second + limits[0].first) + parameters[1]*(limits[1].second + limits[1].first) + parameters[2]*(limits[2].second + limits[2].first));
	return result;
}


/**
 * @brief Creates a std::string representation of std::pair<T1,T2> if T1 and T2 overload << operator
 *
 * @param pair pair to convert
 * @return string of pair
 */
template <class T1, class T2>
inline std::string pair_to_string(const std::pair<T1, T2> &pair) {
    std::stringstream ss;
    ss << "Pair[" << pair.first << "," << pair.second << "]";
    return ss.str();
}

int main(int argc, char *argv[]) {
	CacheIntegrator<LinearCachedIntegral> *storage = new CacheIntegrator<LinearCachedIntegral>(2,0);
	storage->LoadIntegrals();

	std::vector<double> parameters;
	parameters.assign(3,1);

	std::vector<std::pair<double, double> > limits;
	limits.assign(3,std::make_pair(0,0.5));
	std::cout << "Limits in x,y,z: " << pair_to_string(limits[0]) << " Integral is: " << storage->GetIntegral(limits,parameters) << " Analytic is: " << analytic_integral(limits,parameters) << std::endl;
	limits.assign(3,std::make_pair(0,1.0));
	std::cout << "Limits in x,y,z: " << pair_to_string(limits[0]) << " Integral is: " << storage->GetIntegral(limits,parameters) << " Analytic is: " << analytic_integral(limits,parameters) << std::endl;
	limits.assign(3,std::make_pair(0,1.5));
	std::cout << "Limits in x,y,z: " << pair_to_string(limits[0]) << " Integral is: " << storage->GetIntegral(limits,parameters) << " Analytic is: " << analytic_integral(limits,parameters) << std::endl;
	limits.assign(3,std::make_pair(0,2.0));
	std::cout << "Limits in x,y,z: " << pair_to_string(limits[0]) << " Integral is: " << storage->GetIntegral(limits,parameters) << " Analytic is: " << analytic_integral(limits,parameters) << std::endl;
	limits.assign(3,std::make_pair(0,2.5));
	std::cout << "Limits in x,y,z: " << pair_to_string(limits[0]) << " Integral is: " << storage->GetIntegral(limits,parameters) << " Analytic is: " << analytic_integral(limits,parameters) << std::endl;

	storage->SaveIntegrals();

	// switch to batch mode
	storage->SetMode(1);
	for(float f = 0.01; f <= 1.0; f += 0.01) {
		limits.assign(3,std::make_pair(0,f));
		storage->GetIntegral(limits,parameters);
	}
	storage->PrepareBatchJobs("batch_job_example/",30);
	// switch back to normal mode
	storage->SetMode(0);

	// now load all the batch jobs (could be also an independent program and thus a real batch job)
	std::stringstream ss;
	storage->LoadIntegrals("batch_job_example/Job_1/");
	storage->LoadIntegrals("batch_job_example/Job_2/");
	storage->LoadIntegrals("batch_job_example/Job_3/");
	storage->LoadIntegrals("batch_job_example/Job_4/");
	// now compute all the integrals, that aren't yet computed
	storage->ComputeIntegrals();

	for(float f = 0.01; f <= 1.0; f += 0.01) {
		limits.assign(3,std::make_pair(0,f));
		std::cout << storage->GetIntegral(limits,parameters) << " == " << analytic_integral(limits,parameters) << std::endl;
	}

	// now save the results again, this time with all the results from the batch jobs
	storage->SaveIntegrals();

	delete storage;
	return EXIT_SUCCESS;	
}
