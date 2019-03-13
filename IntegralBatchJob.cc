/**
 * @file	IntegralBatchJob.cc
 * @date	May 27, 2011
 * @author	Michael Prim - mprim
 * @brief	A small program that scans for integrals in a given folder and computes all of them and stores the results
 *
 */

#include "FitFramework.h"
#include "mp_utility.h"

int main(int argc, char *argv[]) {
	std::stringstream ss;
	if(argc >= 2) {
		ss << "Job_" << argv[1] << "/";
	} else {
		std::cerr << "Not enough arguments given... job number should be an integer" << std::endl;
		return EXIT_FAILURE;
	}
    
    int debug_level = 1;
    CacheIntegrator<DalitzAnalysisNormCachedIntegral>* dalitz_analysis_norm_cache_integrator = new CacheIntegrator<DalitzAnalysisNormCachedIntegral>(debug_level);
    CacheIntegrator<DalitzAnalysisRealCachedIntegral>* dalitz_analysis_real_cache_integrator = new CacheIntegrator<DalitzAnalysisRealCachedIntegral>(debug_level);
    CacheIntegrator<DalitzAnalysisImagCachedIntegral>* dalitz_analysis_imag_cache_integrator = new CacheIntegrator<DalitzAnalysisImagCachedIntegral>(debug_level);
    
    
    if (!strcmp(argv[2], "norm")) {
        std::string path_to_integrals = "./CachedIntegrals_ACCEPTANCE_bin_2D/batch_job_norm/" + ss.str();
        std::cout << "INFO: Computing integrals in folder: " << path_to_integrals;
        dalitz_analysis_norm_cache_integrator->LoadIntegrals(path_to_integrals);
        dalitz_analysis_norm_cache_integrator->ComputeIntegrals();
        dalitz_analysis_norm_cache_integrator->SaveIntegrals(path_to_integrals);
    }
    
    if (!strcmp(argv[2], "real")) {
        std::string path_to_integrals = "./CachedIntegrals_ACCEPTANCE_bin_2D/batch_job_real/" + ss.str();
        std::cout << "INFO: Computing integrals in folder: " << path_to_integrals;
        dalitz_analysis_real_cache_integrator->LoadIntegrals(path_to_integrals);
        dalitz_analysis_real_cache_integrator->ComputeIntegrals();
        dalitz_analysis_real_cache_integrator->SaveIntegrals(path_to_integrals);
    }
    
    if (!strcmp(argv[2], "imag")) {
        std::string path_to_integrals = "./CachedIntegrals_ACCEPTANCE_bin_2D/batch_job_imag/" + ss.str();
        std::cout << "INFO: Computing integrals in folder: " << path_to_integrals;
        dalitz_analysis_imag_cache_integrator->LoadIntegrals(path_to_integrals);
        dalitz_analysis_imag_cache_integrator->ComputeIntegrals();
        dalitz_analysis_imag_cache_integrator->SaveIntegrals(path_to_integrals);
    }
    
	delete dalitz_analysis_norm_cache_integrator;
	delete dalitz_analysis_real_cache_integrator;
	delete dalitz_analysis_imag_cache_integrator;

    
	return EXIT_SUCCESS;	
}
