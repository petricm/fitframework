/**
 * @file	JoinBatchJobs.cc
 * @date	May 27, 2011
 * @author	Michael Prim - mprim
 * @brief	A small program that joins integrals for a given list of folders
 *
 */

#include "FitFramework.h"
#include "mp_utility.h"

int main(int argc, char *argv[]) {
	if(argc < 2) {
		std::cerr << "Not enough arguments given... job numbers should be list of integers" << std::endl;
		return EXIT_FAILURE;
	}

    int debug_level = 1;
    CacheIntegrator<DalitzAnalysisNormCachedIntegral>* dalitz_analysis_norm_cache_integrator = new CacheIntegrator<DalitzAnalysisNormCachedIntegral>(debug_level);
    CacheIntegrator<DalitzAnalysisRealCachedIntegral>* dalitz_analysis_real_cache_integrator = new CacheIntegrator<DalitzAnalysisRealCachedIntegral>(debug_level);
    CacheIntegrator<DalitzAnalysisImagCachedIntegral>* dalitz_analysis_imag_cache_integrator = new CacheIntegrator<DalitzAnalysisImagCachedIntegral>(debug_level);


	for(int i = 1; i < 9; i++) {
		std::stringstream ss;
		ss << "Job_" << i;
		std::string path_to_integrals = "./CachedIntegrals_ACCEPTANCE_bin_2D/batch_job_norm/" + ss.str() + "/";
		// try to load one set of integrals for each type of integral
		dalitz_analysis_norm_cache_integrator->LoadIntegrals(path_to_integrals);
        dalitz_analysis_norm_cache_integrator->ComputeIntegrals();
	}
    
    for(int i = 1; i < 29; i++) {
		std::stringstream ss;
		ss << "Job_" << i;
		std::string path_to_integrals = "./CachedIntegrals_ACCEPTANCE_bin_2D/batch_job_real/" + ss.str() + "/";
		// try to load one set of integrals for each type of integral
		dalitz_analysis_real_cache_integrator->LoadIntegrals(path_to_integrals);
        dalitz_analysis_real_cache_integrator->ComputeIntegrals();
	}
    
    for(int i = 1; i < 29; i++) {
		std::stringstream ss;
		ss << "Job_" << i;
		std::string path_to_integrals = "./CachedIntegrals_ACCEPTANCE_bin_2D/batch_job_imag/" + ss.str() + "/";
		// try to load one set of integrals for each type of integral
		dalitz_analysis_imag_cache_integrator->LoadIntegrals(path_to_integrals);
        dalitz_analysis_imag_cache_integrator->ComputeIntegrals();
	}



	// now save the results again, this time with all the results from all batch jobs
	std::string path_to_integrals_results = "./CachedIntegrals_ACCEPTANCE_bin_2D/";
	std::string cmd = "mkdir -p " + path_to_integrals_results;
	mp::execute_command(cmd);
	std::cout << "INFO: Saving integrals in folder: " << path_to_integrals_results << std::endl;
	dalitz_analysis_norm_cache_integrator->SaveIntegrals(path_to_integrals_results);
	dalitz_analysis_real_cache_integrator->SaveIntegrals(path_to_integrals_results);
	dalitz_analysis_imag_cache_integrator->SaveIntegrals(path_to_integrals_results);


	delete dalitz_analysis_norm_cache_integrator;
	delete dalitz_analysis_real_cache_integrator;
	delete dalitz_analysis_imag_cache_integrator;


	return EXIT_SUCCESS;	
}
