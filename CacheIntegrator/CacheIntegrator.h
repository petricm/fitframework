#pragma once
/**
 * @file	CacheIntegrator.h
 * @date	May 18, 2011
 * @author	mprim
 * @brief	brief description
 *
 * long description
 */

#ifndef CACHEINTEGRATOR_H_
#define CACHEINTEGRATOR_H_

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <mutex>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

class CachedIntegral;

/**
 * A template class that serves as container for CachedIntegrals. It allows serialization and
 * deserialization of the integral members.
 */
template <class T>
class CacheIntegrator {
public:
	CacheIntegrator(int verbose = 0, int mode = 0) : m_verbose(verbose), m_mode(mode) {
		T integral;
		// we do this call to prevent this template of being used for other than from CachedIntegral derived classes
		// in C++0x we could also use an static assert for that during compile time...
		// we also could do a static_cast, however this will create a warning static_cast<CachedIntegral*>(&integral);
		if(m_verbose >= 1)
			std::cout << "INFO: CacheIntegrator for " << integral.GetIntegralClassIdentifier() << " created..." << std::endl;
	};
	~CacheIntegrator() {
		if(m_verbose >= 1)
			std::cout << "INFO: CacheIntegrator destroyed..." << std::endl;
	}

	/**
	 * Loads from given path the integrals from the file matching the integral typename. Can
	 * be called multiple times to join integrals from different files.
	 *
	 * @param path_to_files
	 */
	void LoadIntegrals(const std::string& path_to_files = "./") {
		T integral;
		std::string filename = path_to_files + integral.GetIntegralClassIdentifier() + ".dat.txt";
		if(m_verbose >= 1) {
			std::cout << "------------------------------------------" << std::endl;
			std::cout << "INFO: Loading Integrals from file " << filename << std::endl;
		}
		std::ifstream ifs(filename.c_str());
		if(ifs.is_open()) {
			// counter for loaded integrals
			unsigned int int_counter = 0;
			// load integral vector
			boost::archive::text_iarchive ia(ifs);
			std::vector<T> integrals;
			ia >> integrals;
			// lock vector with mutex to prevent parallel writes on the vector during adding integrals, unlock at end of scope
			std::unique_lock<std::mutex> lock(m_mutex);
			// search if loaded integral are already available
			for(unsigned int i = 0; i < integrals.size(); ++i) {
				typename std::vector<T>::iterator it;
				it = std::find(m_integrals.begin(), m_integrals.end(), integrals[i]);
				// loaded integral not found in current vector, add it!
				if(it == m_integrals.end()) {
					if(m_verbose >= 2) {
						std::cout << "DEBUG: Adding integral to CacheIntegrator:" << std::endl;
						std::cout << integrals[i].Print();
					}
					m_integrals.push_back(integrals[i]);
					++int_counter;
				} else {
					if(m_verbose >= 2) {
						std::cout << "DEBUG: Integral already found in CacheIntegrator:" << std::endl;
						std::cout << integrals[i].Print();
					}
				}
			}
			// sort internal integral storage after last fill
			std::sort(m_integrals.begin(), m_integrals.end());
			if(m_verbose >= 1) {
				std::cout << "INFO: " << int_counter << " integrals have been added to the storage. " << std::endl;
			}
			ifs.close();
		} else {
			std::cerr << "ERROR: Can't open filestream " << filename << " to load integrals." << std::endl;
		}
		if(m_verbose >= 1) {
			std::cout << "INFO: Loading integrals done" << std::endl;
		}
	}

	/**
	 * Saves the entire CacheIntegrator to a file in the given directory.
	 *
	 * @param path_to_files
	 */
	void SaveIntegrals(const std::string& path_to_files = "./") {
		SaveIntegrals(path_to_files,m_integrals);
	}

	/**
	 * Requests an integral of the function in given limits and with given parameters. If necessary
	 * it is computed (if not running in mode == 1) first and stored afterwards.
	 *
	 * @param limits
	 * @param params
	 * @return
	 */
	double GetIntegral(const std::vector<std::pair<double,double> >& limits,
			const std::vector<double>& params) {
		// create integral we are searching for and search for it
		T integral(limits,params);
		if(m_verbose >= 2) {
			std::cout << "------------------------------------------" << std::endl;
			std::cout << "DEBUG: Integral requested: " << std::endl;
			std::cout << integral.Print();
		}
		typename std::vector<T>::iterator it;
		// do fast search for integral, but expects sorted range
		m_mutex.lock();
		bool is_available = std::binary_search(m_integrals.begin(), m_integrals.end(), integral);
		m_mutex.unlock();
		// depending on search results, get precalculated value or integrate and then store result
		double result = 1;
		if(!is_available) {
			if(m_verbose >= 2)
				std::cout << "DEBUG: Integral not found, calculating..." << std::endl;
			// in mode 1 we don't compute the integral, we return 1 and store the
			// uncomputed integral. It will be computed at the next request in normal mode.
			if(m_mode == 0)
				result = integral.GetIntegral();
			else {
				if(m_verbose >= 2)
					std::cout << "DEBUG: Integral was not computed, just stored!" << std::endl;
			}
			if(m_verbose >= 2)
				std::cout << "DEBUG: " << integral.Print();
			// lock vector with mutex to prevent parallel writes on the vector and it changing its meaning
			std::unique_lock<std::mutex> uni_lock(m_mutex);
			// find first integral larger than given one
			it = std::upper_bound(m_integrals.begin(), m_integrals.end(), integral);
			// insert before to keep sorted range
			m_integrals.insert(it, integral);
		} else {
			if(m_verbose >= 2)
				std::cout << "DEBUG: Integral found and reused..." << std::endl;
			if(m_mode == 0) {
				std::lock_guard<std::mutex> lock(m_mutex);
				it = std::equal_range(m_integrals.begin(), m_integrals.end(), integral).first;
				result = (*it).GetIntegral();
			} else {
				if(m_verbose >= 2)
					std::cout << "DEBUG: Integral was not accessed to avoid computation!" << std::endl;
			}
		}
		return result;
	}

	/**
	 * Prints all integrals in the internal storage to given stream
	 */
	void PrintAllIntegrals(std::ostream& os) {
		typename std::vector<T>::iterator it;
		for(it = m_integrals.begin(); it != m_integrals.end(); ++it) {
			os << (*it).Print();
		}
	}

	/**
	 * Clears all integrals from internal storage
	 */
	void ClearAllIntegrals() {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_integrals.clear();
		std::cout << "INFO: CacheIntegrator cleared..." << std::endl;
	}

	/**
	 * Creates a set of 100 uncomputed integrals in subfolders below path_to_files. They
	 * can be loaded in a batch job program and computet with ComputeIntegrals.
	 *
	 * @param path_to_files
	 */
	void PrepareBatchJobs(const std::string& path_to_files = "./", unsigned int integrals_per_job = 100) {
		if(m_verbose >= 2) {
			std::cout << "------------------------------------------" << std::endl;
			std::cout << "DEBUG: Preparing batch job files... " << std::endl;
		}
		std::vector<T> batch_job_integrals;
		unsigned int batch_job_counter = 1;
		typename std::vector<T>::iterator it;
		for(it = m_integrals.begin(); it != m_integrals.end(); ++it) {
			// check if each integral was already computet or not
			if((*it).GetStatus() == -999) {
				batch_job_integrals.push_back(*it);
			}
			// if we have enough integrals, prepare the input for a batch job
			if(batch_job_integrals.size() >= integrals_per_job) {
				std::stringstream ss;
				ss << "Job_"  << batch_job_counter << "/";
				std::string cmd = "mkdir -p " + path_to_files + "/" + ss.str();
				if(system(cmd.c_str())) {
					std::cerr << "ERROR: Couldn't properly execute " << cmd << std::endl;
				}
				SaveIntegrals(path_to_files + "/" + ss.str(), batch_job_integrals);
				batch_job_integrals.clear();
				++batch_job_counter;
			}
		}
		// prepare the input for the final batch job
		std::stringstream ss;
		ss << "Job_"  << batch_job_counter << "/";
		std::string cmd = "mkdir -p " + path_to_files + "/" + ss.str();
		if(system(cmd.c_str())) {
			std::cerr << "ERROR: Couldn't properly execute " << cmd << std::endl;
		}
		SaveIntegrals(path_to_files + "/" + ss.str(), batch_job_integrals);
		batch_job_integrals.clear();
		++batch_job_counter;
	}

	/**
	 * This will trigger calculation for all uncomputed integrals. Useful für batch mode!
	 */
	void ComputeIntegrals() {
		if(m_verbose >= 2) {
			std::cout << "------------------------------------------" << std::endl;
			std::cout << "DEBUG: Computing all uncomputed integrals... " << std::endl;
		}
		std::vector<double> results;
		typename std::vector<T>::iterator it;
		for(it = m_integrals.begin(); it != m_integrals.end(); ++it) {
			if((*it).GetStatus() == -999) {
				results.push_back((*it).GetIntegral());
				if(m_verbose >= 2)
					std::cout << "DEBUG: " << (*it).Print();
			}
		}
		if(m_verbose >= 2) {
			std::cout << "DEBUG: Computing finished. " << std::endl;
		}
	}

	int GetVerbose() const {
		return m_verbose;
	}
	void SetVerbose(int verbose) {
		this->m_verbose = verbose;
	}

	int GetMode() const {
		return m_mode;
	}
	void SetMode(int mode) {
		this->m_mode = mode;
	}

private:

	/**
	 * Save given vector of integrals to given path, filename is taken from integral type
	 * @param path_to_files
	 * @param integrals_to_save
	 */
	void SaveIntegrals(const std::string& path_to_files, const std::vector<T>& integrals_to_save) {
		T integral;
		std::string filename = path_to_files + integral.GetIntegralClassIdentifier() + ".dat.txt";
		if(m_verbose >= 1) {
			std::cout << "------------------------------------------" << std::endl;
			std::cout << "INFO: Saving Integrals to file " << filename << std::endl;
		}
		// check if file exists and create backup before overwriting it
		std::ifstream ifs(filename.c_str());
		bool file_exists = false;
		if(ifs.is_open()) {
			file_exists = true;
			ifs.close();
		}
		if(file_exists) {
			std::string filename_backup = filename + ".backup";
			std::string cmd = "mv " + filename + " " + filename_backup;
			if(system(cmd.c_str())) {
				std::cerr << "ERROR: Couldn't properly execute " << cmd << std::endl;
			}
			std::cout << "WARNING: File " << filename << " already exists, moved to backup file " << filename_backup << std::endl;
		}

		// save integral vector
		std::ofstream ofs(filename.c_str());
		if(ofs.is_open()) {
			boost::archive::text_oarchive oa(ofs);
			oa << integrals_to_save;
			ofs.close();
		} else {
			std::cerr << "ERROR: Can't open filestream " << filename << " to save integrals." << std::endl;
		}
		if(m_verbose >= 1) {
			std::cout << "INFO: Saving integrals done (" << integrals_to_save.size() << " integrals stored)" << std::endl;
		}
	}

	// 0 = no verbose
	// 1 = verbose (INFO)
	// 2 = verbose (DEBUG)
	int m_verbose;
	// 0 = normal mode
	// 1 = don't compute integrals
	int m_mode;
	std::vector<T> m_integrals;
	std::mutex m_mutex;
};

#endif /* CACHEINTEGRATOR_H_ */
